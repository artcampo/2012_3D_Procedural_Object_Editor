#include "OpStack.hpp"

using namespace ObjEd;

namespace OpStackHelpers{
bool  compareSelections( const std::vector<ObjEd::Selection>&s1, const std::vector<ObjEd::Selection>& s2 )
{
  if (s1.size() != s2.size())
    return false;
  bool equals = true;
  for (unsigned int i = 0; i < s1.size() && equals ; ++i)
    if ( !(
           s1[i].idComponent == s2[i].idComponent &&
           s1[i].typeEntity == s2[i].typeEntity &&
           s1[i].idEntity == s2[i].idEntity
           ))
      equals = false;
  return equals;
}
};

OpStack::~OpStack()
{
  mOpId.clear();

  mPrim.clear();
  mExt.clear();
  mExtI.clear();
  mExtM.clear();
  mNorS.clear();
  mKniL.clear();
  mBevV.clear();
  mBevE.clear();
  mTrans.clear();
  mScale.clear();
  mIndexOp.clear();

}

OpStack::OpStack()
{
  mNumOp          = 0;
  mNumComponents  = 0;
}


OpStack::OpStack( const OpStack& rhs)
{
  mNumOp          = rhs.mNumOp;
  mOpId           = rhs.mOpId;

  mPrim           = rhs.mPrim;

  mExt            = rhs.mExt;
  mExtI           = rhs.mExtI;
  mExtM           = rhs.mExtM;
  mNorS           = rhs.mNorS;

  mBevV           = rhs.mBevV;
  mBevE           = rhs.mBevE;

  mKniL           = rhs.mKniL;

  mTrans          = rhs.mTrans;
  mScale          = rhs.mScale;
}

void OpStack::queueOperation ( const OpInfo::OpExtrude& op )
{
  mOpId.push_back( OpId::eExtrude );
  mExt.push_back ( op );
  ++mNumOp;
  simplifyQueue();
}

void OpStack::queueOperation ( const OpInfo::OpExtrudeMatrix& op )
{
  mOpId.push_back( OpId::eExtrudeMatrix );
  mExtM.push_back ( op );
  ++mNumOp;
  simplifyQueue();
}

void OpStack::queueOperation ( const OpInfo::OpNormScale& op )
{
  mOpId.push_back( OpId::eNormalScale );
  mNorS.push_back( op );
  ++mNumOp;
  simplifyQueue();
}


void OpStack::queueOperation ( const OpInfo::OpExtrudeInner& op )
{
  mOpId.push_back( OpId::eExtrudeInner );
  mExtI.push_back ( op );
  ++mNumOp;
  simplifyQueue();
}

void OpStack::queueOperation ( const OpInfo::OpBevelV& op )
{
  mOpId.push_back( OpId::eBevelV );
  mBevV.push_back ( op );
  ++mNumOp;

  simplifyQueue();
}


void OpStack::queueOperation ( const OpInfo::OpBevelE& op )
{
  mOpId.push_back( OpId::eBevelE );
  mBevE.push_back ( op );
  ++mNumOp;

  simplifyQueue();
}

void OpStack::queueOperation ( const OpInfo::OpPrimCube& op)
{
  mOpId.push_back( OpId::ePrimCube );
  mPrim.push_back ( op );
  ++mNumOp;
}

void OpStack::queueOperation ( const OpInfo::OpTranslate& op)
{
  mOpId.push_back( OpId::eTranslate );
  mTrans.push_back ( op );
  ++mNumOp;
}

void OpStack::queueOperation ( const OpInfo::OpScale& op)
{
  mOpId.push_back( OpId::eScale );
  mScale.push_back ( op );
  ++mNumOp;
}

void OpStack::queueOperation ( const OpInfo::OpKnifeLine& op)
{
  mOpId.push_back( OpId::eKniveLine );
  mKniL.push_back ( op );
  ++mNumOp;
}

void OpStack::simplifyQueue()
{
  if ( mNumOp > 2)
  {
    if ( mOpId[mNumOp - 1] == OpId::eNormalScale )
      simplifyNormalScale();
  }
}

void OpStack::simplifyNormalScale()
{
  /*
  if ( mOpId[mNumOp - 1] == eOpId::eOpModeNormalScale &&
       mOpId[mNumOp - 2] == eOpId::eOpModeNormalScale &&
       OpStackHelpers::compareSelections() )
  {
  }
*/
}

 int OpStack::numOp() const
 {
   return mNumOp;
 }

 int OpStack::opId(int i) const
 {
   return mOpId[i];
 }

 void OpStack::prepareReconstruction()
 {
   mNumComponents = 0;
   mIndexOp.resize( mNumOp );

   std::vector<int> indexOp ( OpId::gsNumOpIds );
   for ( unsigned int i = 0 ; i < OpId::gsNumOpIds ; ++i )
     indexOp[i] = 0;

   for ( unsigned int i = 0 ; i < mNumOp ; ++i )
   {
     mIndexOp[i] = indexOp[ mOpId[i] ];
     ++indexOp[ mOpId[i] ];
   }
 }

 OpInfo::OpPrimCube OpStack::opPrimCube(int i)
 {
   return mPrim[ mIndexOp[i] ];
 }

 OpInfo::OpExtrude OpStack::opExt(int i)
 {
   return mExt[ mIndexOp[i] ];
 }

 OpInfo::OpExtrudeInner   OpStack::opExtI(int i)
 {
   return mExtI[ mIndexOp[i] ];
 }

 OpInfo::OpExtrudeMatrix   OpStack::opExtM(int i)
 {
   return mExtM[ mIndexOp[i] ];
 }

 OpInfo::OpNormScale   OpStack::opNorS(int i)
 {
   return mNorS[ mIndexOp[i] ];
 }



 OpInfo::OpBevelV OpStack::opBevV(int i)
 {
   return mBevV[ mIndexOp[i] ];
 }

 OpInfo::OpTranslate OpStack::opTrans(int i)
 {
   return mTrans[ mIndexOp[i] ];
 }

 OpInfo::OpKnifeLine OpStack::opKniL( int i )
 {
   return mKniL[ mIndexOp[i] ];
 }

 OpInfo::OpScale OpStack::opScale(int i)
 {
   return mScale[ mIndexOp[i] ];
 }

 OpInfo::OpBevelE OpStack::opBevE(int i)
 {
   return mBevE[ mIndexOp[i] ];
 }


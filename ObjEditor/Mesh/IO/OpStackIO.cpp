#include "ObjEditor/Mesh/OpStack.hpp"

///////////////////////////////////////////////////////////////
// QT IO
///////////////////////////////////////////////////////////////

#ifdef _MESH_QT_IO

void OpStack::save(QDataStream &outs)
{
  outs << mNumOp;

  for ( int i = 0; i < mNumOp ; ++i)
    outs << mOpId[i];

  outs << int( mPrim.size() );
  for ( int i = 0; i < mPrim.size() ; ++i)
    outs << mPrim[i];

  outs << int( mExt.size() );
  for ( int i = 0; i < mExt.size() ; ++i)
    outs << mExt[i];

  outs << int( mExtI.size() );
  for ( int i = 0; i < mExtI.size() ; ++i)
    outs << mExtI[i];

  outs << int( mExtM.size() );
  for ( int i = 0; i < mExtM.size() ; ++i)
    outs << mExtM[i];

  outs << int( mNorS.size() );
  for ( int i = 0; i < mNorS.size() ; ++i)
    outs << mNorS[i];

  outs << int( mBevV.size() );
  for ( int i = 0; i < mBevV.size() ; ++i)
    outs << mBevV[i];

  outs << int( mBevE.size() );
  for ( int i = 0; i < mBevE.size() ; ++i)
    outs << mBevE[i];

  outs << int( mKniL.size() );
  for ( int i = 0; i < mKniL.size() ; ++i)
    outs << mKniL[i];

  outs << int( mTrans.size() );
  for ( int i = 0; i < mTrans.size() ; ++i)
    outs << mTrans[i];

  outs << int( mScale.size() );
  for ( int i = 0; i < mScale.size() ; ++i)
    outs << mScale[i];

}

 void OpStack::load(QDataStream &ins)
{
   int temp;
   ins >> mNumOp;

   mOpId.resize( mNumOp );
   for ( int i = 0; i < mNumOp ; ++i)
     ins >> mOpId[i];

   ins >> temp;
   mPrim.resize(temp);
   for ( int i = 0; i < mPrim.size() ; ++i)
     ins >> mPrim[i];

   ins >> temp;
   mExt.resize(temp);
   for ( int i = 0; i < mExt.size() ; ++i)
     ins >> mExt[i];

   ins >> temp;
   mExtI.resize(temp);
   for ( int i = 0; i < mExtI.size() ; ++i)
     ins >> mExtI[i];

   ins >> temp;
   mExtM.resize(temp);
   for ( int i = 0; i < mExtM.size() ; ++i)
     ins >> mExtM[i];

   ins >> temp;
   mNorS.resize(temp);
   for ( int i = 0; i < mNorS.size() ; ++i)
     ins >> mNorS[i];

   ins >> temp;
   mBevV.resize(temp);
   for ( int i = 0; i < mBevV.size() ; ++i)
     ins >> mBevV[i];

   ins >> temp;
   mBevE.resize(temp);
   for ( int i = 0; i < mBevE.size() ; ++i)
     ins >> mBevE[i];

   ins >> temp;
   mKniL.resize(temp);
   for ( int i = 0; i < mKniL.size() ; ++i)
     ins >> mKniL[i];

   ins >> temp;
   mTrans.resize(temp);
   for ( int i = 0; i < mTrans.size() ; ++i)
     ins >> mTrans[i];

   ins >> temp;
   mScale.resize(temp);
   for ( int i = 0; i < mScale.size() ; ++i)
     ins >> mScale[i];


}

#endif

 ///////////////////////////////////////////////////////////////
 // CPP IO
 ///////////////////////////////////////////////////////////////

#ifdef _MESH_CPP_IO

 void OpStack::save(std::ofstream& outs)
 {
   outs << mNumOp << endl;


   for ( int i = 0; i < mNumOp ; ++i)
     outs << mOpId[i] << endl;

   outs << int( mPrim.size() ) << endl;
   for ( int i = 0; i < mPrim.size() ; ++i)
     outs << mPrim[i] << endl;

   outs << int( mExt.size() ) << endl;
   for ( int i = 0; i < mExt.size() ; ++i)
     outs << mExt[i] << endl;

   outs << int( mExtI.size() ) << endl;
   for ( int i = 0; i < mExtI.size() ; ++i)
     outs << mExtI[i] << endl;

   outs << int( mExtM.size() ) << endl;
   for ( int i = 0; i < mExtM.size() ; ++i)
     outs << mExtM[i] << endl;

   outs << int( mNorS.size() ) << endl;
   for ( int i = 0; i < mNorS.size() ; ++i)
     outs << mNorS[i] << endl;

   outs << int( mBevV.size() ) << endl;
   for ( int i = 0; i < mBevV.size() ; ++i)
     outs << mBevV[i] << endl;

   outs << int( mBevE.size() ) << endl;
   for ( int i = 0; i < mBevE.size() ; ++i)
     outs << mBevE[i] << endl;

   outs << int( mKniL.size() ) << endl;
   for ( int i = 0; i < mKniL.size() ; ++i)
     outs << mKniL[i] << endl;

   outs << int( mTrans.size() ) << endl;
   for ( int i = 0; i < mTrans.size() ; ++i)
     outs << mTrans[i] << endl;

   outs << int( mScale.size() ) << endl;
   for ( int i = 0; i < mScale.size() ; ++i)
     outs << mScale[i] << endl;

 }

  void OpStack::load(std::fstream& ins)
 {
    int temp;
    ins >> mNumOp;

    mOpId.resize( mNumOp );
    for ( int i = 0; i < mNumOp ; ++i)
      ins >> mOpId[i];

    ins >> temp;
    mPrim.resize(temp);
    for ( int i = 0; i < mPrim.size() ; ++i)
      ins >> mPrim[i];

    ins >> temp;
    mExt.resize(temp);
    for ( int i = 0; i < mExt.size() ; ++i)
      ins >> mExt[i];

    ins >> temp;
    mExtI.resize(temp);
    for ( int i = 0; i < mExtI.size() ; ++i)
      ins >> mExtI[i];

    ins >> temp;
    mExtM.resize(temp);
    for ( int i = 0; i < mExtM.size() ; ++i)
      ins >> mExtM[i];

    ins >> temp;
    mNorS.resize(temp);
    for ( int i = 0; i < mNorS.size() ; ++i)
      ins >> mNorS[i];

    ins >> temp;
    mBevV.resize(temp);
    for ( int i = 0; i < mBevV.size() ; ++i)
      ins >> mBevV[i];

    ins >> temp;
    mBevE.resize(temp);
    for ( int i = 0; i < mBevE.size() ; ++i)
      ins >> mBevE[i];

    ins >> temp;
    mKniL.resize(temp);
    for ( int i = 0; i < mKniL.size() ; ++i)
      ins >> mKniL[i];

    ins >> temp;
    mTrans.resize(temp);
    for ( int i = 0; i < mTrans.size() ; ++i)
      ins >> mTrans[i];

    ins >> temp;
    mScale.resize(temp);
    for ( int i = 0; i < mScale.size() ; ++i)
      ins >> mScale[i];


 }

#endif

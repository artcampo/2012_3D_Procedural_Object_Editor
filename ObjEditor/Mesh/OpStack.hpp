#ifndef _OP_STACK_HPP
#define _OP_STACK_HPP

#include    "App/dx_misc.hpp"
#include    "ObjEditor/ObjEditorSharedData.hpp"
#include    "ObjEditor/Mesh/OpInfo.hpp"

#define _MESH_QT_IO
#define _MESH_CPP_IO

#ifdef _MESH_QT_IO
  #include <QDataStream>
#endif

#ifdef _MESH_CPP_IO
  #include <iostream>
#endif



class OpStack
{
public:
    ~OpStack();
    OpStack();
    OpStack( const OpStack& rhs);

    void queueOperation ( const OpInfo::OpBevelV& op );
    void queueOperation ( const OpInfo::OpBevelE& op );
    void queueOperation ( const OpInfo::OpExtrude& op );
    void queueOperation ( const OpInfo::OpExtrudeInner& op );
    void queueOperation ( const OpInfo::OpExtrudeMatrix& op );
    void queueOperation ( const OpInfo::OpNormScale& op );
    void queueOperation ( const OpInfo::OpPrimCube& op);
    void queueOperation ( const OpInfo::OpTranslate& op);
    void queueOperation ( const OpInfo::OpScale& op);
    void queueOperation ( const OpInfo::OpKnifeLine& op);

#ifdef _MESH_QT_IO
    void  save( QDataStream& outs );
    void  load( QDataStream& ins );
#endif

#ifdef _MESH_CPP_IO
    void  save( std::ofstream& outs );
    void  load( std::fstream& ins );
#endif


public:
    void  prepareReconstruction();
    int   numOp () const;
    int   opId ( int i ) const;

    OpInfo::OpPrimCube        opPrimCube( int i );

    OpInfo::OpExtrude         opExt( int i );
    OpInfo::OpExtrudeInner    opExtI( int i );
    OpInfo::OpExtrudeMatrix   opExtM( int i );
    OpInfo::OpNormScale       opNorS( int i );

    OpInfo::OpBevelV          opBevV( int i );
    OpInfo::OpBevelE          opBevE( int i );

    OpInfo::OpKnifeLine       opKniL( int i );

    OpInfo::OpTranslate       opTrans( int i );
    OpInfo::OpScale           opScale( int i );

private:
    int               mNumOp;
    std::vector<int>  mOpId;

    std::vector<OpInfo::OpPrimCube>         mPrim;

    std::vector<OpInfo::OpExtrude>          mExt;
    std::vector<OpInfo::OpExtrudeInner>     mExtI;
    std::vector<OpInfo::OpExtrudeMatrix>    mExtM;
    std::vector<OpInfo::OpNormScale>        mNorS;

    std::vector<OpInfo::OpKnifeLine>        mKniL;    

    std::vector<OpInfo::OpBevelV>           mBevV;
    std::vector<OpInfo::OpBevelE>           mBevE;

    std::vector<OpInfo::OpTranslate>        mTrans;
    std::vector<OpInfo::OpScale>            mScale;


    //  temporal data
private:
    std::vector<int>  mIndexOp;
    int               mNumComponents;

private:
    void simplifyQueue();
    void simplifyNormalScale();

};


namespace OpStackHelpers{
bool  compareSelections( const std::vector<ObjEd::Selection>&s1, const std::vector<ObjEd::Selection>& s2 );
};

#endif // _OP_STACK_HPP

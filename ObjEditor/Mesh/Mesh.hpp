#ifndef _MESH_H
#define _MESH_H

#pragma once
#include <vector>
#include <list>


#include "ObjEditor/Mesh/MeshComponent.hpp"
#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"

#include "Geometry/Intersections.hpp"
#include "App/pointlistmodel.h"
#include "ObjEditor/ObjEditorSharedData.hpp"
#include "ObjEditor/Mesh/OpStack.hpp"
#include "ObjEditor/Mesh/OpInfo.hpp"
#include "ObjEditor/Mesh/OperationsInfo.hpp"
#include "ObjEditor/Mesh/OperationsOptions.hpp"
#include "ObjEditor/Mesh/Serialization/MeshSerial.hpp"

//io - qt
#define _MESH_QT_IO
#define _MESH_CPP_IO

#ifdef _MESH_QT_IO
  #include <QDataStream>
#endif




//passing text description
#include <sstream>
#include <iostream>

using namespace ObjEd;

class Mesh
{
public:
  ~Mesh();
  Mesh();  
  Mesh( const MeshSerial& rhs );
  void release();
  void init();
public:
  OpStack*    mStack;

  //  render
public:
  std::vector<EditableMeshModel*> convert( ID3D10Device* aDev,
                                           D3DXVECTOR3 aCamPos,
                                           ColourMapping* aColours );
  std::vector<EditableMeshModelProcTex*> convertProcTex( ID3D10Device* aDev,
                                                         D3DXVECTOR3 aCamPos,
                                                         ColourMapping* aColours );

  //  IO
public:

#ifdef _MESH_QT_IO
  void  save( QDataStream& outs );
  void  load( QDataStream& ins, QString& aPathApp );
#endif

#ifdef _MESH_CPP_IO
  void  save( std::ofstream & outs );
  void  load( std::fstream & ins );
#endif

private:
  void  reconstructFromStack();
  //  edition
public:
  void  newPrimitive( int aPrim );
  int   nComponents() const;
  int   storeComponentFlag();

public:
  void        setMode ( int aMode );
  Selection   checkEntity(D3DXVECTOR3 aRayB,
                          D3DXVECTOR3 aRayE,
                          float aDistThreshold,
                          D3DXMATRIX& aView,
                          D3DXVECTOR3 aCamPos,
                          D3DXVECTOR3& aPointInSegment,
                          bool atHalf,
                          int aWhichHalf,
                          D3DXVECTOR3& aPoint);

  void        unselectEntities  ();
  void        selectEntity      ( Selection aSel );
  void        selectAddEntity   ( Selection aSel );
  bool        selectRemoveEntity( Selection aSel );  
  bool        isEntitySelected  ( const ObjEd::Selection& aEntity );

  void        unselectObjects();

  bool        hasBasisOfSelection ( std::vector<ObjEd::Selection> aSel );
  std::vector<D3DXVECTOR3>  basisOfSelection( std::vector<ObjEd::Selection> aSel );

  std::vector<ObjEd::Selection> getCurrentSelection() const;

  D3DXVECTOR3 centerMassSelectedEntities( const int aEntityType );
  D3DXVECTOR3 centerMassSelectedVertices ();
  D3DXVECTOR3 centerMassSelectedEdges ();
  D3DXVECTOR3 centerMassSelectedFaces ();
  D3DXVECTOR3 centerMassSelectedObjects ();

  //operations
public:
  void  opTranslate( D3DXVECTOR3 aValue,
                     const std::vector<ObjEd::Selection>& aSel  );
  void  endOpTranslate ();
  void  opTranslate( const OpInfo::OpTranslate& aOp );

  void  opStartScale();
  void  opScale( D3DXVECTOR3 aValue,
                 const std::vector<ObjEd::Selection>& aSel );
  void  endOpScale ();
  void  opScale( const OpInfo::OpScale& aOp );

  //operations - extrudes
public:
  void  opStartExtrude( const std::vector<ObjEd::Selection>& aSel,
                        Options::ExtrudeOptions& aOptions );
  void  opExtrude ( float aValue );
  void  endOpExtrude();
  void  opExtrude ( const OpInfo::OpExtrude& aOp );

  void  opStartExtrudeInner( const std::vector<ObjEd::Selection>& aSel,
                             Options::ExtInnerOptions& aOptions  );
  void  opExtrudeInner ( float aValue );
  void  endOpExtrudeInner();
  void  opExtrudeInner ( const OpInfo::OpExtrudeInner& aOp );

  void  opStartExtrudeMatrix( const std::vector<ObjEd::Selection>& aSel,
                              Options::ExtMatrixOptions& aOptions  );
  void  opExtrudeMatrix ( float aValue );
  void  endOpExtrudeMatrix();
  void  opExtrudeMatrix ( const OpInfo::OpExtrudeMatrix& aOp );

  //operations - bevels
public:
  void  opStartBevelV( const std::vector<ObjEd::Selection>& aSel );
  void  opBevelV ( float aValue );
  void  endOpBevelV();
  void  opBevelV ( const OpInfo::OpBevelV& aOp );

  void  opStartBevelE( const std::vector<ObjEd::Selection>& aSel );
  void  opBevelE ( float aValue );
  void  endOpBevelE();
  void  opBevelE ( const OpInfo::OpBevelE& aOp );

  void  opStartNorScale( const std::vector<ObjEd::Selection>& aSel );
  void  opNorScale ( float aValue );
  void  endOpNorScale();
  void  opNorScale ( const OpInfo::OpNormScale& aOp );


  void  opKniveLine( ObjEd::KniveLineInfo &aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );
  void  opKnifeLine ( const OpInfo::OpKnifeLine& aOp  );
  void  endOpKniveLine();

  void  opPrimCube( int aMeshComponentId );
  void  endOpPrimCube( int aMeshComponentId );
  void  opPrimCube( const OpInfo::OpPrimCube& aOp );

  //  operations - textures
  bool  hasProcTex();
  void  initProcTex( );
  void  newProcTex();
  void  setProcTex( const std::vector<ProcTex>& aProcTex );
  std::vector<ProcTex> procTex () const;

  void  opTexAddToSelection ( const int aCompId, const int aFaceId, const int aTexId );


private:
  int  addComponent( MeshComponent* aComp );

private:

  int     mMode;
  //TODO: smart
  std::vector<MeshComponent*>   mMeshObject;  
  int               mSelectedComponent;
  ObjEd::Selection  mHighlightedComponent;
  std::vector<bool> mSelO;

  std::vector<ProcTex>   mTextures;

  //  temp data (should this go to editor? TODO COMPRESSION)
private:
  std::vector<ObjEd::Selection>     mTransSel;
  D3DXVECTOR3                       mTransValue;

  std::vector<ObjEd::Selection>     mScaleSel;
  ScaleInfo                         mScaleInfo;

  std::vector<ObjEd::Selection>     mBevelVSel;
  std::vector<ObjEd::BevelVInfo>    mBevelVInfo;

  std::vector<ObjEd::Selection>     mBevelESel;
  ObjEd::BevelEInfo                 mBevelEInfo;

  std::vector<ObjEd::Selection>     mExtrudeSel;
  std::vector<ObjEd::ExtrudeInfo>   mExtrudeInfo;
  Options::ExtrudeOptions           mExtrudeCurrentOptions;

  std::vector<ObjEd::Selection>         mExtrudeMatrixSel;
  std::vector<ObjEd::ExtrudeMatrixInfo> mExtrudeMatrixInfo;
  Options::ExtMatrixOptions           mExtMatrixCurrentOptions;

  std::vector<ObjEd::Selection>     mExtrudeInnerSel;
  std::vector<ObjEd::ExtrudeInnerInfo>   mExtrudeInnerInfo;
  Options::ExtInnerOptions           mExtInnerCurrentOptions;

  std::vector<ObjEd::Selection>     mNorScaleSel;
  std::vector<ObjEd::NorScaleInfo>  mNorScaleInfo;

  OpInfo::OpKnifeLine         mKnifeLineOp;


private:
  Selection   checkVertex (D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold,
                           D3DXVECTOR3& aPoint, D3DXVECTOR3 aCamPos );
  Selection   checkEdge   (D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold,
                           D3DXVECTOR3& aPointInSegment, bool atHalf, int aWhichHalf,
                           D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );
  Selection   checkFace   (D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );
  Selection   checkObject (D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );


  void    selectVertex  ( ObjEd::Selection aSel );
  void    selectEdge    ( ObjEd::Selection aSel );
  void    selectFace    ( ObjEd::Selection aSel );
  void    selectObject  ( int aObjId );
  void    unselectObject( int aObjId );
  std::vector<ObjEd::Selection> getCurrentComponentSelection() const;

  void    opTranslateVertex(D3DXVECTOR3 aValue ,
                            const std::vector<ObjEd::Selection>& aSel );
  void    opTranslateEdges(D3DXVECTOR3 aValue,
                           const std::vector<ObjEd::Selection>& aSel );
  void    opTranslateFaces(D3DXVECTOR3 aValue,
                           const std::vector<ObjEd::Selection>& aSel );
  void    opTranslateObject(D3DXVECTOR3 aValue,
                            const std::vector<ObjEd::Selection>& aSel );

  void    opScaleVertices(D3DXVECTOR3 aCenter,
                        D3DXVECTOR3 aValue,
                        const std::vector<ObjEd::Selection>& aSel);
  void    opScaleEdges(D3DXVECTOR3 aCenter,
                       D3DXVECTOR3 aValue,
                       const std::vector<ObjEd::Selection>& aSel);
  void    opScaleFaces(D3DXVECTOR3 aCenter,
                       D3DXVECTOR3 aValue,
                       const std::vector<ObjEd::Selection>& aSel);
  void    opScaleObjects(D3DXVECTOR3 aCenter,
                        D3DXVECTOR3 aValue,
                        const std::vector<ObjEd::Selection>& aSel);

  std::list<int>    verticesOfEdgeSelection( const std::vector<ObjEd::Selection>& aSel );
  std::list<int>    verticesOfFaceSelection( const std::vector<ObjEd::Selection>& aSel );

  /*
  std::list<int>    verticesOfEdgeSelection();
  std::list<int>    verticesOfFaceSelection();
*/

  std::vector<int>  verticesOfFaces(  const std::vector<Face>& aFaces );

public:
  std::vector<int> DEBUGlistEntity(ObjEd::Selection sel );
public:
  MeshInfo infoMeshText();
public:
  friend class HE_meshSerial;
  friend class MeshSerial;

};

#endif // _MESH_H

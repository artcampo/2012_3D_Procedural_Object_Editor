  #ifndef _MESH_COMPONENT_H
#define _MESH_COMPONENT_H

#include <list>
#include <algorithm>
#include <string.h>   //wcscpy_s


#include "ObjEditor/ObjEditorSharedData.hpp"

#include "ObjEditor/Mesh/vertex.hpp"
#include "ObjEditor/Mesh/Edge.hpp"
#include "ObjEditor/Mesh/Face.hpp"
#include "ObjEditor/Mesh/OperationsInfo.hpp"
#include "ObjEditor/Mesh/OperationsOptions.hpp"
#include "ObjEditor/Mesh/OpStack.hpp"
#include "ObjEditor/Mesh/OpInfo.hpp"

#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"

#include "MeshHalfEdge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "Geometry/SimpleGraph.hpp"
#include "Geometry/DxMathHelpers.hpp"

#include "Shader/ShaderHelpers.hpp"
#include "ObjEditor/Mesh/Serialization/MeshComponentSerial.hpp"

using namespace ObjEd;

struct FacesOfEdge{
  int fl;
  int fr;
};



class MeshComponent
{
  struct  BevelEIntermidiateSharedNEdge{
    HE_edge*  bevelEdge;
    HE_edge*  expandedEdge;
    HE_face*  face;
    bool      hasFirst, hasLast;
    HE_edge*  first;
    HE_edge*  last;
    HE_edge*  nextOffirst;
    HE_edge*  prevOflast;
    HE_edge*  top;
    HE_edge*  bottom;
  };

  struct  BevelESharedNEdgeGroup{
    int   numEdges;
    int   index;  //into infoSharedN vector
  };

public:
  ~MeshComponent();
  MeshComponent( );  
  MeshComponent( int id );  //TODO: why?
  MeshComponent( const MeshComponentSerial& rhs );

#ifdef _MESH_QT_IO
  void  saveTexture   ( QDataStream &outs );
  void  loadTexture   ( QDataStream &ins );
  void  saveSelection ( QDataStream &outs );
  void  loadSelection ( QDataStream &ins );
#endif

#ifdef _MESH_CPP_IO
  void saveTexture(std::ofstream &outs);
  void loadTexture(std::fstream &ins);
#endif

public:
  void setId ( int id );
  Intersections::BoundingSphere boundingSphere() const;

  //geometry specific
public:
  int   nVertices() const;
  int   nEdges() const;
  int   nFaces() const;

  VertexObj   vertex  ( int aId ) const;
  Edge        edge    ( int aId ) const;
  Face        face    ( int aId ) const;

  FacesOfEdge facesOfEdge ( int aId ) const;
  bool        faceContainsSelectedEntity ( const int aId, const ObjEd::Selection& aSel ) const;
  HE_edge*    halfEdgeOfFace ( int aFaceId, int aEdgeId );
  std::vector<int>   edgesOfFacesFacingCamera ( D3DXVECTOR3 aCamPos );
  std::vector<Face>  facesFacingCamera ( D3DXVECTOR3 aCamPos );
  std::vector<int>   facesOfEntity( const ObjEd::Selection& aSel ) const;
  std::vector<int>   facesOfEdgeList( const int aId ) const;

  void        modifyVertex ( const int aId, const VertexObj& v );  

  D3DXVECTOR3   center ( Face& aF ) const;
  std::vector<D3DXVECTOR3>  basisOfEdge ( const int edgeId ) const;
  std::vector<D3DXVECTOR3>  basisOfFace ( const int faceId ) const;

  //  Render
public:
  EditableMeshModel* convert( ID3D10Device* aDev, D3DXVECTOR3 aCamPos , ColourMapping* aColours );
  EditableMeshModelProcTex* convertProcTex( ID3D10Device* aDev,
                                            D3DXVECTOR3 aCamPos,
                                            ColourMapping* aColours,
                                            int aNumTextures,
                                            std::vector<ProcTex>& aTextures);

  //  Modes and selection
public:
  void setMode(int aMode);
  bool isEntitySelected ( int aEntityId );
  bool isEntitySelected ( const ObjEd::Selection &aEntity );
  std::vector<ObjEd::Selection> getCurrentSelection() const;

  //  Mode V
  void  unselectVertices    ();
  void  selectVertex        ( int aVertexId );
  bool  selectRemoveVertex  ( int aVertexId );

  //  Mode E
  void  unselectEdges       ();
  void  selectEdge          ( int aId );
  bool  selectRemoveEdge    (int aId);

  //  Mode F
  void  unselectFaces       ();
  void  selectFace          ( int aId );
  bool  selectRemoveFace    (int aId);




  //  others: these are under vigilance for delete TODO
  D3DXVECTOR3 centerMassSelectedVertices ();
  D3DXVECTOR3 centerMassSelectedEdges ();
  D3DXVECTOR3 centerMassSelectedFaces ();

  //  Edition
public:
  //  remove this! opTranslateSelectedVertices  
  void    opTranslateVertex(const int aId, D3DXVECTOR3 aValue);
  void    opScaleVertices(const int aId, D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue);

  ExtrudeInfo extrudeStart ( int aId, const Options::ExtrudeOptions& aOptions );
  void        extrude      ( int aId, ExtrudeInfo& aExt  );


  ExtrudeInnerInfo extrudeInnerStart ( int aId, const Options::ExtInnerOptions& aOptions );
  void        extrudeInner      ( int aId, ExtrudeInnerInfo& aExt  );

  ExtrudeMatrixInfo extrudeMatrixStart ( int aId, const Options::ExtMatrixOptions& aOptions );
  void        extrudeMatrix      ( int aId, ExtrudeMatrixInfo& aExt  );

  BevelVInfo  bevelVStart ( int aId );
  void        bevelV      ( int aId, BevelVInfo& aExt  );

  BevelEInfo  bevelEStart ( std::vector<ObjEd::Selection>& aSel );
  void        bevelE      ( BevelEInfo& aExt );

  NorScaleInfo  norScaleStart ( int aId );
  void          norScale      ( int aId, NorScaleInfo& aExt  );

  //Todo: change selection for one without component id and no face record
  OpInfo::OpKnifeLine   kniveLine ( ObjEd::KniveLineInfo &aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );
  void                  kniveLine ( const OpInfo::OpKnifeLine& aOp );

  void    initTex ();
  void    setTex  ( const int aFaceId, const int aTexId );
private:
  void    resizeTexIdSelection();

private:
  std::vector<HE_vert*>   extrudeFace( int aId );

  void  resizeSelectionVectors();  
  bool  knifeEdgeNotInBorders ( const ObjEd::KniveLineInfo &aInfo, const int aEdgeId ) const;

  void  bevelESetPositions ( BevelEInfo &info, int bId, int eId );
  bool  faceVisible ( const Face& f, const D3DXVECTOR3 aCamPos );

  //  Debug only
private:
  void  checkIntegrity();
  void  triangulateFace( int faceId, std::vector<VertexPosNorCol>& triangles, ColourMapping* aColours );
  std::vector<int>  uniqueVerticesOfFaces ( std::vector<Face>& aFaces );

  //  Operations internal functions
private:
  BevelEIntermidiateSharedNEdge bevelESplitSingleBorder( const int aVId, const int aEId, BevelEInfo& aInfo );

  void  bevelESplitSharedBorderDeg2(  const int aVId, const std::vector<int>& aEdgesId,
                                      BevelEInfo& aInfo,
                                      std::vector<BevelEIntermidiateSharedNEdge>& aEdgeInfo );

  void  bevelESplitSharedBorderDegN(  const int aVId, const std::vector<int>& aEdgesId,
                                      BevelEInfo& aInfo,
                                      std::vector<BevelEIntermidiateSharedNEdge>& aEdgeInfo );

  void  bevelECreateFace( BevelEIntermidiateSharedNEdge& aSharedN1,
                          BevelEIntermidiateSharedNEdge& aSharedN2 );

  BevelEIntermidiateSharedNEdge& bevelEGetEdge
        ( std::vector<BevelEIntermidiateSharedNEdge> &aInfo1,
          std::vector<BevelEIntermidiateSharedNEdge> &aInfo2,
          std::vector<BevelEIntermidiateSharedNEdge> &aInfoN,
          HE_edge *e);

  int   bevelEGetIndex ( std::vector<BevelEIntermidiateSharedNEdge>& aInfo, HE_edge* e );

  void kniveLineCutOneFace   ( const OpInfo::OpKnifeLine& aOp );
  void kniveLineCutManyFaces ( const OpInfo::OpKnifeLine& aOp );
  bool knifeEdgeContainsFace ( const int aId,
                               const std::vector<int>& aFaces,
                               int& aOpposingFaceId ) const;



  //  DEBUG
public:
  std::vector<int> DEBUGlistEdge( ObjEd::Selection sel );
  std::vector<int> DEBUGlistFace( ObjEd::Selection sel );

  //  Primitives
public:
  void  createCubePrimitive();


private:
  HE_mesh*     mMesh;
  int          mComponentId;

private:  
  Selection   mHighlight;
  int         mMode;

  std::vector<bool> mSelV;
  std::vector<bool> mSelE;
  std::vector<bool> mSelF;

  std::vector<int>  mTextureIdPerFace;

private:
  friend class Mesh;
  friend class MeshComponentSerial;

};

#endif // _MESH_COMPONENT_H

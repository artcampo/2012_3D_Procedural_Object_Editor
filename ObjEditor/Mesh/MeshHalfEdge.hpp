#ifndef MESHHALFEDGE_HPP
#define MESHHALFEDGE_HPP

#include "vertex.hpp"
#include "Face.hpp"
#include "Edge.hpp"
#include <vector>
#include <algorithm>

class HE_edge;
class HE_vert;
class HE_face;
class HE_meshSerial;

class HE_mesh{
public:
  ~HE_mesh();
  HE_mesh(){};
  HE_mesh( const HE_meshSerial& rhs );

  void        newFullEdge( const HE_edge* e1, const HE_edge* e2 );
  void        recycleFullEdge ( const int aId, const HE_edge* e1, const HE_edge* e2 );

  HE_edge*    newHalfEdge( HE_vert* v, HE_face* e );
  HE_vert*    newVertex( const VertexObj& av );
  HE_face*    newFace( );

  VertexObj   vertex  ( int id ) const;
  Face        face    ( int id ) const;
  Edge        fullEdge( int id ) const;

  HE_face*    hface ( int id ) const;
  HE_vert*    hvert ( int id ) const;
  HE_edge*    hedge ( int id ) const;
  HE_edge*    hedgeIncidentToVertex ( int id, const HE_vert* v ) const;

  bool      faceContainsHalfEdge ( const HE_face* aFace, const HE_edge* aEdge ) const;

  int   nVertices() const;
  int   nFaces() const;
  int   nHalfEdges() const;
  int   nFullEdges() const;

  void  modifyVertex(const int aId, const VertexObj &v );


  //debug
  int  fullOfhalf( HE_edge *e );

private:
  std::vector<HE_edge*>  mHalfEdges;
  std::vector<HE_face*>  mFaces;
  std::vector<HE_vert*>  mVertices;

  std::vector<HE_edge*>  mHalfEdgeOfFullEdge;   //given an edge, one of its half-edges
  std::vector<int>       mFullEdgeofHalfEdge;   //given any half edge, its edge

  // for copy
  friend class Mesh;
  friend class MeshComponent;

  //  operation helpers
public:
  HE_vert*    splitEdge( HE_edge* aSplitEdge, const VertexObj& av );

  HE_face*    sharedFace( HE_vert* aVb, HE_vert* aVe ) const;
  HE_face*    sharedFace( HE_vert* aVb, HE_edge* aEe ) const;
  HE_face*    sharedFace( HE_edge* aEb, HE_vert* aVe ) const;
  HE_face*    sharedFace( HE_edge* aEb, HE_edge* aEe ) const;

  HE_vert*    splitFace( HE_face* aFace, HE_vert* aVb, HE_vert* aVe );

  //Bevel E
public:
  std::vector<HE_edge*> edgePairOfVerticesPair ( const HE_vert* aV1, const HE_vert* aV2 ) const;
  std::vector<int>  verticesOfEdgesOfCorner( const int vbId, const int veId ) const;
  std::vector<HE_vert*>  bevelSingleEdgeHIGH ( const int vbId, const int veId );
  HE_face* bevelEexteriorFace( const HE_vert* v1, const HE_vert* v2 ) const;
  HE_edge* bevelEnewHalfEdge( HE_vert* v );
  HE_edge* bevelEExteriorEdge( const HE_vert* v1, const HE_vert* v2 ) const;

  void bevelESingleCornerE(
      HE_vert*  v,
      HE_vert*  vprev,
      HE_vert** vc,
      HE_edge** el,
      HE_edge** er,
      HE_edge** eln,
      HE_edge** ern,
      D3DXVECTOR3* ein);
  void bevelESingleCornerB(
      HE_vert*  v,
      HE_vert*  vprev,
      HE_vert** vc,
      HE_edge** el,
      HE_edge** er,
      HE_edge** eln,
      HE_edge** ern,
      D3DXVECTOR3* ein);

  void bevelESharedCorner(
      HE_vert*  v,
      HE_edge*  e,
      HE_edge*  e2,
      HE_vert** vc,
      HE_edge** el,
      HE_edge** er,
      HE_edge** eln,
      HE_edge** ern );

  void bevelESingleEdge(
      HE_vert* v1,
      HE_vert* vc1,
      HE_edge* er,
      HE_edge* ern,

      HE_edge* e1,

      HE_vert* v2,
      HE_vert* vc2,
      HE_edge* el,
      HE_edge* eln
      );

public:   
  HE_vert* serialIdv (HE_vert* v) const;
  HE_face* serialIdf (HE_face* v) const;
  HE_edge* serialIde (HE_edge* v) const;

  HE_vert* unserialIdv (HE_vert* v) const;
  HE_face* unserialIdf (HE_face* v) const;
  HE_edge* unserialIde (HE_edge* v) const;

  friend class MeshComponent;
  friend class HE_meshSerial;

  //debug
public:
  void checkIntegrity();
};

#endif // MESHHALFEDGE_HPP

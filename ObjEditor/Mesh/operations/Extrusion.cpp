#include "OBjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

std::vector<HE_vert*> MeshComponent::extrudeFace ( int aId )
{
  HE_face* face               = mMesh->hface( aId );
  std::vector<HE_vert*> oldv  = face->vertices();
  std::vector<HE_edge*> e     = face->edges();

  int n = e.size();

  //std::vector<D3DXVECTOR3>  vertices ( n );
  std::vector<HE_vert*> v  ( n );
  std::vector<HE_edge*> ea ( n );
  std::vector<HE_edge*> eb ( n );
  std::vector<HE_edge*> ec ( n );
  std::vector<HE_edge*> ed ( n );
  std::vector<HE_edge*> ep ( n );
  std::vector<int>     eId ( n );
  std::vector<HE_face*> f  ( n );
  HE_face* finner = face;

  // Create vertices, faces
  for (int i = 0 ; i < n ; ++i )
  {
    v[i]    = mMesh->newVertex( e[i]->mVert->mVertex );
    f[i]    = mMesh->newFace();
    ep[i]   = e[i]->mPair;
    eId[i]  = mMesh->fullOfhalf( e[i] );
  }

  //  Create all the edges
  for (int i = 0 ; i < n ; ++i )
  {
    ea[i] = mMesh->newHalfEdge( v[i], f[i] );
    eb[i] = mMesh->newHalfEdge( v[ (i + n - 1) % n], f[i] );
    ec[i] = mMesh->newHalfEdge( oldv[ (i + n - 1) % n], f[i] );
    ed[i] = mMesh->newHalfEdge( oldv[i], f[i] );

    ed[i]->next( ea[i] );
    ea[i]->next( eb[i] );
    eb[i]->next( ec[i] );
    ec[i]->next( ed[i] );
  }

  // set pairs of exterior,
  // and Set pairs
  for (int i = 0 ; i < n ; ++i )
  {
    ep[i]->pair( ed[i] );

    ea[i]->pair( ec[(i + 1) % n] );
    eb[i]->pair( e[i] );

    e[i]->mVert = v[i];

    mMesh->newFullEdge( ea[i], ec[(i + 1) % n] );
    mMesh->newFullEdge( ed[i], ep[i] );

    //mMesh->newFullEdge( eb[i], en[i] );
    mMesh->recycleFullEdge( eId[i], e[i], eb[i]);
  }

  resizeSelectionVectors();

  return v;
}

//  \param  aId   id of face to extrude
//  \return info to extrude face
ExtrudeInfo MeshComponent::extrudeStart ( int aId, const Options::ExtrudeOptions& aOptions )
{
  ExtrudeInfo info;

  HE_face* face               = mMesh->hface( aId );
  std::vector<HE_vert*> oldv  = face->vertices();
  std::vector<HE_edge*> e = face->edges();

  int n = e.size();
  std::vector<D3DXVECTOR3>  vertices ( n );


  //   compute center
  info.center.resize(n);
  for (int i = 0 ; i < n ; ++i )
  {
    vertices[i] = e[i]->mVert->mVertex.pos;
    info.center[i] = vertices[i];
  }

  //  compute normal
  D3DXVec3Cross ( &info.normal, &( vertices[1] - vertices[0] ), &( vertices[2] - vertices[0] ));
  D3DXVec3Normalize ( &info.normal, &info.normal );

  info.newVertices.resize( aOptions.numParts );
  for ( int i = 0; i < aOptions.numParts; ++i)
  {
    info.newVertices[i] = extrudeFace( aId );
  }

  info.numParts = aOptions.numParts;
  info.value = 0.0f;

  return info;
}

void MeshComponent::extrude(int aId, ObjEd::ExtrudeInfo &aExt )
{
  int n = aExt.center.size();
  for (int j = 0 ; j < aExt.numParts ; ++j )
  {
    float partMult = (1.0f + float(j) ) / float(aExt.numParts);
    for (int i = 0 ; i < n ; ++i )
    {
      D3DXVECTOR3 pos = aExt.center[i] + aExt.normal*aExt.value*partMult;
      VertexObj v = aExt.newVertices[j][i]->mVertex;
      v.pos = pos;
      aExt.newVertices[j][i]->modifyVertex( v );
    }
  }

  checkIntegrity();
}

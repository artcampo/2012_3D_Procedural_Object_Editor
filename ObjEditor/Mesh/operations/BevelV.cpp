#include "OBjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

//  \param  aId   id of face to extrude
//  \return info to extrude face
BevelVInfo MeshComponent::bevelVStart ( int aId )
{
  BevelVInfo info;

  HE_vert* vInitial        = mMesh->hvert( aId );
  std::vector<HE_face*> f  = vInitial->faces();
  std::vector<HE_edge*> e  = vInitial->edgesClockWise();

  int n = e.size();

  std::vector<HE_vert*> v  ( n );
  std::vector<HE_edge*> eout ( n );
  std::vector<HE_edge*> ein ( n );
  HE_face* finner;

  info.direction.resize( n );

  info.center = vInitial->mVertex.pos;
  VertexObj vObj = vInitial->mVertex;

  //  Create vertices, and face
  v[0] = vInitial;
  for (int i = 1 ; i < n ; ++i )
  {
    v[i] = mMesh->newVertex( vObj );
  }
  finner = mMesh->newFace();

  //  Compute directions
  for (int i = 0 ; i < n ; ++i )
  {
    info.direction[i] = (e[i]->mPair->mVert->mVertex).pos - vObj.pos;
  }

  //  Create all the edges
  for (int i = 0 ; i < n ; ++i )
  {
    eout[i] = mMesh->newHalfEdge( v[(i + 1)%n], f[i] );
    ein[i]  = mMesh->newHalfEdge( v[i], finner );

    mMesh->newFullEdge( eout[i], ein[i] );
  }
  //  set next of inner/outer face, and Set pairs
  for (int i = 0 ; i < n ; ++i )
  {    
    e[i]->mVert = v[i];
    eout[i]->next( e[i]->mNext );
    e[i]->next( eout[i] );
    ein[i]->prev(  ein[(i + 1) % n] );
    ein[i]->pair( eout[i] );
  }

  resizeSelectionVectors();

  //  Set info to apply later extrusion
  info.previousValue = 0.0f;
  info.newVertices = v;

  return info;
}

void MeshComponent::bevelV(int aId, ObjEd::BevelVInfo &aExt )
{
  int n = aExt.newVertices.size();
  for (int i = 0 ; i < n ; ++i )
  {
    D3DXVECTOR3 increment = aExt.direction[i]*aExt.value - aExt.direction[i]*aExt.previousValue;
    VertexObj v = aExt.newVertices[i]->mVertex;
    v.pos += increment;
    aExt.newVertices[i]->modifyVertex( v );
  }

  aExt.previousValue = aExt.value;
  checkIntegrity();
}

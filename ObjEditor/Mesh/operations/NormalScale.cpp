#include "OBjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

//  \param  aId   id of face to extrude
//  \return info to extrude face
NorScaleInfo MeshComponent::norScaleStart ( int aId )
{
  NorScaleInfo info;

  HE_face* face               = mMesh->hface( aId );

  info.newVertices = face->vertices();
  int n = info.newVertices.size();
  info.initialVertex.resize( n );

  //  Create vertices, faces, compute center
  info.center = D3DXVECTOR3(0,0,0);
  for (int i = 0 ; i < n ; ++i )
  {
    info.initialVertex[i] = info.newVertices[i]->mVertex.pos;
    info.center += info.initialVertex[i];
  }
  info.center *= 1.0f/float( n );

  //  change to local frame
  for (int i = 0 ; i < n ; ++i )
    info.initialVertex[i] -= info.center;

  return info;
}

void MeshComponent::norScale(int aId, ObjEd::NorScaleInfo &aExt )
{
  int n = aExt.newVertices.size();

  for (int i = 0 ; i < n ; ++i )
  {    
    VertexObj v;
    v.pos = aExt.center + aExt.value*aExt.initialVertex[i];
    aExt.newVertices[i]->modifyVertex( v );
  }

  checkIntegrity();
}

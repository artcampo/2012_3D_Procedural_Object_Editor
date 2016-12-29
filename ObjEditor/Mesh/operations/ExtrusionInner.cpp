#include "OBjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

//  \param  aId   id of face to extrudeInner
//  \return info to extrudeInner face
ExtrudeInnerInfo MeshComponent::extrudeInnerStart ( int aId,
                                                    const Options::ExtInnerOptions& aOptions )
{
  ExtrudeInnerInfo info;

  HE_face* face               = mMesh->hface( aId );
  std::vector<HE_vert*> oldv  = face->vertices();
  std::vector<HE_edge*> e = face->edges();

  //  Set info to apply later extrusion

  int n = e.size();
  std::vector<D3DXVECTOR3>  vertices ( n );

  //   compute center
  info.center.resize(n);
  info.u.resize(n);
  info.v.resize(n);

  for (int i = 0 ; i < n ; ++i )
  {
    vertices[i] = e[i]->mVert->mVertex.pos;
    info.center[i] = vertices[i];
  }

  for (int i = 0 ; i < n ; ++i )
  {
    int uindex = (i + 1) % n;
    int vindex = (i + n - 1) % n;

    info.u[i] = vertices[uindex] - vertices[i];
    info.v[i] = vertices[vindex] - vertices[i];
  }

  info.newVertices.resize( aOptions.numParts );
  for ( int i = 0; i < aOptions.numParts; ++i)
  {
    info.newVertices[i] = extrudeFace( aId );
  }

  info.numParts = aOptions.numParts;
  info.value = 0.0f;

  resizeSelectionVectors();

  return info;

}

void MeshComponent::extrudeInner(int aId, ObjEd::ExtrudeInnerInfo &aExt )
{
  int n = aExt.center.size();
  for (int j = 0 ; j < aExt.numParts ; ++j )
  {
    float partMult = (1.0f + float(j) ) / float(aExt.numParts);
    for (int i = 0 ; i < n ; ++i )
    {
      D3DXVECTOR3 pos = aExt.center[i]
                      - aExt.u[i]*aExt.value*partMult
                      - aExt.v[i]*aExt.value*partMult;
      VertexObj v = aExt.newVertices[j][i]->mVertex;
      v.pos = pos;
      aExt.newVertices[j][i]->modifyVertex( v );
    }
  }

}

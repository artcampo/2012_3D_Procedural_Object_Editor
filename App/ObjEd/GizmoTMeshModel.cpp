#include "GizmoTMeshModel.hpp"

GizmoTMeshModel::~GizmoTMeshModel()
{
  delete mAxis;
}

GizmoTMeshModel::GizmoTMeshModel( std::vector<VertexPosCol>& mVertices,
                                  ID3D10Device* aMd3dDevice )
{
  DxMeshModel<VertexPosCol>* mesh;
  mesh  = new DxMeshModel<VertexPosCol>( sizeof(VertexPosCol), mVertices.size(), (VertexPosCol*) (&mVertices[0]) );
  mAxis = new DxMeshModelBatch<VertexPosCol>( sizeof(VertexPosCol), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
  mAxis->addMeshModel( mesh );
  mAxis->createVB( );
}

void GizmoTMeshModel::display()
{
  mAxis->DisplaySansShader();
}



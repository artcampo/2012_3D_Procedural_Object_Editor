#include "pointlistmodel.h"

PointListModel::PointListModel( std::vector<VertexPointPSC>& mVertices, ID3D10Device* aMd3dDevice  )
{
  DxMeshModel<VertexPointPSC>* mesh;
  mesh  = new DxMeshModel<VertexPointPSC>( sizeof(VertexPointPSC), mVertices.size(), (VertexPointPSC*) (&mVertices[0]) );
  mMesh = new DxMeshModelBatch<VertexPointPSC>( sizeof(VertexPointPSC), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
  mMesh->addMeshModel( mesh );
  mMesh->createVB( );
}


PointListModel::~PointListModel()
{
  delete mMesh;
}

void PointListModel::display()
{
  mMesh->DisplaySansShader();
}

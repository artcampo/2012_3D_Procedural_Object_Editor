#include "linelistmodel.h"

LineListModel::LineListModel( std::vector<D3DXVECTOR3>& mVertices, D3DXCOLOR& aColour, ID3D10Device* aMd3dDevice )
{
  DxMeshModel<VertexPos>* mesh;
  mesh  = new DxMeshModel<VertexPos>( sizeof(VertexPos), mVertices.size(), (VertexPos*) (&mVertices[0]) );
  mMesh = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
  mMesh->addMeshModel( mesh );
  mMesh->createVB( );
}

LineListModel::~LineListModel()
{
  delete mMesh;
}

void LineListModel::display()
{
  mMesh->DisplaySansShader();
}

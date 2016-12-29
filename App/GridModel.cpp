#include "GridModel.h"

/*
    vertices are line lists.
    mVerticesAxis should have two lines for the main axis of this plane
*/
GridModel::GridModel( std::vector<D3DXVECTOR3>& mVertices,
                      std::vector<D3DXVECTOR3>& mVerticesAxis,
                      D3DXCOLOR& aColour,
                      ID3D10Device* aMd3dDevice )
{
  mColourAxis = D3DXCOLOR( 0.2f, 0.2f, 0.3f, 1.0f );
  mColour = aColour;

  {
    DxMeshModel<VertexPos>* mesh;
    mesh = new DxMeshModel<VertexPos>( sizeof(VertexPos), mVertices.size(), (VertexPos*) (&mVertices[0]) );
    mMesh = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
    mMesh->addMeshModel( mesh );
    mMesh->createVB( );
  }
  {
    DxMeshModel<VertexPos>* mesh;
    mesh = new DxMeshModel<VertexPos>( sizeof(VertexPos), mVerticesAxis.size(), (VertexPos*) (&mVerticesAxis[0]) );
    mMeshAxis = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
    mMeshAxis->addMeshModel( mesh );
    mMeshAxis->createVB( );
  }
}

GridModel::~GridModel()
{
  delete mMesh;
  delete mMeshAxis;
}

void GridModel::display()
{
  mMesh->DisplaySansShader();
}

void GridModel::displayAxis()
{
  mMeshAxis->DisplaySansShader();
}

D3DXCOLOR & GridModel::getColour()
{
  return mColour;
}

D3DXCOLOR & GridModel::getColourAxis()
{
  return mColourAxis;
}


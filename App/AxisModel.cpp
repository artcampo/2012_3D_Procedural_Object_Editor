#include "AxisModel.h"

AxisModel::AxisModel( std::vector<D3DXVECTOR3>& mVertices,  ID3D10Device* aMd3dDevice )
{
  {
    DxMeshModel<VertexPos>* mesh;
    mesh = new DxMeshModel<VertexPos>( sizeof(VertexPos), 2, (VertexPos*) (&mVertices[0]) );
    mMeshX = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
    mMeshX->addMeshModel( mesh );
    mMeshX->createVB( );
  }

  {
    DxMeshModel<VertexPos>* mesh;
    mesh = new DxMeshModel<VertexPos>( sizeof(VertexPos), 2, (VertexPos*) (&mVertices[2]) );
    mMeshY = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
    mMeshY->addMeshModel( mesh );
    mMeshY->createVB( );
  }

  {
    DxMeshModel<VertexPos>* mesh;
    mesh = new DxMeshModel<VertexPos>( sizeof(VertexPos), 2, (VertexPos*) (&mVertices[4]) );
    mMeshZ = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST, aMd3dDevice );
    mMeshZ->addMeshModel( mesh );
    mMeshZ->createVB( );
  }
}

AxisModel::~AxisModel()
{
  delete mMeshX;
  delete mMeshY;
  delete mMeshZ;
}

void AxisModel::displayX()
{
  mMeshX->DisplaySansShader();
}

void AxisModel::displayY()
{
  mMeshY->DisplaySansShader();
}

void AxisModel::displayZ()
{
  mMeshZ->DisplaySansShader();
}

D3DXCOLOR AxisModel::getColourX()
{
  return D3DXCOLOR(1,0,0,1);
}

D3DXCOLOR AxisModel::getColourY()
{
  return D3DXCOLOR(0,1,0,1);
}

D3DXCOLOR AxisModel::getColourZ()
{
  return D3DXCOLOR(0,0,1,1);
}

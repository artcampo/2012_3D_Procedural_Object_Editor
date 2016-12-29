#include "imagequadmodel.hpp"

ImageQuadModel::ImageQuadModel( std::vector<D3DXVECTOR3>& aPoints, float aAlpha, ID3D10Device* aMd3dDevice )
{
  mAlpha = aAlpha;
  std::vector<VertexPosUV> vertices (6);

  vertices[0].pos = aPoints[0];
  vertices[0].tex = D3DXVECTOR2(0,1);
  vertices[1].pos = aPoints[1];
  vertices[1].tex = D3DXVECTOR2(1,1);
  vertices[2].pos = aPoints[2];
  vertices[2].tex = D3DXVECTOR2(1,0);

  vertices[3].pos = aPoints[0];
  vertices[3].tex = D3DXVECTOR2(0,1);
  vertices[4].pos = aPoints[2];
  vertices[4].tex = D3DXVECTOR2(1,0);
  vertices[5].pos = aPoints[3];
  vertices[5].tex = D3DXVECTOR2(0,0);

  DxMeshModel<VertexPosUV>* mesh;
  mesh  = new DxMeshModel<VertexPosUV>( sizeof(VertexPosUV), 6, &vertices[0] );
  mMesh = new DxMeshModelBatch<VertexPosUV>( sizeof(VertexPosUV), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
  mMesh->addMeshModel( mesh );
  mMesh->createVB( );
}

ImageQuadModel::~ImageQuadModel()
{
  delete mMesh;
}

void ImageQuadModel::display()
{
  mMesh->DisplaySansShader();
}

float ImageQuadModel::getAlpha()
{
  return mAlpha;
}

#include "EditableMeshModel.h"

EditableMeshModel::EditableMeshModel( std::vector<VertexPosNorCol>& aTrianglePoints,
                                      std::vector<VertexPosCol>& aEdgePoints,
                                      PointListModel* aVertices,
                                      bool aHasVertices,
                          D3DXCOLOR& aColour,
                          ID3D10Device* aMd3dDevice)
{

  DxMeshModel<VertexPosNorCol>* mesh;
  mesh  = new DxMeshModel<VertexPosNorCol>( sizeof(VertexPosNorCol), aTrianglePoints.size(), (VertexPosNorCol*) (&aTrianglePoints[0]) );
  mMeshTriangles = new DxMeshModelBatch<VertexPosNorCol>( sizeof(VertexPosNorCol), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
  mMeshTriangles->addMeshModel( mesh );
  mMeshTriangles->createVB( );

  DxMeshModel<VertexPosCol>* mesh2;
  mesh2  = new DxMeshModel<VertexPosCol>( sizeof(VertexPosCol), aEdgePoints.size(), (VertexPosCol*) (&aEdgePoints[0]) );
  mMeshEdges = new DxMeshModelBatch<VertexPosCol>( sizeof(VertexPosCol), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
  mMeshEdges->addMeshModel( mesh2 );
  mMeshEdges->createVB( );


  mHasVertices = aHasVertices;
  if (aHasVertices)
    mMeshVertices = aVertices;

}

EditableMeshModel::~EditableMeshModel()
{
  delete mMeshTriangles;
  delete mMeshEdges;
  if (mHasVertices)
    delete mMeshVertices;
}


void EditableMeshModel::displayTriangles()
{
  mMeshTriangles->DisplaySansShader();
}

void EditableMeshModel::displayEdges()
{
  mMeshEdges->DisplaySansShader();
}

void EditableMeshModel::displayVertices()
{
  if (mHasVertices)
    mMeshVertices->display();
}

bool EditableMeshModel::hasVertices()
{
  return mHasVertices;
}

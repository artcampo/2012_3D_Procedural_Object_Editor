#include "EditableMeshModelProcTex.h"

EditableMeshModelProcTex::EditableMeshModelProcTex( std::vector< std::vector<VertexPosNor> >& aTrianglePoints,
                                                    std::vector< Shader* > aShaders,
                                                    ID3D10Device* aMd3dDevice)
{

  int n = aTrianglePoints.size();
  mMeshTriangles.resize(n);
  mShaders.resize(n);

  for ( int i = 0; i < n ; ++i )
  {
    mShaders[i] = aShaders[i];

    DxMeshModel<VertexPosNor>* mesh;
    mesh  = new DxMeshModel<VertexPosNor>( sizeof(VertexPosNor), aTrianglePoints[i].size(), (VertexPosNor*) (&aTrianglePoints[i][0]) );
    mMeshTriangles[i] = new DxMeshModelBatch<VertexPosNor>( sizeof(VertexPosNor), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
    mMeshTriangles[i]->addMeshModel( mesh );
    mMeshTriangles[i]->createVB( );
  }

}

EditableMeshModelProcTex::~EditableMeshModelProcTex()
{
  for ( int i = 0; i < mMeshTriangles.size(); ++i)
    delete mMeshTriangles[i];
  for ( int i = 0; i < mShaders.size(); ++i)
    delete mShaders[i];

  mMeshTriangles.clear();
};


void EditableMeshModelProcTex::display( D3DXVECTOR3& camPos,
                                        D3DXMATRIX& mWorld,
                                        D3DXMATRIX& mView,
                                        D3DXMATRIX& mProjection,
                                        ID3D10Device* aMd3dDevice)
{
  //mMeshTriangles->DisplaySansShader();
  int n = mMeshTriangles.size();
  for ( int i = 0; i < n ; ++i )
  {
    ID3D10EffectPass* pass;
    mShaders[i]->SetRawValue( &(std::string("camPos")), (float *)&camPos ,3 );
    mShaders[i]->SetMatrix( &(std::string("World")), &mWorld );
    mShaders[i]->SetMatrix( &(std::string("View")), &mView );
    mShaders[i]->SetMatrix( &(std::string("Projection")), &mProjection );

    aMd3dDevice->IASetInputLayout( mShaders[i]->mVertexLayout );
    aMd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    pass = mShaders[i]->mTech->GetPassByIndex(0);
    pass->Apply(0);

    mMeshTriangles[i]->DisplaySansShader();
  }

}

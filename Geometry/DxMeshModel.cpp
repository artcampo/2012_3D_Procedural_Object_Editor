//#include		"Geometry/DxMeshModel.hpp"


///////////////////////////////////////////////////////////////////////////////
//		MeshModel
///////////////////////////////////////////////////////////////////////////////

template <class VertexLayout>
DxMeshModel<VertexLayout>::DxMeshModel ( int aVertexLayoutSize, int aNumVertices, 
                           VertexLayout* aVertices  )
{
  mNumVertices        = aNumVertices;
  mVertexLayoutSize   = aVertexLayoutSize;
  // * vertexLayoutSize);
  mVertices     = std::vector<VertexLayout> (aNumVertices);
  for ( int i = 0; i < (aNumVertices) ; ++i ) {
    mVertices[i] = *(aVertices + i) ;
  }
}

///////////////////////////////////////////////////////////////////////////////
//	  DxMeshModelBatch
//
//  A collection of DxMeshModel, ready to be rendered.
///////////////////////////////////////////////////////////////////////////////

template <class VertexLayout>
DxMeshModelBatch<VertexLayout>::DxMeshModelBatch ( int aVertexLayoutSize,   //TODO: esto sobra, o la clase tiene que cambiar
                                     D3D10_PRIMITIVE_TOPOLOGY aTopology,
                                     ID3D10Device* aMd3dDevice):
                                     mTopology(aTopology),
                                     mNumVertices(0),
                                     md3dDevice(aMd3dDevice)
                                    
{
  mVertexLayoutSize = aVertexLayoutSize;
}

template <class VertexLayout>
void DxMeshModelBatch<VertexLayout>::addMeshModel ( DxMeshModel<VertexLayout>* aMesh )
{  
  mNumVertices += aMesh->mNumVertices;
  unsigned int size = mVertices.size();
  mVertices.resize ( size + aMesh->mVertices.size() );
  for (unsigned int i = 0; i < aMesh->mVertices.size(); ++i)
    mVertices[size+i] = aMesh->mVertices[i];
}

//vertexLayout siempre del mi smo tamaño?

template <class VertexLayout>
bool DxMeshModelBatch<VertexLayout>::createVB   	( )
{  
  //Vertex buffer
  D3D10_BUFFER_DESC bd;
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = mVertexLayoutSize * mNumVertices;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;  
  D3D10_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = &mVertices[0];
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  HRESULT hr; //Used to check everything went ok (Windows only)  
  hr = md3dDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
  if( FAILED( hr ) ) 
      return false;  
 
  return true;
}

template <class VertexLayout>
void DxMeshModelBatch<VertexLayout>::DisplaySansShader		  ( )
{
  //Global::md3dDevice->IASetPrimitiveTopology(mTopology);
  UINT stride = mVertexLayoutSize;
  UINT offset = 0;
  md3dDevice->
    IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );  
  md3dDevice->Draw(mNumVertices, 0);
}

template <class VertexLayout>
DxMeshModelBatch<VertexLayout>::~DxMeshModelBatch()
{  
  ReleaseCOM( mVertexBuffer );
  mVertices.clear();
}

#include		"DxMeshModelIndexed.hpp"

///////////////////////////////////////////////////////////////////////////////
//		MeshModel
///////////////////////////////////////////////////////////////////////////////

DxMeshModelIndexed::DxMeshModelIndexed ( int aVertexLayoutSize, int aNumVertices,
                          VertexTextEd* aVertices  ):
                          mNumVertices(aNumVertices),
                          mVertexLayoutSize(aVertexLayoutSize),
                          mIsSetIndices(false)

{
  mVertices     = std::vector<VertexTextEd> (aNumVertices);
  for ( int i = 0; i < (aNumVertices) ; ++i ) {
    mVertices[i] = aVertices[i];
  }
}

DxMeshModelIndexed::~DxMeshModelIndexed ()
{
}

void DxMeshModelIndexed::setIndices (DWORD* aIndices, DWORD aNumFaces)
{
  mIndices      = std::vector<DWORD>(aNumFaces*3);
  mNumTriangles = aNumFaces;
  mIsSetIndices = true;
  for(DWORD i = 0; i < aNumFaces*3; ++i)
    mIndices[i] = aIndices[i];
}

bool DxMeshModelIndexed::isSetIndices ()
{
  return mIsSetIndices;
}

///////////////////////////////////////////////////////////////////////////////
//	  DxMeshModelIndexedBatch
//
//  A collection of DxMeshModelIndexed, ready to be rendered.
///////////////////////////////////////////////////////////////////////////////

DxMeshModelIndexedBatch::DxMeshModelIndexedBatch ( int aVertexLayoutSize,
                                     D3D10_PRIMITIVE_TOPOLOGY aTopology,
                                     ID3D10Device*           ad3dDevice):
                                     mTopology(aTopology),
                                     mNumVertices(0),
                                     mIndices(NULL),
                                     mVertexBuffer(NULL),
                                     mIndexBuffer(NULL),
                                     mNumObjects(0),
                                     mNumIndices(0),
                                     md3dDevice(ad3dDevice)
                                    
{
  mVertexLayoutSize = aVertexLayoutSize;
}

DxMeshModelIndexedBatch::~DxMeshModelIndexedBatch()
{
}

void DxMeshModelIndexedBatch::addMeshModel  ( DxMeshModelIndexed* aMesh )
{  
  mNumVertices += aMesh->mNumVertices;  
  unsigned int sizeVB = mVertices.size();
  unsigned int sizeIB = mIndices.size();
  mVertices.resize ( sizeVB + aMesh->mVertices.size());
  mIndices.resize  ( sizeIB + aMesh->mIndices.size() );
  for (unsigned int i = 0; i < aMesh->mVertices.size(); ++i){
    mVertices[sizeVB+i] = aMesh->mVertices[i];
  }
  if (aMesh->isSetIndices()){
    mNumIndices  += aMesh->mNumTriangles * 3;
    for (unsigned int i = 0; i < aMesh->mIndices.size(); ++i){
      mIndices[sizeIB+i] = aMesh->mIndices[i] + mNumVertices * mNumObjects;
    }
    ++mNumObjects;
  }
}

bool DxMeshModelIndexedBatch::createVB   	( )
{  
  D3D10_BUFFER_DESC bd;
  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = mVertexLayoutSize * mNumVertices;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;  
  D3D10_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = (VertexTextEd*)&mVertices[0];
  HRESULT hr; //Used to check everything went ok (Windows only)  
  hr = md3dDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
  if( FAILED( hr ) ) 
      return false;  

  return true;
}

bool DxMeshModelIndexedBatch::createIB  	( )
{  
  D3D10_BUFFER_DESC ibd;
  ibd.Usage = D3D10_USAGE_IMMUTABLE;
  ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
  ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = (DWORD*)&mIndices[0];
  HRESULT hr; //Used to check everything went ok (Windows only)  
  hr = md3dDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer);

  if (FAILED(hr))
    return false;
 
  return true;
}

void DxMeshModelIndexedBatch::display		  (Shader* afxBox)
{  
  //Word View Projection Matrix

  md3dDevice->IASetInputLayout(afxBox->mVertexLayout);
  md3dDevice->IASetPrimitiveTopology(mTopology);
  D3D10_TECHNIQUE_DESC techDesc;
    afxBox->mTech->GetDesc( &techDesc );
  UINT stride = mVertexLayoutSize;
  UINT offset = 0;
  for(UINT p = 0; p < techDesc.Passes; ++p)
  {    
    afxBox->mTech->GetPassByIndex( p )->Apply(0);
    md3dDevice->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    md3dDevice->DrawIndexed(mNumIndices, 0, 0 );
    //md3dDevice->Draw(mNumVertices, 0);
  }
}

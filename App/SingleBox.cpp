//=======================================================================================
// SingleBox.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "SingleBox.hpp"

SingleBox::SingleBox(ID3D10Device*           ad3dDevice):
  mNumVertices(24),
  mNumFaces(12), //2 per quad 
  mVB(0), 
  mIB(0),
  md3dDevice(ad3dDevice)
{  
  mVertices = new VertexTextEd[mNumVertices];
  mIndices  = new DWORD[mNumFaces*3];
}
 
SingleBox::~SingleBox()
{  	
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);	
  delete mVertices;
  delete mIndices;
}

void SingleBox::init ( const float aScaleX, const float aScaleY, const float aScaleZ )
{
  VertexTextEd v[24];

  // Fill in the front face NormalVertex data.
  v[0] = VertexTextEd(-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f );
  v[1] = VertexTextEd(-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f );
  v[2] = VertexTextEd( 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f );
  v[3] = VertexTextEd( 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f );

// Fill in the back face NormalVertex data.
v[4] = VertexTextEd(-1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f );
v[5] = VertexTextEd( 1.0f, -1.0f, 1.0f,     0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f );
v[6] = VertexTextEd( 1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f );
v[7] = VertexTextEd(-1.0f,  1.0f, 1.0f,     0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f );

// Fill in the top face NormalVertex data.
v[8]  = VertexTextEd(-1.0f, 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f );
v[9]  = VertexTextEd(-1.0f, 1.0f,  1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f );
v[10] = VertexTextEd( 1.0f, 1.0f,  1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f );
v[11] = VertexTextEd( 1.0f, 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f );

// Fill in the bottom face NormalVertex data.
v[12] = VertexTextEd(-1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f );
v[13] = VertexTextEd( 1.0f, -1.0f, -1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f );
v[14] = VertexTextEd( 1.0f, -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f );
v[15] = VertexTextEd(-1.0f, -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f );

// Fill in the left face NormalVertex data.
v[16] = VertexTextEd(-1.0f, -1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f );
v[17] = VertexTextEd(-1.0f,  1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f );
v[18] = VertexTextEd(-1.0f,  1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f );
v[19] = VertexTextEd(-1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f );

// Fill in the right face NormalVertex data.
v[20] = VertexTextEd( 1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f );
v[21] = VertexTextEd( 1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f );
v[22] = VertexTextEd( 1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f );
v[23] = VertexTextEd( 1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f );

  float scale = 1.0f;
  //Scale da box
  for(DWORD i = 0; i < mNumVertices; ++i)
  {
    v[i].pos.x *= aScaleX;
    v[i].pos.y *= aScaleY;
    v[i].pos.z *= aScaleZ;
  }

  D3D10_BUFFER_DESC vbd;
  vbd.Usage = D3D10_USAGE_IMMUTABLE;
  vbd.ByteWidth = sizeof(VertexTextEd) * mNumVertices;
  vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA vinitData;
  vinitData.pSysMem = v;
  HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 5, 6,
		4, 6, 7,

		// top face
		8, 9, 10,
		8, 10, 11,

		// bottom face
		12, 13, 14,
		12, 14, 15,

		// left face
		16, 17, 18,
		16, 18, 19,

		// right face
		20, 21, 22,
		20, 22, 23,
	};

	D3D10_BUFFER_DESC ibd;
  ibd.Usage = D3D10_USAGE_IMMUTABLE;
  ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
  ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA iinitData;
  iinitData.pSysMem = indices;
  HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));

  for (DWORD i = 0; i < mNumVertices; ++i){    
    mVertices[i] = v[i];
  }

  for (DWORD i = 0; i < mNumFaces*3; ++i)
    mIndices[i] = indices[i];
}

ID3D10Buffer* SingleBox::getIndexBuffer()
{
  return mIB;
}

ID3D10Buffer* SingleBox::getVertexBuffer()
{
  return mVB;
}

DWORD SingleBox::getNumFaces()
{
  return mNumFaces;
}

DWORD SingleBox::getNumVertices()
{
  return mNumVertices;
}

VertexTextEd* SingleBox::getVertices()
{
  return mVertices;
}

DWORD* SingleBox::getIndices()
{
  return mIndices;
}

void SingleBox::display()
{
  ID3D10Buffer* mVB;
  ID3D10Buffer* mIB;

  mIB = getIndexBuffer();
  UINT stride = sizeof(VertexTextEd);
  UINT offset = 0;
  md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

  mVB = getVertexBuffer();
  md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);      
  md3dDevice->DrawIndexed(getNumFaces()*3, 0, 0);     	
}

#include	"Shader.hpp"
#include	"App/dx_misc.hpp"

#include   <stdio.h>

#include <QMessageBox>
#include <QString>

//===========================================================================
/*!
    Destructor: releases resources.
*/
//===========================================================================

Shader::~Shader ()
{
  Release();
}

//===========================================================================
/*!
    Initialize a Shader from its stored form

    \param      aData  All data describing this shader.
*/
//===========================================================================
void Shader::Init ( ShaderClassData *aData, ID3D10Device* ad3dDevice )
{

  mId                 = aData->mId;
  mFileName           = aData->mFileName;
  mIndexVertexLayout  = aData->mIndexVertexLayout;

  mTechName           = aData->mTechName;

  mNumMatrices        = aData->mNumMatrices;
  mNumRawValues       = aData->mNumRawValues;
  mNumResources       = aData->mNumResources;

  for ( int i = 0; i < mNumMatrices ; ++i)
  {
    mMatrices[i].mMatrixName  = aData->mMatrices[i];
    mMatrices[i].mMatrixVar   = NULL;
  }

  for ( int i = 0; i < mNumRawValues ; ++i)
  {
    mRawValues[i].mRawValueName  = aData->mRawValueName[i];
    mRawValues[i].mRawValueVar   = NULL;
  }

  for ( int i = 0; i < mNumResources ; ++i)
  {
    mResources[i].mResourceName  = aData->mResourceName[i];
    mResources[i].mResourceVar   = NULL;
  }

  md3dDevice = ad3dDevice;
  //  Nullify all pointers
  mVertexLayout = NULL;
  mEffect       = NULL;
  mTech         = NULL;
}

//===========================================================================
/*!
    Initialize a Shader

    \return     bool true <=> Shader compiled succesfully
*/
//===========================================================================
bool Shader::Compile ( )
{

  DWORD dwShaderFlags = 0;
  dwShaderFlags |= D3D10_SHADER_SKIP_VALIDATION;
  dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
  dwShaderFlags |= D3D10_SHADER_NO_PRESHADER;
  dwShaderFlags |= D3D10_SHADER_PREFER_FLOW_CONTROL;

  dwShaderFlags |= D3D10_SHADER_DEBUG;
/*
  #if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3D10_SHADER_DEBUG;
  #endif
*/

  ID3D10Blob* compilationErrors = 0;
  HRESULT hr;

  hr = D3DX10CreateEffectFromFile( mFileName, 0, 0, 
    "fx_4_0", dwShaderFlags, 0, md3dDevice, 0, 0, &mEffect, &compilationErrors, 0);


  if(FAILED(hr))
  {
	  if( compilationErrors )
		{
			QMessageBox::warning (
				0,
				QString("fx error"),
				QString((char*)compilationErrors->GetBufferPointer()));
		  ReleaseCOM(compilationErrors);
		}
    return false;
  }

  mTech = mEffect->GetTechniqueByName( mTechName.c_str() );


  for ( int i = 0; i < mNumMatrices ; ++i)
  {
    mMatrices[i].mMatrixVar  = mEffect->GetVariableByName( mMatrices[i].mMatrixName.c_str() )->AsMatrix();
  }


  for ( int i = 0; i < mNumRawValues ; ++i)
  {
    mRawValues[i].mRawValueVar = mEffect->GetVariableByName( mRawValues[i].mRawValueName.c_str() );
  }

  for ( int i = 0; i < mNumResources ; ++i)
  {
    mResources[i].mResourceVar = mEffect->GetVariableByName( mResources[i].mResourceName.c_str() )->AsShaderResource();
  }

	// Create the input layout.
	D3D10_PASS_DESC PassDesc;
	mTech->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	HR(md3dDevice->CreateInputLayout( 
                        shaderVertexLayouts[mIndexVertexLayout].mVertexLayout, 
                        shaderVertexLayouts[mIndexVertexLayout].mNumElements, 
                        PassDesc.pIAInputSignature,
                        PassDesc.IAInputSignatureSize, &mVertexLayout )) ;



  return true;
}


//===========================================================================
/*!
    Sets a resource

    \param      aName  Its name
    \param      aDesc  Its descriptor
*/
//===========================================================================
void Shader::SetResource ( std::string* aName, ID3D10ShaderResourceView* aDesc )
{
  bool found = false;

  for ( int i = 0; (i < mNumResources) && !found ; ++i)
  {
    if ( mResources[i].mResourceName == *aName )
    {
      found = true;
      mResources[i].mResourceVar->SetResource( aDesc );
    }
  }

  if ( !found )
  {
    char s[1024];
    sprintf_s(s, 1024, "Resource not found: %s",aName->c_str());
    //MessageBoxA(0, s, 0, 0);
    //PostQuitMessage(0);
  }
}

//===========================================================================
/*!
    Sets a matrix

    \param      aName  Its name
    \param      aMatrix  Matrix data (dx float4x4)
*/
//===========================================================================
void Shader::SetMatrix   ( std::string* aName, D3DXMATRIX* aMatrix )
{
  bool found = false;

  for ( int i = 0; (i < mNumMatrices) && !found ; ++i)
  {
    if ( mMatrices[i].mMatrixName == *aName )
    {
      found = true;
      mMatrices[i].mMatrixVar->SetMatrix ((float*)aMatrix);
    }
  }

  if ( !found )
  {
    char s[1024];
    sprintf_s(s, 1024, "Matrix not found: %s",aName->c_str());
    //MessageBoxA(0, s, 0, 0);
    //PostQuitMessage(0);
  }
}

//===========================================================================
/*!
    Sets Raw values, composed of floats

    \param      aName  Its name
    \param      aData  Pointer to consecutive float values
    \param      aNum  number of floats to send
*/
//===========================================================================

void Shader::SetRawValue   ( std::string* aName, float* aData, int aNum )
{
  bool found = false;

  for ( int i = 0; (i < mNumRawValues) && !found ; ++i)
  {
    if ( mRawValues[i].mRawValueName == *aName )
    {
      found = true; 
      mRawValues[i].mRawValueVar->SetRawValue (aData, 0, sizeof(float)*aNum );
    }
  }

  if ( !found )
  {
    char s[1024];
    sprintf_s(s, 1024, "Raw values not found: %s",aName->c_str());
    //MessageBoxA(0, s, 0, 0);
    //PostQuitMessage(0);
  }
}

void Shader::Release()
{  
  ReleaseCOM(mVertexLayout);
  ReleaseCOM(mEffect);
}


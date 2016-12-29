#include "RenderViewApp.hpp"

RenderViewApp::RenderViewApp(HWND screen, UINT width, UINT height):
    BaseDX(screen, width, height, false, L""),
    mShaderLines(NULL)

{
  mBackTexture = NULL;
  mBackTextureRV = NULL;
  mHasBackModel = NULL;
}

RenderViewApp::~RenderViewApp()
{
  delete   mShaderLines;

}

bool RenderViewApp::init()
{
  if (!BaseDX::init())
    return false;
  else {

    mShaderLines        = new Shader();
    mShaderLines->Init(gStoredShaders[shadFwdGeomP], md3dDevice);
    mShaderPoints       = new Shader();
    mShaderPoints->Init(gStoredShaders[shadFwdPointPSC], md3dDevice);
    mShaderQuadImage    = new Shader();
    mShaderQuadImage->Init(gStoredShaders[shadFwdTrianglesPUV], md3dDevice);


    if (!mShaderLines->Compile())
      return false;
    if (!mShaderPoints->Compile())
      return false;
    if (!mShaderQuadImage->Compile())
      return false;


  }

  return true;
}


void RenderViewApp::drawScene (
  D3DXVECTOR3& camPos,
  D3DXVECTOR3& camTar,
  D3DXVECTOR3& camUp,
  GridModel* aGrid,
  AxisModel* aAxis,
  PairTangentsModel* aPairTangentsModel,
  std::wstring& aTitle,
  bool aMuteBackground,
  float aScaleGrid )
{
  calculateTime();


  cam->setPositionAndAt( &camPos, &camTar );
  cam->updateCamera( &mView );
  cam->setUpVector(camUp );
  BaseDX::drawScene();

  /////////////////////////////////////////////////////
  //    Render LINES - STRIP
  mShaderLines->SetMatrix( &(std::string("World")), &mWorld );
  mShaderLines->SetMatrix( &(std::string("View")), &mView );
  mShaderLines->SetMatrix( &(std::string("Projection")), &mProjection );


  md3dDevice->IASetInputLayout( mShaderLines->mVertexLayout );
  //md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );

  ID3D10EffectPass* pass = mShaderLines->mTech->GetPassByIndex(0);
  pass->Apply(0);


  for( unsigned int i = 0 ; i < mBezModels.size() ;++i )
    if (mBezModels[i] != NULL)
    {
      mShaderLines->SetRawValue( &(std::string("colour")), mBezModels[i]->getColour(), 4 );
      pass->Apply(0);
      mBezModels[i]->displayLines();
    }

  if (aPairTangentsModel != 0 )
  {
    mShaderLines->SetRawValue( &(std::string("colour")), aPairTangentsModel->getColour(), 4 );
    pass->Apply(0);
    aPairTangentsModel->displayLines();
  }

  /////////////////////////////////////////////////////
  //    Render LINES - LIST
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
  if ( aGrid != 0)
  {
    mShaderLines->SetRawValue( &(std::string("colour")), aGrid->getColour(), 4 );
    pass->Apply(0);
    aGrid->display();

    mShaderLines->SetRawValue( &(std::string("colour")), aGrid->getColourAxis(), 4 );
    pass->Apply(0);
    aGrid->displayAxis();

  }

  if ( aAxis != 0)
  {
    mShaderLines->SetRawValue( &(std::string("colour")), aAxis->getColourX(), 4 );
    pass->Apply(0);
    aAxis->displayX();
    mShaderLines->SetRawValue( &(std::string("colour")), aAxis->getColourY(), 4 );
    pass->Apply(0);
    aAxis->displayY();
    mShaderLines->SetRawValue( &(std::string("colour")), aAxis->getColourZ(), 4 );
    pass->Apply(0);
    aAxis->displayZ();
  }



  /////////////////////////////////////////////////////
  //    Render POINTS

  D3DXMATRIX m = mView * mProjection;
  mShaderPoints->SetMatrix( &(std::string("gViewProj")), &m );
  mShaderPoints->SetMatrix( &(std::string("View")), &mView );
  D3DXMatrixInverse( &m, NULL,&mView);
  mShaderPoints->SetMatrix( &(std::string("invView")), &m );
  mShaderPoints->SetRawValue( &(std::string("gEyePosW")), (float *)&(camPos), 3);
  D3DXVECTOR3 up = cam->getUpVector();
  mShaderPoints->SetRawValue( &(std::string("camUp")), (float *)(&up), 3);
  m = mView * mProjection;
  D3DXMatrixInverse( &m, NULL,&m);
  mShaderPoints->SetMatrix( &(std::string("gInvViewProj")), &m );
  m = mProjection;
  D3DXMatrixInverse( &m, NULL,&m);
  mShaderPoints->SetMatrix( &(std::string("invProj")), &m );

  md3dDevice->IASetInputLayout( mShaderPoints->mVertexLayout );
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
  pass = mShaderPoints->mTech->GetPassByIndex(0);
  pass->Apply(0);

  for( unsigned int i = 0 ; i < mBezModels.size() ;++i )
    if (mBezModels[i] != NULL)
    {
      mBezModels[i]->displayPoints();
    }

  if (aPairTangentsModel != 0 )
  {
    aPairTangentsModel->displayPoints();
  }

  /////////////////////////////////////////////////////
  //    Render Background image
  if (mHasBackModel && !aMuteBackground)
  {
    mShaderQuadImage->SetMatrix( &(std::string("World")), &mWorld );
    mShaderQuadImage->SetMatrix( &(std::string("View")), &mView );
    mShaderQuadImage->SetMatrix( &(std::string("Projection")), &mProjection );

    md3dDevice->IASetInputLayout( mShaderQuadImage->mVertexLayout );
    md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    float alpha = mBackModel->getAlpha();
    mShaderQuadImage->SetRawValue( &(std::string("imageAlpha")), &alpha, 1 );

    mShaderQuadImage->SetResource( &(std::string("textBack")), mBackTextureRV );

    ID3D10EffectPass* pass = mShaderQuadImage->mTech->GetPassByIndex(0);
    pass->Apply(0);

    mBackModel->display();
  }

  /////////////////////////////////////////////////////
  //    Render View name
  BaseDX::drawText( aTitle, aScaleGrid );

  mSwapChain->Present( 0, 0 );
}


void  RenderViewApp::setBezModels( std::vector<BezierModel*>& aBezModels )
{
  mBezModels.resize( aBezModels.size() );
  for( unsigned int i = 0 ; i < aBezModels.size() ;++i )
  {
    mBezModels[i] = aBezModels[i];
  }
}

ID3D10Device* RenderViewApp::getD3DXdevice()
{
  return md3dDevice;
}

void RenderViewApp::setBackgroundImageTexture(int w,
                               int h,
                               unsigned char *data)
{
  BaseDX::createTexture( &mBackTexture, &mBackTextureRV, w,h,data );
}

void RenderViewApp::setBackgroundImageModel(ImageQuadModel *aModel)
{
  mBackModel = aModel ;
  mHasBackModel = true;
}

void RenderViewApp::removeBackgroundImageModel()
{
  mHasBackModel = false;
  mBackModel = NULL;
}

void RenderViewApp::releaseBackgroundImageTexture()
{
  ReleaseCOM( mBackTexture );
  ReleaseCOM( mBackTextureRV );
  mBackTexture = NULL;
  mBackTextureRV = NULL;
  mHasBackModel = false;
}

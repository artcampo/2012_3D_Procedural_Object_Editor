#include "ObjEdRenderApp.hpp"

ObjEdRenderApp::ObjEdRenderApp(HWND screen, UINT width, UINT height):
    BaseDX(screen, width, height, false, L""),
    mShaderTranslLines(NULL)

{
  mShaderTranslFaces = NULL;
  mShaderTranslPoints= NULL;
  mShaderTranslColoredLines= NULL;
  mShaderPhongColoredTriangles= NULL;

}

ObjEdRenderApp::~ObjEdRenderApp()
{  
  mTextureRasterizerState->Release();
  delete mShaderTranslLines;
  delete mShaderTranslFaces;
  delete mShaderTranslPoints;
  delete mShaderTranslColoredLines;
  delete mShaderPhongColoredTriangles;

  mModelsProcTex.clear();
  mModels.clear();

  //BaseDX::~BaseDX();
}

bool ObjEdRenderApp::init()
{
  if (!BaseDX::init())
    return false;
  else {

    //  General to the interface



    //  Translucent mode
    mShaderTranslFaces  = new Shader();
    mShaderTranslFaces->Init(gStoredShaders[shadFwdGeomFacePC], md3dDevice);
    mShaderTranslLines        = new Shader();
    mShaderTranslLines->Init(gStoredShaders[shadFwdGeomP], md3dDevice);
    mShaderTranslPoints       = new Shader();
    mShaderTranslPoints->Init(gStoredShaders[shadFwdPointPSC], md3dDevice);
    mShaderTranslColoredLines        = new Shader();
    mShaderTranslColoredLines->Init(gStoredShaders[shadFwdGeomPC], md3dDevice);

    mShaderPhongColoredTriangles        = new Shader();
    mShaderPhongColoredTriangles->Init(gStoredShaders[shadFwdPhongGeomPNC], md3dDevice);



    if (!mShaderTranslFaces->Compile())
      return false;
    if (!mShaderTranslLines->Compile())
      return false;
    if (!mShaderTranslPoints->Compile())
      return false;
    if (!mShaderTranslColoredLines->Compile())
      return false;

    if (!mShaderPhongColoredTriangles->Compile())
      return false;

    // Raster states
    D3D10_RASTERIZER_DESC rasterizerState;
    rasterizerState.CullMode = D3D10_CULL_BACK;
    rasterizerState.FillMode = D3D10_FILL_SOLID;
    rasterizerState.FrontCounterClockwise = true;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = true;
    rasterizerState.ScissorEnable = false;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = false;

    md3dDevice->CreateRasterizerState( &rasterizerState, &mTextureRasterizerState);


    D3D10_DEPTH_STENCIL_DESC l_DefaultStateDesc;
    l_DefaultStateDesc.DepthEnable = true;
    l_DefaultStateDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
    l_DefaultStateDesc.DepthFunc =  D3D10_COMPARISON_LESS;
    l_DefaultStateDesc.StencilEnable = false;
    l_DefaultStateDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
    l_DefaultStateDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;
    l_DefaultStateDesc.FrontFace.StencilFailOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.FrontFace.StencilDepthFailOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.FrontFace.StencilPassOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.FrontFace.StencilFunc  = D3D10_COMPARISON_ALWAYS;
    l_DefaultStateDesc.BackFace.StencilFailOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.BackFace.StencilDepthFailOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.BackFace.StencilPassOp  = D3D10_STENCIL_OP_KEEP;
    l_DefaultStateDesc.BackFace.StencilFunc  = D3D10_COMPARISON_ALWAYS;
    md3dDevice->CreateDepthStencilState(&l_DefaultStateDesc, &mDepthStencilStateDefault);


  }

  return true;
}


void ObjEdRenderApp::drawScene (
  D3DXVECTOR3& camPos,
  D3DXVECTOR3& camTar,
  D3DXVECTOR3& camUp,
  GridModel* aGrid,
  AxisModel* aAxis,  
  std::wstring& aTitle,
  bool aMuteBackground,
  float aScaleGrid,

  bool aShowGizmoT,
  GizmoTMeshModel* aGizmoT,
  bool aShowGizmoR,
  GizmoTMeshModel* aGizmoR,
  bool aShowGizmoS,
  GizmoTMeshModel* aGizmoS,

  bool aShowKniveLine,
  LineListModel* aKniveLine
    )
{

  calculateTime();


  cam->setPositionAndAt( &camPos, &camTar );
  cam->updateCamera( &mView );
  cam->setUpVector(camUp );
  BaseDX::drawScene();

  ID3D10EffectPass* pass;

  /////////////////////////////////////////////////////
  //    Render LINES - STRIP
  mShaderTranslLines->SetMatrix( &(std::string("World")), &mWorld );
  mShaderTranslLines->SetMatrix( &(std::string("View")), &mView );
  mShaderTranslLines->SetMatrix( &(std::string("Projection")), &mProjection );


  md3dDevice->IASetInputLayout( mShaderTranslLines->mVertexLayout );
  //md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );

  pass = mShaderTranslLines->mTech->GetPassByIndex(0);
  pass->Apply(0);

  /////////////////////////////////////////////////////
  //    Render LINES - LIST
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
  if ( aGrid != 0)
  {
    mShaderTranslLines->SetRawValue( &(std::string("colour")), aGrid->getColour(), 4 );
    pass->Apply(0);
    aGrid->display();

    mShaderTranslLines->SetRawValue( &(std::string("colour")), aGrid->getColourAxis(), 4 );
    pass->Apply(0);
    aGrid->displayAxis();

  }

  if (aShowKniveLine)
  {
    //mShaderTranslLines->SetRawValue( &(std::string("colour")), (float*)&aKniveLine->mColour, 4 );
    pass->Apply(0);
    aKniveLine->display();
  }

  if ( aAxis != 0)
  {
    mShaderTranslLines->SetRawValue( &(std::string("colour")), aAxis->getColourX(), 4 );
    pass->Apply(0);
    aAxis->displayX();
    mShaderTranslLines->SetRawValue( &(std::string("colour")), aAxis->getColourY(), 4 );
    pass->Apply(0);
    aAxis->displayY();
    mShaderTranslLines->SetRawValue( &(std::string("colour")), aAxis->getColourZ(), 4 );
    pass->Apply(0);
    aAxis->displayZ();
  }


  if (aShowGizmoT)
  {
    aGizmoT->display();
  }
  if (aShowGizmoR)
  {
    aGizmoR->display();
  }
  if (aShowGizmoS)
  {
    aGizmoS->display();
  }

  /////////////////////////////////////////////////////
  //    Render COLORED LINES - LIST
  //invView
  D3DXMATRIX inv;
  D3DXMatrixInverse( &inv, NULL, &mView);
  mShaderTranslColoredLines->SetMatrix( &(std::string("invView")), &inv );
  mShaderTranslColoredLines->SetMatrix( &(std::string("World")), &mWorld );
  mShaderTranslColoredLines->SetMatrix( &(std::string("View")), &mView );
  mShaderTranslColoredLines->SetMatrix( &(std::string("Projection")), &mProjection );

  md3dDevice->IASetInputLayout( mShaderTranslColoredLines->mVertexLayout );
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );

  pass = mShaderTranslColoredLines->mTech->GetPassByIndex(0);
  pass->Apply(0);

  for( unsigned int i = 0 ; i < mModels.size() ;++i )
    if (mModels[i] != NULL)
    {
      mModels[i]->displayEdges();
    }



  /////////////////////////////////////////////////////
  //    Render Triangles

  mShaderTranslFaces->SetMatrix( &(std::string("World")), &mWorld );
  mShaderTranslFaces->SetMatrix( &(std::string("View")), &mView );
  mShaderTranslFaces->SetMatrix( &(std::string("Projection")), &mProjection );

  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  md3dDevice->IASetInputLayout( mShaderTranslFaces->mVertexLayout );

  pass = mShaderTranslFaces->mTech->GetPassByIndex(0);
  pass->Apply(0);


  for( unsigned int i = 0 ; i < mModels.size() ;++i )
    if (mModels[i] != NULL)
    {
      mModels[i]->displayTriangles();
    }

  /////////////////////////////////////////////////////
  //    Render POINTS

  D3DXMATRIX m = mView * mProjection;
  mShaderTranslPoints->SetMatrix( &(std::string("gViewProj")), &m );
  mShaderTranslPoints->SetMatrix( &(std::string("View")), &mView );
  D3DXMatrixInverse( &m, NULL,&mView);
  mShaderTranslPoints->SetMatrix( &(std::string("invView")), &m );
  mShaderTranslPoints->SetRawValue( &(std::string("gEyePosW")), (float *)&(camPos), 3);
  D3DXVECTOR3 up = cam->getUpVector();
  mShaderTranslPoints->SetRawValue( &(std::string("camUp")), (float *)(&up), 3);
  m = mView * mProjection;
  D3DXMatrixInverse( &m, NULL,&m);
  mShaderTranslPoints->SetMatrix( &(std::string("gInvViewProj")), &m );
  m = mProjection;
  D3DXMatrixInverse( &m, NULL,&m);
  mShaderTranslPoints->SetMatrix( &(std::string("invProj")), &m );

  md3dDevice->IASetInputLayout( mShaderTranslPoints->mVertexLayout );
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
  pass = mShaderTranslPoints->mTech->GetPassByIndex(0);
  pass->Apply(0);

  for( unsigned int i = 0 ; i < mModels.size() ;++i )
    if (mModels[i] != NULL )
    {
      mShaderTranslLines->SetRawValue( &(std::string("colour")),D3DXCOLOR(0,1,0,1), 4 );
      pass->Apply(0);
      mModels[i]->displayVertices();
    }

  /////////////////////////////////////////////////////
  //    Render View name
  BaseDX::drawText( aTitle, aScaleGrid );

  mSwapChain->Present( 0, 0 );
}


void  ObjEdRenderApp::setModels( std::vector<EditableMeshModel*>& aModels )
{
  mModels.clear();
  mModels.resize( aModels.size() );
  for( unsigned int i = 0 ; i < aModels.size() ;++i )
  {
    mModels[i] = aModels[i];
  }
}

void  ObjEdRenderApp::setModels( std::vector<EditableMeshModelProcTex*>& aModels )
{
/*
  for( unsigned int i = 0 ; i < mModelsProcTex.size() ;++i )
    delete mModelsProcTex[i];
*/
  mModelsProcTex.clear();
  mModelsProcTex.resize( aModels.size() );
  for( unsigned int i = 0 ; i < aModels.size() ;++i )
  {
    mModelsProcTex[i] = aModels[i];
  }

}

ID3D10Device* ObjEdRenderApp::getD3DXdevice()
{
  return md3dDevice;
}

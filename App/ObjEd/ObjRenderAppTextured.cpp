#include "App/ObjEdRenderApp.hpp"

void ObjEdRenderApp::drawSceneTextured (
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


  /////////////////////////////////////////////////////
  //    OBJECT: Triangles


  md3dDevice->RSSetState(mTextureRasterizerState);
  md3dDevice->OMSetDepthStencilState(mDepthStencilStateDefault, 0);

  for( unsigned int i = 0 ; i < mModelsProcTex.size() ;++i )
    if (mModelsProcTex[i] != NULL)
    {
      mModelsProcTex[i]->display( camPos, mWorld, mView, mProjection, md3dDevice );
    }


  mShaderTranslLines->SetMatrix( &(std::string("World")), &mWorld );
  mShaderTranslLines->SetMatrix( &(std::string("View")), &mView );
  mShaderTranslLines->SetMatrix( &(std::string("Projection")), &mProjection );


  md3dDevice->IASetInputLayout( mShaderTranslLines->mVertexLayout );
  //md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );

  pass = mShaderTranslLines->mTech->GetPassByIndex(0);
  pass->Apply(0);

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

/*
  /////////////////////////////////////////////////////
  //    Render COLORED LINES - LIST
  mShaderTranslColoredLines->SetMatrix( &(std::string("World")), &mWorld );
  mShaderTranslColoredLines->SetMatrix( &(std::string("View")), &mView );
  mShaderTranslColoredLines->SetMatrix( &(std::string("Projection")), &mProjection );

  md3dDevice->IASetInputLayout( mShaderTranslColoredLines->mVertexLayout );
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );

  pass = mShaderTranslColoredLines->mTech->GetPassByIndex(0);
  pass->Apply(0);

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
*/



  /////////////////////////////////////////////////////
  //    Render View name
  BaseDX::drawText( aTitle, aScaleGrid );

  mSwapChain->Present( 0, 0 );

}


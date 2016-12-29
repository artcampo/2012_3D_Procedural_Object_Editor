#include "App/ObjEdRenderApp.hpp"

void ObjEdRenderApp::drawScenePhong (
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

  mShaderPhongColoredTriangles->SetRawValue( &(std::string("camPos")), (float *)&camPos ,3 );
  mShaderPhongColoredTriangles->SetMatrix( &(std::string("World")), &mWorld );
  mShaderPhongColoredTriangles->SetMatrix( &(std::string("View")), &mView );
  mShaderPhongColoredTriangles->SetMatrix( &(std::string("Projection")), &mProjection );

  md3dDevice->IASetInputLayout( mShaderPhongColoredTriangles->mVertexLayout );
  md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  pass = mShaderPhongColoredTriangles->mTech->GetPassByIndex(0);
  pass->Apply(0);


  for( unsigned int i = 0 ; i < mModels.size() ;++i )
    if (mModels[i] != NULL)
    {
      mModels[i]->displayTriangles();
    }


  /////////////////////////////////////////////////////
  //    OBJECT: edges
  D3DXMATRIX inv;
  D3DXMatrixInverse( &inv, NULL, &mView);
  mShaderTranslColoredLines->SetMatrix( &(std::string("invView")), &inv );
  mShaderTranslColoredLines->SetRawValue( &(std::string("camPos")), (float *)&camPos ,3 );
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
  //    Object: POINTS

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

  if (aShowKniveLine)
  {
    //mShaderTranslLines->SetRawValue( &(std::string("colour")), (float*)&aKniveLine->mColour, 4 );
    pass->Apply(0);
    aKniveLine->display();
  }

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


  /////////////////////////////////////////////////////
  //    Render View name
  BaseDX::drawText( aTitle, aScaleGrid );

  mSwapChain->Present( 0, 0 );

}


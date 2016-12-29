#include "EditorTextureApp.hpp"
#include "Project_Dependant/StoredShaders.hpp"

EditorTextureApp::EditorTextureApp(HWND screen, UINT width, UINT height, bool showFPS):
    BaseTextureDX(screen, width, height, false, L"", showFPS),
    mBox(NULL),
    mfxBox(NULL),
    mMeshMod(NULL),
    mMeshModBatch(NULL),
    mMeshMod2(NULL),
    mMeshModBatch2(NULL)
{}

EditorTextureApp::~EditorTextureApp()
{
  delete   mBox;
  delete   mfxBox;
  delete   mMeshModBatch;  
}

bool EditorTextureApp::init()
{
  if (!BaseTextureDX::init())
    return false;
  else {
    mBox          = new SingleBox(md3dDevice);
    mBox2         = new SingleBox(md3dDevice);
    mfxBox        = new Shader();

    int vertexLayoutSize = sizeof(VertexTextEd);
    mMeshModBatch = new DxMeshModelIndexedBatch(vertexLayoutSize,
                                         D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                                         md3dDevice);
    mMeshModBatch2 = new DxMeshModelIndexedBatch(vertexLayoutSize,
                                         D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                                         md3dDevice);

    mBox->init();
    mMeshMod = new DxMeshModelIndexed(vertexLayoutSize, mBox->getNumVertices(),
                               mBox->getVertices());
    mMeshMod->setIndices(mBox->getIndices(), mBox->getNumFaces());
    mMeshModBatch->addMeshModel(mMeshMod);
    delete mMeshMod;
    mMeshMod = 0;


    mBox2->init(100,1,10);
    mMeshMod2 = new DxMeshModelIndexed(vertexLayoutSize, mBox2->getNumVertices(),
                               mBox2->getVertices());
    mMeshMod2->setIndices(mBox2->getIndices(), mBox2->getNumFaces());
    mMeshModBatch2->addMeshModel(mMeshMod2);
    delete mMeshMod2;
    mMeshMod2 = 0;

    mfxBox->Init(gStoredShaders[indexShadNoise], md3dDevice);
    if (!mfxBox->Compile())
      return false;
    if (!mMeshModBatch->createVB() || !mMeshModBatch->createIB())
      return false;
    if (!mMeshModBatch2->createVB() || !mMeshModBatch2->createIB())
      return false;


    if ( ! generatePermutationTexture() )
        return false;

    cam->setPositionAndAt( &D3DXVECTOR3(2.7,1.3,1.5), &D3DXVECTOR3(0.5,0,0.5));
  }

  return true;
}


void EditorTextureApp::drawScene ( std::string& aChannelName, std::string& aSignalName, bool aDisplayName )
{
  getKeyboard();
  calculateTime();
  BaseTextureDX::drawScene();
  //Boxes
  std::string matrixName = "World";
  D3DXMatrixIdentity( &mWorld );
  mfxBox->SetMatrix(&matrixName, &mWorld);
  matrixName = "View";
  mfxBox->SetMatrix(&matrixName, &mView);
  matrixName = "Projection";
  mfxBox->SetMatrix(&matrixName, &mProjection);
  D3DXVECTOR3 camPos;
  cam->getEyePos( &camPos );
  D3DXVec3TransformCoord( &camPos, &camPos, &mView );
  mfxBox->SetRawValue( &(std::string("camPos")), (float *)(&camPos), 3);
  mfxBox->SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  //  regular box
  mMeshModBatch->display(mfxBox);

  //  like ballp's track
  matrixName = "World";
  D3DXMatrixTranslation( &mWorld, 0, -2, 0 );
  mfxBox->SetMatrix(&matrixName, &mWorld);
  mMeshModBatch2->display(mfxBox);

  std::string displayText = aChannelName + std::string("::") + aSignalName;
  if (aDisplayName)
    BaseTextureDX::drawText(displayText);
  mSwapChain->Present( 0, 0 );
}

bool EditorTextureApp::changeShaderPath(LPCWSTR aPath)
{

  ShaderClassData shaderNoise =
  {
    0,           //id
    aPath, //file name
    indexVertexLayout_POS_NOR_TAN_UV,  //vertexlayout index
    //  sizes
    3,    // numMatrices
    0,    // numRawValues
    1,    // numResources

    //  Techniques
    "ForwardRender",

    //  Matrices
    {
      "World",
      "View",
      "Projection",
    },

    //  Raw Values
    {

    },

    //  Resources
    {
      "PermTexture"
    },
  };
  mfxBox->Init(&shaderNoise, md3dDevice);
  if (!mfxBox->Compile())
    return false;

  return true;
}

void EditorTextureApp::keyPressed(UINT key)
{
  keys[key] = true;
}

void EditorTextureApp::keyReleased(UINT key)
{
  keys[key] = false;
}

void EditorTextureApp::setCamera(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
  cam->setPositionAndAt( &pos, &at );
}


bool EditorTextureApp::generatePermutationTexture()
{
  //  Create Vectors over a semisphere
  Rand::RandGen r(42);
  std::vector<int> perm = r.genPermutations( 1024 );


  HRESULT hr;

  D3D10_TEXTURE1D_DESC desc;
  ZeroMemory( &desc, sizeof(desc) );
  desc.Width  = 256;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R32_UINT;
  desc.Usage = D3D10_USAGE_IMMUTABLE;
  desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  D3D10_SUBRESOURCE_DATA dataDesc;
  ZeroMemory( &dataDesc, sizeof(dataDesc) );
  dataDesc.pSysMem = &perm[0];
  //dataDesc.SysMemPitch = mSsdoTexturePatternSize*sizeof(float)*3;
  dataDesc.SysMemPitch = sizeof(int);
  dataDesc.SysMemSlicePitch = 0;
  hr = md3dDevice->CreateTexture1D(&desc, &dataDesc, &mPermTexture);
  if( FAILED( hr ) )
      return false;

  D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;

  ZeroMemory( &descSRV, sizeof(descSRV) );
  descSRV.Format = DXGI_FORMAT_R32_UINT;
  descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
  descSRV.Texture1D.MipLevels = 1;
  descSRV.Texture1D.MostDetailedMip = 0;

  hr = md3dDevice->CreateShaderResourceView( mPermTexture, &descSRV, &mPermTextureSRV ) ;
  if( FAILED( hr ) )
      return false;

  return true;
}

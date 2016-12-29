#include "BaseTextureDX.hpp"


#include <sstream>

BaseTextureDX::BaseTextureDX(HWND hwnd, int width, int height, bool isFullScreen,
               std::wstring wTitle, bool showFPS):
               screen               (hwnd),
               mSwapChain           (0),
               mDepthStencilBuffer  (0),
               mRenderTargetView    (0),
               mDepthStencilView    (0),
               mFont                (0),
               //appIcon              (0),
               ptsBegin             (0.0f,0.0f,0.0f),
               ptsEnd               (0.0f,0.0f,0.0f),
               cam                  (0),
               currentWidth         (width),
               currentHeight        (height),
               isFullScreen_        (isFullScreen),
               mClearColor          (WHITE),
               secondsCounter       (0.0f),
               nFrame               (0),
               prevTime             (0),
               baseTime             (0),
               deltaTime            (0.0),
               secondsPerCount      (0.0),
               showFPS_             (showFPS),
               wTitle_              (wTitle),
               md3dDevice           (NULL),
               mLastFps             (0)
{
  for (UINT point_i = 0; point_i < 256; ++point_i) 
    keys[point_i] = false; 
  //Text
  text = L"";
  //Set secondsPerCount
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  secondsPerCount = 1.0 / (double)countsPerSec;
}
//Destructor

BaseTextureDX::~BaseTextureDX()
{
	mSwapChain->SetFullscreenState(false, 0);

	if( md3dDevice )
		md3dDevice->ClearState();


	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(md3dDevice);
  ReleaseCOM(mFont);  

  delete physics;
  delete cam;
}

bool BaseTextureDX::init()
{
  if( !initDirectx() )     return false;
  if( !initGameObjects() ) return false;

  startTimer();
  
  return true;
}

bool BaseTextureDX::initGameObjects()
{
  cam     = new Camera();
  physics = new Physics( cam, md3dDevice);

  return true;
}

bool BaseTextureDX::initDirectx()
{
  #if defined(DEBUG) | defined(_DEBUG)
	  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif
    
	DXGI_SWAP_CHAIN_DESC swapchain;
	swapchain.BufferDesc.Width                    = currentWidth;
	swapchain.BufferDesc.Height                   = currentHeight;
	swapchain.BufferDesc.RefreshRate.Numerator    = 60;
	swapchain.BufferDesc.RefreshRate.Denominator  = 1;
	swapchain.BufferDesc.Format                   = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain.BufferDesc.ScanlineOrdering         = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain.BufferDesc.Scaling                  = DXGI_MODE_SCALING_UNSPECIFIED;
	// No multisampling.
	swapchain.SampleDesc.Count   = 1;
	swapchain.SampleDesc.Quality = 0;

	swapchain.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain.BufferCount  = 1;
	swapchain.OutputWindow = screen;  
	swapchain.Windowed     = !isFullScreen_;
	swapchain.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	swapchain.Flags        = 0;

                    
  D3D10_DRIVER_TYPE md3dDriverType  = D3D10_DRIVER_TYPE_HARDWARE;	

	UINT createDeviceFlags = 0;
  #if defined(DEBUG) || defined(_DEBUG)  
      createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
  #endif 
  IDXGIFactory1 *pFactory;
  HR(CreateDXGIFactory1(__uuidof(IDXGIFactory),(void**)(&pFactory)));
  IDXGIAdapter1 *pAdapter;
  pFactory->EnumAdapters1(0,&pAdapter);

  D3D10CreateDevice(pAdapter,md3dDriverType,0,createDeviceFlags,D3D10_SDK_VERSION,&md3dDevice);
  pFactory->CreateSwapChain(md3dDevice,&swapchain,&mSwapChain);  
  ReleaseCOM(pAdapter);
  ReleaseCOM(pFactory);
  
  //Init font
  D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
  fontDesc.Width           = 0;
  fontDesc.Weight          = 0;
  fontDesc.MipLevels       = 1;
  fontDesc.Italic          = false;
  fontDesc.CharSet         = DEFAULT_CHARSET;
  fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
  fontDesc.Quality         = DEFAULT_QUALITY;
  fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
  wcscpy_s(fontDesc.FaceName, L"Times New Roman");

	D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFont); 

	onResize();

  return true;
}
// I/O

void BaseTextureDX::drawScene	()
{
  md3dDevice->ClearRenderTargetView(mRenderTargetView, mClearColor);
	md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
  md3dDevice->OMSetDepthStencilState(0, 0);
  //Set camera
  D3DXMatrixIdentity  ( &mWorld );
  cam->updateCamera   ( &mView );
  D3DXVECTOR3 mEyePos;
  cam->getEyePos ( &mEyePos );  
}

void BaseTextureDX::drawText( std::string& aText )
{  
  ++nFrame;

  std::wstring str2(aText.length(), L' '); // Make room for characters
  std::copy(aText.begin(), aText.end(), str2.begin());
  text = str2;


  if( getDeltaTime() >= 1.0f ){

    // Reset
    mLastFps = nFrame;
    nFrame = 0;
    secondsCounter  += 1.0f;

  }
  std::wostringstream textStream;
  textStream.precision(6);
  textStream << L"  - FPS: " << mLastFps << L"";
  std::wstring t = textStream.str();

  text += t;
/*
  if( ( getDeltaTime()) >= 1.0f ){
	  std::wostringstream textStream;   
	  textStream.precision(6);
    D3DXVECTOR3 e,a;
    cam->getEyePos( &e );
    cam->getEyeAt( &a );
    float centiseconds = getCentiseconds();
    
    if (showFPS_) {
      textStream << L"FPS: " << nFrame << L"\n";

      textStream
        << L"Centiseconds: "<< centiseconds << L"\n"
        << L"CamPos: "<< e.x <<L","<<e.y<<L","<<e.z<< L"\n"
        << L"CamAt: "<< a.x <<L","<<a.y<<L","<<a.z<< L"\n";
    }
     
    text = textStream.str();
	  // Reset
	  nFrame = 0;
	  secondsCounter  += 1.0f;
  }*/
  
  RECT Rect = {5, 5, 0, 0};
  //SACA UN WARNING A SABER PORQUE
  mFont->DrawText(0, text.c_str(), -1, &Rect, DT_NOCLIP, BLACK);
}

void BaseTextureDX::onResize()
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);
	// Resize the swap chain and recreate the render target view.
	mSwapChain->ResizeBuffers(1, currentWidth, currentHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D10Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);
	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width              = currentWidth;
	depthStencilDesc.Height             = currentHeight;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage              = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; 
	depthStencilDesc.MiscFlags          = 0;

	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	// Bind the render target view and depth/stencil view to the pipeline.
	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	// Set the viewport transform.
	D3D10_VIEWPORT vp;

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = currentWidth;
	vp.Height   = currentHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	md3dDevice->RSSetViewports(1, &vp);  

  D3DXMatrixPerspectiveFovLH( &mProjection, ( float )D3DX_PI * 0.5f, currentWidth / (FLOAT)currentHeight, 0.1f, 10000.0f );
  
}

void BaseTextureDX::getKeyboard()
{
  if(keys[VK_UP]) {
    physics->Move(KUP);
  }
  if(keys[VK_RIGHT]) {
    physics->Move(KRIGHT);
  }  
  if(keys[VK_DOWN]) {
    physics->Move(KDOWN);
  }
  if(keys[VK_LEFT]) {
    physics->Move(KLEFT);
  }  
  if(keys['W']) {
    physics->Move(KW);
  }
  if(keys['S']) {
    physics->Move(KS);
  }
}
//Modifiers
void BaseTextureDX::enableFps	()
{
	showFPS_ = true;
}

void BaseTextureDX::disableFps()
{
	showFPS_ = false;
}

//Time
void BaseTextureDX::startTimer()
{
	__int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  prevTime = baseTime = currTime;
  nFrame   = 0;
}
void BaseTextureDX::calculateTime()
{
  __int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
	deltaTime = (currTime - prevTime)*secondsPerCount;
	prevTime  = currTime;  
}

double BaseTextureDX::getSeconds()
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - baseTime) * secondsPerCount;
}


float BaseTextureDX::getDeltaTime() const
{   
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	
  
  return ((currTime - baseTime) * (float)secondsPerCount) - (float)secondsCounter;
}

float BaseTextureDX::getCentiseconds()
{
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);	

  return (currTime - baseTime) * (float)secondsPerCount * 100.0f;
}

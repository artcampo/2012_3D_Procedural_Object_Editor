#ifndef _BASEDX_TEXTURE_HPP_
#define _BASEDX_TEXTURE_HPP_

#include <iostream>

#include	"dx_misc.hpp"
//#include	"ParticleManager.hpp"
#include	"physics/Physics.hpp" 

class BaseTextureDX
{
  public:

    BaseTextureDX();
    BaseTextureDX(HWND hwnd, int width, int height, bool isFullScreen,
               std::wstring wTitle, bool showFPS);
    virtual ~BaseTextureDX();
    //Windows
    int Main();    
    //Modifiers
    void disableFps	();    
		void enableFps	();    
    //Time
    void startTimer();
    void calculateTime();
    float getDeltaTime() const;
    float mGetDeltaTime() const;
    float getExtendedTime();
    double getSeconds();
    float getCentiseconds();
    // Inheritance
    virtual bool init();
    virtual void onResize();
    virtual void getKeyboard();
    virtual void drawScene();
    virtual void drawText( std::string& aText );
  protected:
    //		Game classes
    Camera*           cam;
    Physics*				  physics;    
    //
    D3DXMATRIX          mWorld;
    D3DXMATRIX          mView;
    D3DXMATRIX          mProjection;
    //		Dx device
    IDXGISwapChain*         mSwapChain;
    ID3D10Texture2D*        mDepthStencilBuffer;
    ID3D10RenderTargetView* mRenderTargetView;
    ID3D10DepthStencilView* mDepthStencilView;    
    ID3D10Device*           md3dDevice;
	  //		Windows
		//HINSTANCE hInstance;
    HWND screen;
    int currentWidth, currentHeight;
    bool isFullScreen_;
    //HICON appIcon;
    bool keys[256];
    std::wstring wTitle_;    
    D3DXCOLOR               mClearColor;      //Background color   
    //Mouse points
    D3DXVECTOR3 ptsBegin, ptsEnd;
    //Text
    bool		  showFPS_;
    ID3DX10Font* mFont;
    std::wstring text;
    //Time
  	double secondsPerCount;  
	  double deltaTime;
    __int64 baseTime;
	  __int64 prevTime;
    double secondsCounter;
    //FPS
    unsigned int nFrame; 
    unsigned int mLastFps;

    // *** Functions *** 

    bool initWindowsApp();
    bool initDirectx();
    bool initGameObjects(); 
};
#endif //_BASEDX_HPP_

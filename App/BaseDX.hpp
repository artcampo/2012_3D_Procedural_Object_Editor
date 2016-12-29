#ifndef _BASEDX_HPP_
#define _BASEDX_HPP_

#include <iostream>

#include	"dx_misc.hpp"
//#include	"ParticleManager.hpp"
#include	"physics/Physics.hpp" 
#include <iostream>
#include <sstream>

class BaseDX
{
  public:

    BaseDX();
    BaseDX(HWND hwnd, int width, int height, bool isFullScreen,
               std::wstring wTitle);
    virtual ~BaseDX();
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
    //virtual void drawScene();
    void drawScene();

    virtual void drawText( std::wstring& aText, float aScaleGrid  );

    void createTexture( ID3D10Texture2D**   texture,
                                ID3D10ShaderResourceView** textureRSV,
                                int w,
                                int h,
                                unsigned char *data);
public:
    D3DXMATRIX          mWorld;
    D3DXMATRIX          mView;
    D3DXMATRIX          mProjection;
    D3D10_VIEWPORT mViewport;
  protected:
    //		Game classes
    Camera*           cam;
    Physics*				  physics;

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
    std::wstring wTitle_;    
    D3DXCOLOR               mClearColor;      //Background color   
    std::wstring text;

    //Time
  	double secondsPerCount;  
	  double deltaTime;
    __int64 baseTime;
	  __int64 prevTime;
    double secondsCounter;
    //FPS
    unsigned int nFrame; 

    // *** Functions *** 

    bool initWindowsApp();
    bool initDirectx();
    bool initGameObjects(); 
};
#endif //_BASEDX_HPP_

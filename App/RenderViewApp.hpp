#ifndef RENDER_VIEW_HPP
#define RENDER_VIEW_HPP

#include "App\BaseDX.hpp"
#include "Shader\Shader.hpp"
#include "Project_Dependant\StoredShaders.hpp"
#include "Geometry\DxMeshModel.hpp"
//#include "App\Global.hpp"
#include "App/beziermodel.h"
#include "App/GridModel.h"
#include "App/AxisModel.h"
#include "App/ImageQuadModel.hpp"
#include "pairtangentsmodel.h"

class RenderViewApp : public BaseDX
{
  public:
    RenderViewApp(HWND screen, UINT width, UINT height);
    ~RenderViewApp();
    bool init();
    bool changeShaderPath(LPCWSTR aPath);

    void keyPressed(UINT key);
    void keyReleased(UINT key);

    void drawScene(  D3DXVECTOR3& camPos,
                     D3DXVECTOR3& camTar,
                     D3DXVECTOR3& camUp,
                     GridModel* aGrid,
                     AxisModel* aAxis,
                     PairTangentsModel* aPairTangentsModel,
                     std::wstring& aTitle,
                     bool aMuteBackground,
                     float aScaleGrid );

    void  setBezModels( std::vector<BezierModel*>& aBezModels );

    ID3D10Device* getD3DXdevice();
    void setBackgroundImageTexture(int w,
                                   int h,
                                   unsigned char *data);
    void setBackgroundImageModel( ImageQuadModel* aModel);
    void removeBackgroundImageModel();
    void releaseBackgroundImageTexture();

  private:
    //Box

    Shader*    mShaderLines;
    Shader*    mShaderPoints;
    Shader*    mShaderQuadImage;

    std::vector<BezierModel*> mBezModels;

    bool                        mHasBackModel;
    ImageQuadModel*             mBackModel;
    ID3D10Texture2D*            mBackTexture;
    ID3D10ShaderResourceView*   mBackTextureRV;



};

#endif // EDITORAPP_HPP

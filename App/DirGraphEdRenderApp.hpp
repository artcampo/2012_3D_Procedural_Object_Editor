#ifndef _DirGraphEdRenderApp_HPP
#define _DirGraphEdRenderApp_HPP

#include "App\BaseDX.hpp"
#include "Shader\Shader.hpp"
#include "Project_Dependant\StoredShaders.hpp"
#include "Geometry\DxMeshModel.hpp"
#include "App/ObjEd/GizmoTMeshModel.hpp"

#include "App/GridModel.h"
#include "App/AxisModel.h"
#include "App/ImageQuadModel.hpp"
#include "App/EditableDirGraph.h"


class DirGraphEdRenderApp : public BaseDX
{
  public:
    DirGraphEdRenderApp(HWND screen, UINT width, UINT height);
    ~DirGraphEdRenderApp();
    bool init();
    bool changeShaderPath(LPCWSTR aPath);

    void keyPressed(UINT key);
    void keyReleased(UINT key);

    void drawScene(  D3DXVECTOR3& camPos,
                     D3DXVECTOR3& camTar,
                     D3DXVECTOR3& camUp,
                     GridModel* aGrid,
                     AxisModel* aAxis,
                     bool aShowGizmoT,
                     GizmoTMeshModel* aGizmoT,
                     std::wstring& aTitle,
                     bool aMuteBackground,
                     float aScaleGrid,                   
                     bool aShowKniveLine,
                     LineListModel* aKniveLine
                     );



    void  setModel( EditableDirGraph* aModel );

    ID3D10Device* getD3DXdevice();

  private:
    ID3D10RasterizerState* mTextureRasterizerState;
    ID3D10DepthStencilState* mDepthStencilStateDefault;

    //  Translucent
    Shader*    mShaderTranslFaces;
    Shader*    mShaderTranslLines;
    Shader*    mShaderTranslPoints;
    Shader*    mShaderTranslColoredLines;
    Shader*    mShaderPhongColoredTriangles;

    Shader*    mShaderGraphLines;

    EditableDirGraph* mModel;
    bool              mHasModel;

};

#endif // _DirGraphEdRenderApp_HPP

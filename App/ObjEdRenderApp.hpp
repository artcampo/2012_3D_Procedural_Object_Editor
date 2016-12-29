#ifndef _ObjEdRenderApp_HPP
#define _ObjEdRenderApp_HPP

#include "App\BaseDX.hpp"
#include "Shader\Shader.hpp"
#include "Project_Dependant\StoredShaders.hpp"
#include "Geometry\DxMeshModel.hpp"


#include "App/GridModel.h"
#include "App/AxisModel.h"
#include "App/ImageQuadModel.hpp"
#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"
#include "App/ObjEd/GizmoTMeshModel.hpp"


class ObjEdRenderApp : public BaseDX
{
  public:
    ObjEdRenderApp(HWND screen, UINT width, UINT height);
    ~ObjEdRenderApp();
    bool init();
    bool changeShaderPath(LPCWSTR aPath);

    void keyPressed(UINT key);
    void keyReleased(UINT key);

    void drawScene(  D3DXVECTOR3& camPos,
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
                     );

    void drawScenePhong
                  (  D3DXVECTOR3& camPos,
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
                     );


    void drawSceneTextured
                  (  D3DXVECTOR3& camPos,
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
                     );



    void  setModels( std::vector<EditableMeshModel*>& aModels );
    void  setModels( std::vector<EditableMeshModelProcTex*>& aModels );

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

    std::vector<EditableMeshModel*> mModels;
    std::vector<EditableMeshModelProcTex*> mModelsProcTex;

};

#endif // _ObjEdRenderApp_HPP

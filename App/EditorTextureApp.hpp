#ifndef EditorTextureApp_HPP
#define EditorTextureApp_HPP

#include "App\BaseTextureDX.hpp"
#include "App\SingleBox.hpp"
#include "Shader\Shader.hpp"
#include "Project_Dependant\StoredShaders.hpp"
#include "Geometry\DxMeshModelIndexed.hpp"
#include "Noise/RandomNumberGenerator.hpp"

class EditorTextureApp : public BaseTextureDX
{
  public:
    EditorTextureApp(HWND screen, UINT width, UINT height, bool showFPS);
    ~EditorTextureApp();
    bool init();
    bool changeShaderPath(LPCWSTR aPath);
    void drawScene( std::string& aChannelName, std::string& aName, bool aDisplayName );
    void keyPressed(UINT key);
    void keyReleased(UINT key);
    void setCamera( D3DXVECTOR3 pos, D3DXVECTOR3 at );

  private:
    //Box
    SingleBox* mBox;
    SingleBox* mBox2;
    Shader*    mfxBox;
    //Geometry
    DxMeshModelIndexed* mMeshMod;
    DxMeshModelIndexedBatch* mMeshModBatch;
    DxMeshModelIndexed* mMeshMod2;
    DxMeshModelIndexedBatch* mMeshModBatch2;

    ID3D10Texture1D*                    mPermTexture;
    ID3D10ShaderResourceView*           mPermTextureSRV;

private:
    bool    generatePermutationTexture();

};

#endif // EditorTextureApp_HPP

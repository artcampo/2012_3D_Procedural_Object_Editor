#ifndef _DX_CAMERA_MANAGER_
#define _DX_CAMERA_MANAGER_

#include <d3dx10.h>
#include <dxerr.h>

class Camera
{
  public:
    Camera();
    void    updateCamera      ( D3DXMATRIX* mView );
    void    getEyePos         ( D3DXVECTOR3* eyePos);
    void    getEyeAt          ( D3DXVECTOR3* eyePos);
    void    incrementEyePos   ( D3DXVECTOR3* incPos);
    void    incrementTargetPos( D3DXVECTOR3* incPos);
    void    setUpVector (D3DXVECTOR3& aUp);
    D3DXVECTOR3 getUpVector ();

    //  to control camera with a path
    void    setPositionAndAt( D3DXVECTOR3* eyePos, D3DXVECTOR3* atAdd );

  private:
    D3DXVECTOR3 At;
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Up;

    D3DXMATRIX  camera_frame_basis;

    void recomputeTbnView();
};


//---------------------------------------------
// EXTERN FUNCTIONS
//---------------------------------------------

void  dx_3dPerpendicular ( D3DXVECTOR3* n, D3DXVECTOR3* v );

#endif

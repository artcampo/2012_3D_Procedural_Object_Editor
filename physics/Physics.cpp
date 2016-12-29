#include "Physics.hpp"

Physics::Physics ( Camera* aCam, ID3D10Device*       ad3dDevice):
  physicsCam_(aCam),
  inc(0.036f),
  md3dDevice(ad3dDevice)
{}

Physics::~Physics()
{}

void Physics::Move ( unsigned int key )
{
    switch( key )
    {
      case KUP:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f, inc ) );
        break;

      case KDOWN:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f,-inc ) );
        break;

      case KRIGHT:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( inc, 0.0f, 0.0f ) ); 
        break;

      case KLEFT:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( -inc, 0.0f, 0.0f ) );
        break;

      case KW:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( 0.0f,inc, 0.0f ) );
        break;

      case KS:
        physicsCam_->incrementEyePos( &D3DXVECTOR3( 0.0f,-inc, 0.0f ) );
        break;
    };
    
}

void  Physics::MouseMove ( D3DXVECTOR3* inc_mouse )
{
  //camera_frame_basis()
  D3DXVECTOR3 v = 0.05f*(*inc_mouse);
  physicsCam_->incrementTargetPos( &v ); 
}

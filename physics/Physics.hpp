#ifndef PHYSICS_HPP
#define PHYSICS_HPP


#include  "../Camera/Camera.hpp"
#include  "../App/dx_misc.hpp"

class Physics {
 
public:	
	Physics( Camera* cam, ID3D10Device*       ad3dDevice );
	~Physics();

	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse );

private:
  float          inc;
  Camera*        physicsCam_;
  ID3D10Device*  md3dDevice;

};


#endif

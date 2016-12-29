#ifndef _SINGLEBOX_H
#define _SINGLEBOX_H

#include "dx_misc.hpp"
//#include "Global.hpp"


class SingleBox
{
public:

	SingleBox(ID3D10Device*           ad3dDevice);
	~SingleBox();

        void init( const float aScaleX = 1.0f,  const float aScaleY = 1.0f ,  const float aScaleZ = 1.0f  );
  void display();

  VertexTextEd* getVertices();
  DWORD*        getIndices();
  ID3D10Buffer* getVertexBuffer();
  ID3D10Buffer* getIndexBuffer();
  DWORD         getNumFaces();
  DWORD         getNumVertices();

private:

  VertexTextEd* mVertices;
  DWORD*        mIndices;
	DWORD         mNumVertices;
	DWORD         mNumFaces;
	
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;  

	ID3D10Device*           md3dDevice;
};

#endif // SINGLEBOX_H

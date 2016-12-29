#ifndef _VERTEXLAYOUT_HPP_
#define _VERTEXLAYOUT_HPP_

#include <d3dx10.h>
#include <dxerr.h>


struct VertexLayout{
  D3D10_INPUT_ELEMENT_DESC*   mVertexLayout;
  int                         mNumElements;
};

const static int VERTLAY_POS_TEX = 0;


#endif
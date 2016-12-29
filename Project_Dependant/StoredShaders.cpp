/*!
 * \file StoredShaders.cpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include    "Shader/Shader.hpp"
#include    "Shader/VertexLayout.hpp"
#include    "Project_Dependant/StoredShaders.hpp"



//===========================================================================
/*
  VertexLayout definitions.
*/
//===========================================================================

const static int numVertexLayouts = 10;

D3D10_INPUT_ELEMENT_DESC layout_1[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};

D3D10_INPUT_ELEMENT_DESC layout_2[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_3[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
};

D3D10_INPUT_ELEMENT_DESC layout_4[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "SIZE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_5[] =  {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "SIZE",   0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_6[] =  {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};

D3D10_INPUT_ELEMENT_DESC layout_PC[] =	{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
};


D3D10_INPUT_ELEMENT_DESC layout_PNC[] =  {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_PNUV[] =  {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

D3D10_INPUT_ELEMENT_DESC layout_PNTUV[] =  {
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

VertexLayout shaderVertexLayouts[numVertexLayouts] = 
{
  { &layout_1[0], 2},
  { &layout_2[0], 2},
  { &layout_3[0], 1},
  { &layout_4[0], 2},
  { &layout_5[0], 3},
  { &layout_6[0], 2},
  { &layout_PC[0], 2},
  { &layout_PNC[0], 3},
  { &layout_PNUV[0],3},
  { &layout_PNTUV[0],4}
};


//===========================================================================
/*
  Shaders
*/
//===========================================================================

#include "StoredShadersGeneralEditor.cpp"
#include "StoredShadersPhong.cpp"
#include "StoredShadersTranslucent.cpp"

//===========================================================================
/*
  Shader table.
*/
//===========================================================================

ShaderClassData*  gStoredShaders[gNumStoredShaders] =
{
  &shaderFwdGeometry,
  &shaderFwdPoint,
  &shaderFwdTrianglesUV,
  &shaderColor,
  &shaderNoise,
  &shaderFwdGeometryColour,
  &shaderFwdGeometryFaceColour,

  // phong
  &shaderFwdPhongGeometry,
  &shaderFwdPhongPoint,
  &shaderFwdPhongGeometryColour,

  //graph
  &shaderFwdLineGraphPC

};

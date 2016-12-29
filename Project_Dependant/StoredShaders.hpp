#ifndef _STOREDSHADERS_HPP_
#define _STOREDSHADERS_HPP_

/*!
 * \file SotredShaders.hpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"


//===========================================================================
/*
  Identifiers of VertexLayouts
*/
//===========================================================================

const static int indexVertexLayout_POS_TEX  = 0;
const static int indexVertexLayout_POS_NOR  = 1;
const static int indexVertexLayout_POS      = 2;
const static int indexVertexLayout_POS_SIZ  = 3;
const static int indexVertexLayout_POS_SIZ_COL  = 4;
const static int indexVertexLayout_POS_UV  = 5;
const static int indexVertexLayout_POS_COL  = 6;
const static int indexVertexLayout_POS_NOR_COL  = 7;
const static int indexVertexLayout_POS_NOR_UV  = 8;
const static int indexVertexLayout_POS_NOR_TAN_UV  = 9;



//===========================================================================
/*
  Shader table.
*/
//===========================================================================


extern ShaderClassData*  gStoredShaders[gNumStoredShaders];

enum eShaderIndices{
  //  Translucent mode
  shadFwdGeomP,
  shadFwdPointPSC,
  shadFwdTrianglesPUV,    //  Draw aud
  indexShadColor,
  indexShadNoise,
  shadFwdGeomPC,
  shadFwdGeomFacePC,

  //  Opaque-Phong mode
  shadFwdPhongGeomP,
  shadFwdPhongPointPSC,
  shadFwdPhongGeomPNC,

  //  Render for graph
  shadFwdLineGraphPC
};


#endif

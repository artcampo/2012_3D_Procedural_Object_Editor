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
  Shaders definitions: general
*/
//===========================================================================


//===========================================================================
/*
  Shaders definitions: general
*/
//===========================================================================


ShaderClassData shaderFwdTrianglesUV =
  {
    shadFwdTrianglesPUV,          //id

    L"",             //file name
    indexVertexLayout_POS_UV,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    1,    // numRawValues
    1,    // numResources

    //  Techniques
    "test",

    //  Matrices
    {
      "World",
      "View",
      "Projection",
    },

    //  Raw Values
    {
      "imageAlpha"
    },

    //  Resources
   {
      "textBack"
    },

  };


ShaderClassData shaderColor =
  {
    indexShadColor,           //id
    L"./Assets/shaders/color.fx",                 //file name
    indexVertexLayout_POS_COL,  //vertexlayout index

    //  sizes
    1,    // numMatrices
    0,    // numRawValues
    0,    // numResources

    //  Techniques
    "ColorTech",

    //  Matrices
    {
      "gWVP",
    },

    //  Raw Values
    {

    },

    //  Resources
    {

    },

  };

ShaderClassData shaderNoise =
  {
    indexShadColor,           //id
    L"./Assets/shaders/noise.fx",                 //file name
    indexVertexLayout_POS_NOR_UV,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    0,    // numRawValues
    1,    // numResources

    //  Techniques
    "ForwardRender",

    //  Matrices
    {
      "World",
      "View",
      "Projection",
    },

    //  Raw Values
    {

    },

    //  Resources
    {
      "PermTexture"
    },

  };

ShaderClassData shaderFwdLineGraphPC =
  {
    shadFwdLineGraphPC,          //id

    L"./Assets/shaders/fwdLineGraphPC.fx",             //file name
    indexVertexLayout_POS_COL,  //vertexlayout index

    //  sizes
    4,    // numMatrices
    1,    // numRawValues
    0,    // numResources

    //  Techniques
    "FWDC",

    //  Matrices
    {
      "World",
      "View",
      "Projection",
      "invView"
    },

    //  Raw Values
    {
      "camPos"
    },

    //  Resources
    {
    },

  };


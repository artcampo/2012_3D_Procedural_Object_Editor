/*!
 * \file StoredShaders.cpp
 * \brief Static data with shaders definition
 * \author Arturo
 */

#include		"Shader/Shader.hpp"
#include		"Shader/VertexLayout.hpp"
#include    "Project_Dependant/StoredShaders.hpp"


//===========================================================================
/*
  Shaders definitions.
*/
//===========================================================================

//===========================================================================
/*
  Shaders: PHONG
*/
//===========================================================================


ShaderClassData shaderFwdPhongGeometry =
  {
    shadFwdPhongGeomP,          //id

    L"C:/Arturo/Democoding/Small_Projects/qt_projects/ObjEd/Assets/shaders/editor/fwdPhongGeomPNC.fx",             //file name
    indexVertexLayout_POS_NOR_COL,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    1,    // numRawValues
    0,    // numResources

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
      "camPos"
    },

    //  Resources
   {
    },
    
  };

ShaderClassData shaderFwdPhongPoint =
  {
    shadFwdPhongPointPSC,          //id

    L"C:/Arturo/Democoding/Small_Projects/qt_projects/ObjEd/Assets/shaders/fwdPhongPointPSC.fx",             //file name
    indexVertexLayout_POS_SIZ_COL,  //vertexlayout index

    //  sizes
    5,    // numMatrices
    2,    // numRawValues
    0,    // numResources

    //  Techniques
    "ParticleTest",

    //  Matrices
    {
      "gViewProj",
      "View",
      "invView",
      "gInvViewProj",
      "invProj"

    },

    //  Raw Values
    {
      "gEyePosW",
      "camUp"
    },

    //  Resources
    {
    },

};


ShaderClassData shaderFwdPhongGeometryColour =
  {
    shadFwdPhongGeomPNC,          //id

    L"./Assets/shaders/fwdPhongGeomPC.fx",             //file name
    indexVertexLayout_POS_NOR_COL,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    1,    // numRawValues
    0,    // numResources

    //  Techniques
    "FWDC",

    //  Matrices
    {
      "World",
      "View",
      "Projection",
    },

    //  Raw Values
    {
      "camPos"
    },

    //  Resources
    {
    },

  };



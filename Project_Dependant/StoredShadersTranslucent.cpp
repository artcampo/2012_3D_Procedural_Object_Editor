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
  Shaders definitions: translucent
*/
//===========================================================================


ShaderClassData shaderFwdGeometry = 
  {
    shadFwdGeomP,          //id

    L"C:/Arturo/Democoding/Small_Projects/qt_projects/ObjEd/Assets/shaders/fwdGeomP.fx",             //file name
    indexVertexLayout_POS,  //vertexlayout index

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
      "colour"
    },

    //  Resources
   {
      "PermTexture"
    },
    
  };

ShaderClassData shaderFwdPoint =
  {
    shadFwdPointPSC,          //id

    L"C:/Arturo/Democoding/Small_Projects/qt_projects/ObjEd/Assets/shaders/fwdPointPSC.fx",             //file name
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



ShaderClassData shaderFwdGeometryFaceColour =
  {
    shadFwdGeomFacePC,          //id

    L"",             //file name
    indexVertexLayout_POS_COL,  //vertexlayout index

    //  sizes
    3,    // numMatrices
    0,    // numRawValues
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
    },

    //  Resources
    {
    },

  };

ShaderClassData shaderFwdGeometryColour =
  {
    shadFwdGeomPC,          //id

    L"./Assets/shaders/fwdGeomPC.fx",             //file name
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

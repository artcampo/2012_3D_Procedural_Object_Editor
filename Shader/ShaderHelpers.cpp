#include "ShaderHelpers.hpp"


namespace ShaderHelpers{

ShaderClassData basicShader()
{
  ShaderClassData data;

  data.mId = 10;
  data.mIndexVertexLayout = 1;  //indexVertexLayout_POS_NOR
  data.mNumMatrices = 3;
  data.mMatrices[0] = "World";
  data.mMatrices[1] = "View";
  data.mMatrices[2] = "Projection";
  data.mNumRawValues = 1;
  data.mRawValueName[0] = "camPos";
  data.mNumResources = 1;
  data.mResourceName[0] = "PermTexture";
  data.mTechName = "ForwardRender";


  return data;
}

};


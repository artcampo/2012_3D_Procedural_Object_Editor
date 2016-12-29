#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>

#include "VertexLayout.hpp"

//===========================================================================
/*
  VertexLayout definitions come from file "StoredShaders.cpp" which
  is project dependant.
*/
//===========================================================================
extern const int numVertexLayouts;      /*!< number of different vertex layouts */
extern VertexLayout shaderVertexLayouts[];

static const int gNumStoredShaders = 11; /*!< number of different shaders */

//===========================================================================
/*
  Tweak these to the minimum to reduce exe size
*/
//===========================================================================
const static int ShaderClassMaxMatrices      = 5;
const static int ShaderClassMaxRawValues     = 10;
const static int ShaderClassMaxResources     = 5;

//===========================================================================
/*
  Various structs concerning shaders
*/
//===========================================================================
struct ShaderMatrix{
  std::string                   mMatrixName;
  ID3D10EffectMatrixVariable*   mMatrixVar;
};

struct ShadeRawValue{
  std::string           mRawValueName;
  ID3D10EffectVariable* mRawValueVar;
};

struct ShaderResource{
  std::string                           mResourceName;
  ID3D10EffectShaderResourceVariable*   mResourceVar;
};

struct ShaderClassData{
  int           mId;
  LPCWSTR       mFileName;
  int           mIndexVertexLayout;

  int           mNumMatrices;
  int           mNumRawValues;
  int           mNumResources;

  std::string        mTechName;
  std::string        mMatrices     [ShaderClassMaxMatrices];
  std::string        mRawValueName [ShaderClassMaxRawValues];
  std::string        mResourceName [ShaderClassMaxResources];
};

//===========================================================================
/*!
      \class      Shader
      \brief      Class that handles dx10 shaders, hlsl 4.0
*/
//===========================================================================
class Shader
{
public:
  Shader (){};
  ~Shader ();

  void Init         ( ShaderClassData *aData, ID3D10Device* ad3dDevice );
  bool Compile      ( );
  void  Release     ();

  void SetResource  ( std::string* aName, ID3D10ShaderResourceView* aDesc );
  void SetMatrix    ( std::string* aName, D3DXMATRIX* aMatrix );
  void SetRawValue  ( std::string* aName, float* aData, int aNum );

  ID3D10EffectTechnique*  mTech;          /*!< Handle to technique*/
  ID3D10InputLayout*      mVertexLayout;  /*!< How vertices are defined */
  ID3D10Device*           md3dDevice;

private:

  int                     mId;        /*!< Unique shader identifier */
  LPCWSTR                 mFileName;  /*!< fx file name */
  int                     mIndexVertexLayout;  /*!< Index of vertex layout used*/

  int                     mNumMatrices; /*!< Number of matrices used */
  int                     mNumRawValues;/*!< Number of non-matrix uniforms used */
  int                     mNumResources;/*!< Number of resources used (mostly textures)*/

  
  ID3D10Effect*           mEffect;        /*!< Handle to effect*/

  
  std::string             mTechName;      /*!< Technique's name*/

  ShaderMatrix            mMatrices [ShaderClassMaxMatrices];  /*!< Array with handlers to matrices */
  ShadeRawValue           mRawValues[ShaderClassMaxRawValues]; /*!< Array with handlers to raw values*/
  ShaderResource          mResources[ShaderClassMaxResources]; /*!< Array with handlers to resources*/
};


//---------------------------------------------
// EXTERN FUNCTIONS
//---------------------------------------------


#endif

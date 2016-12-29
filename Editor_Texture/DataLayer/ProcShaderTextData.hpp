#ifndef _PROCSHADERTEXTDATA_HPP_
#define _PROCSHADERTEXTDATA_HPP_


#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

namespace ProcShadTextData{

  const std::string headerDef = "//  Procedural Texture Shader v0.001\n//  Deferred render\n//  Not Gordian\n\n";
  const std::string header = "//  Procedural Texture Shader v0.001\n//  Forward render\n//  Not Gordian\n\n";

  const std::string includes =
      "#include \"simplex.fx\"\n#include \"shader_defines.fx\"\n"
      "#include \"lighting.fx\"\n\n";

  const std::string includesDef =
      "#include \"simplex.fx\"\n#include \"shader_defines.fx\"\n\n";

  const std::string uniforms    = "matrix World;\nmatrix View;\nmatrix Projection;\nfloat3 camPos;\n\n";
  const std::string uniformsDef = "matrix World;\nmatrix View;\nmatrix Projection;\nfloat3 camPos;\n\n";
  
  const std::string layoutVsIn =
"struct VS_INPUT\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float3 nor : NORMAL;\n"
"    float3 tan : TANGENT;\n"
"    float2 uv  : TEXCOORD;\n"
"};\n\n";

  const std::string layoutVsInDef1 =
"struct VS_INPUT\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float3 nor : NORMAL;\n"
"    float3 tan : TANGENT;\n"
"    float2 uv  : TEXCOORD;\n"
"};\n\n";

  const std::string layoutVsInDef2 =
"struct VS_INPUT\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float3 nor : NORMAL;\n"
"    float3 tan : TANGENT;\n"
"};\n\n";

  
  const std::string layoutVsOut =
"struct VS_OUTPUT\n"
"{\n"
"    float4 pos   : SV_POSITION;\n"
"    float4 nor   : NORMAL;\n"
"    float4 tan   : TANGENT;\n"
"    float2 uv    : TEXCOORD;\n"
"    float4 posLS : POSITION1;\n"
"    float4 posWS : POSITION2;\n"
"    float  depth : SV_ClipDistance0;\n"
"};\n\n";

  const std::string layoutVsOutDef1 =
"struct VS_OUTPUT\n"
"{\n"
"    float4 pos   : SV_POSITION;\n"
"    float4 nor   : NORMAL;\n"
"    float4 tan   : TANGENT;\n"
"    float4 posLS : POSITION1;\n"
"    float4 posWS : POSITION2;\n"
"    float2 uv    : TEXCOORD;\n"
"    float  depth : SV_ClipDistance0;\n"
"};\n"
"\n\n"
"struct PS_OUTPUT\n"
"{\n"
"    float4 pos : SV_Target0;\n"
"    float4 col : SV_Target1;\n"
"    float4 nor : SV_Target2;\n"
"    float4 tan : SV_Target3;\n"
"    float4 lig : SV_Target4;\n"
"};\n\n";

  const std::string layoutVsOutDef2 =
"struct VS_OUTPUT\n"
"{\n"
"    float4 pos   : SV_POSITION;\n"
"    float4 nor   : NORMAL;\n"
"    float4 tan   : TANGENT;\n"
"    float4 posLS : POSITION1;\n"
"    float4 posWS : POSITION2;\n"
"    float  depth : SV_ClipDistance0;\n"
"};\n"
"\n\n"
"struct PS_OUTPUT\n"
"{\n"
"    float4 pos : SV_Target0;\n"
"    float4 col : SV_Target1;\n"
"    float4 nor : SV_Target2;\n"
"    float4 tan : SV_Target3;\n"
"    float4 lig : SV_Target4;\n"
"};\n\n";


  const std::string vertexShader =
"VS_OUTPUT VS( VS_INPUT input )\n"
"{\n"
"  VS_OUTPUT output;\n"
"  float4x4 gWVP;\n"
"  float4x4 gWV;\n\n"
"  gWV             = mul(World,View);\n"
"  gWVP            = mul(gWV,Projection);\n"
"  output.pos      = mul( float4(input.pos,1.0f) , gWVP);\n"
"  output.nor      = mul( float4(input.nor,0.0f), World);\n"
"  output.tan      = mul( float4(input.tan,0.0f), World);\n"
"  output.uv       = input.uv;\n"
"  output.nor.xyz  = normalize(output.nor.xyz);\n"
"  output.posLS    = float4(input.pos,1.0f);\n"
"  output.posWS    = mul( float4(input.pos,1.0f) , World);\n"
"  output.depth    = mul( float4(input.pos,1.0f), gWV ).z;\n"
"  return output;\n"
"}\n\n";

  const std::string vertexShaderDef1 =
"VS_OUTPUT VS( VS_INPUT input )\n"
"{\n"
"  VS_OUTPUT output;\n"
"  float4x4 gWVP;\n"
"  float4x4 gWV;\n\n"
"  gWV             = mul(World,View);\n"
"  gWVP            = mul(gWV,Projection);\n"
"  gWVP            = mul(World,View);\n"
"  gWVP            = mul(gWVP,Projection);\n"
"  output.pos      = mul( float4(input.pos,1.0f) , gWVP);\n"
"  output.nor      = mul( float4(input.nor,0.0f), World);\n"
"  output.nor.xyz  = normalize(output.nor.xyz);\n"
"  output.tan      = mul( float4(input.tan,0.0f), World);\n"
"  output.posLS    = float4(input.pos,1.0f);\n"
"  output.posWS    = mul( float4(input.pos,1.0f), World);\n"
"  output.depth    = mul( float4(input.pos,1.0f), gWV ).z;\n"
"  output.uv       = input.uv;\n"
"  return output;\n"
"}\n\n";

  const std::string vertexShaderDef2 =
"VS_OUTPUT VS( VS_INPUT input )\n"
"{\n"
"  VS_OUTPUT output;\n"
"  float4x4 gWVP;\n"
"  float4x4 gWV;\n\n"
"  gWV             = mul(World,View);\n"
"  gWVP            = mul(gWV,Projection);\n"
"  gWVP            = mul(World,View);\n"
"  gWVP            = mul(gWVP,Projection);\n"
"  output.pos      = mul( float4(input.pos,1.0f) , gWVP);\n"
"  output.nor      = mul( float4(input.nor,0.0f), World);\n"
"  output.nor.xyz  = normalize(output.nor.xyz);\n"
"  output.tan      = mul( float4(input.tan,0.0f), World);\n"
"  output.posLS    = float4(input.pos,1.0f);\n"
"  output.posWS    = mul( float4(input.pos,1.0f), World);\n"
"  output.depth    = mul( float4(input.pos,1.0f), gWV ).z;\n"
"  return output;\n"
"}\n\n";

  const std::string pass =
"technique10 ForwardRender\n"
"{\n"
"    pass P0\n"
"    {\n"
"        SetVertexShader( CompileShader(   vs_4_0, VS() ) );\n"
"        SetGeometryShader( NULL );\n"
"        SetPixelShader( CompileShader(    ps_4_0, PS() ) );\n"
"    }\n"
"}\n\n";

  const std::string passDef =
"technique10 DeferredRender\n"
"{\n"
"    pass P0\n"
"    {\n"
"        SetDepthStencilState(EnableDepthLess, 0);\n"
"        SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);\n"
"        SetVertexShader( CompileShader(   vs_4_0, VS() ) );\n"
"        SetGeometryShader( NULL );\n"
"        SetPixelShader( CompileShader(    ps_4_0, PS() ) );\n"
"    }\n"
"}\n\n";

  ///////////////////////////////////////////////////////////////////
  //    Pixel shader - where the beef is

const std::string pixelShaderPart1 =
"float4 PS( VS_OUTPUT input ) : SV_Target\n"
"{\n"
"  float4 finalPixel = float4(0,0,0,1);\n"
"  float3 objectScale = float3(1,1,1);\n";

const std::string pixelShaderPart1DeF =
"PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target\n"
"{\n"
"  PS_OUTPUT output = (PS_OUTPUT)0;\n"
"  float4 finalPixel = float4(0,0,0,1);\n"
"  float3 objectScale = float3(1,1,1);\n";

  const std::string pixelShaderPart2 =
"\n\n"
"  float4 diffuseColour = ";

  const std::string pixelShaderPart2a =
"\n"
"  finalPixel.rgb = diffuseColour.rgb;\n";

const std::string pixelShaderPart4a =
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb,  input.nor.xyz, float3( 2, 0, 0 ), camPos );\n"
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb,  input.nor.xyz, float3( 0, 2, 0 ), camPos );\n"
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb,  input.nor.xyz, float3( 0, 0, 2 ), camPos );\n";

const std::string pixelShaderPart4b =
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 2, 0, 0 ), camPos );\n"
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 0, 2, 0 ), camPos );\n"
"  finalPixel.rgb += lightingP( input.posWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 0, 0, 2 ), camPos );\n";

const std::string pixelShaderPart5 =
"\n"
"  return finalPixel;\n"
"}\n\n";

const std::string pixelShaderPart5Def =
"\n"
"  output.tan = input.tan;\n"
"  output.pos = input.posWS;\n"
"  output.col = float4( diffuseColour.xyz, 1);\n"
"  output.lig = ";



const std::string pixelShaderPreviousFuncLines =
"// Assuming one point of the line is (0,0,0)\n"
"float distLine3d0( float3 p, float3 l)\n"
"{\n"
"  float n  = length(cross(l,-p));\n"
"  float d  = length(l);\n"
"  return n/d;\n"
"}\n\n"
"float procLine( float3 val, float3 m, float aBaseDist )\n"
"{\n"
"  float d = distLine3d0( val, m );\n"
"  d /= aBaseDist;\n"
"  d *= 30;\n"
"  d = saturate(d);\n"
"  return d;\n"
"}\n\n";



  const std::string variables =
"\n"
"  float3  noiseParams;\n"
"  float   noiseT;\n"
"  float   value;\n"
"  float   value2;\n"
"  float3  noiseIn;\n"
"  float3  vectorModulus;\n"
"  float3  vectorValue;\n"
"  float   tFade;\n"
"  float3  inPerlin;\n"
"  float   divPerlin;\n"
"  float3  spherical;\n\n";

  const std::string variablesSpherical=
"  spherical.z = length(input.posLS);\n"
"  spherical.x = acos(input.posLS.z / spherical.z);\n"
"  spherical.y = atan(input.posLS.y / input.posLS.x);\n\n";


  ///////////////////////////////////////////////////////////////////
  //    Bump section

  const std::string bumpFunction1 =
"  float3 viewVector = normalize( input.pos - camPos);\n"
"  float3 s = normalize(input.tan);\n"
"  float3 r = normalize( cross( input.nor, s ) );\n"
"  float moduleBump = 0.01;\n"
"  float3 vec[4] = {\n"
"      float3(0,0,0),\n"
"      float3(0,0,0),\n"
"      float3(0,0,0),\n"
"      float3(0,0,0) \n"
"      };\n"
"  vec[1] += s*moduleBump;\n"
"  vec[2] += r*moduleBump;\n"
"  vec[3] += s*moduleBump + r*moduleBump;\n"
"  float res[4];      \n\n"
"  for ( int i = 0; i < 4 ; ++i)\n"
"    {\n"
"    float3 inputParameters = input.posWS + vec[i];\n";

  const std::string bumpFunction2 =
"\n"
" res[i] = ";


  const std::string bumpFunction5 =
";\n"
"  }\n"
"  float3 dF = float3( res[1] - res[0], res[2] - res[0], res[3] - res[0] );\n"
"  dF *= 1.0f/moduleBump;\n"
"  float3 disturbedNormal = input.nor.xyz + dF;\n"
"  disturbedNormal = normalize(disturbedNormal);\n";




  ///////////////////////////////////////////////////////////////////
  //    Perlin Octaves

  const std::string perlin1 =
"  inPerlin = ";

  const std::string perlin2 =
";\n"
"  divPerlin = 1;\n"
"  for( int i = 0; i < ";

  const std::string perlin3 =
"; ++i)\n"
"  {\n"
"    float r = Snoise3D( inPerlin );\n"
"    r /= divPerlin;\n"
"    ";
      const std::string perlin4 =
" += r;\n"
"    inPerlin *= 2;\n"
"    divPerlin *= ";

  const std::string perlin5 =
";\n"
"  }\n";


  ///////////////////////////////////////////////////////////////////
  //    Two color gradient

  const std::string col2Grad1 =
"lerp( ";
  const std::string col2Grad2 =
", ";
  const std::string col2Grad3 =
", ";
  const std::string col2Grad4 =
" );\n";


};  // end namespace ProcShadTextData

#endif

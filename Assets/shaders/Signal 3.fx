//  Procedural Texture Shader v0.001
//  Forward render
//  Not Gordian

#include "simplex.fx"
#include "perlin.fx"
#include "shader_defines.fx"
#include "lighting.fx"

matrix World;
matrix View;
matrix Projection;
float3 camPos;

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos   : SV_POSITION;
    float4 Nor   : NORMAL;
    float4 PosLS : POSITION1;
    float4 PosWS : POSITION2;
};

float4 bump( float4 inputParameters )
  {
  float4  noiseColor1;
  float4  noiseColor2;
  float3  ponderation; //snd
  float3  noiseParams;
  float   noiseT;
  float   value;  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  spherical;

  spherical.z = length(inputParameters);
  spherical.x = acos(inputParameters.z / spherical.z);
  spherical.y = atan(inputParameters.y / inputParameters.x);

  float4  Signal00 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal22 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal11 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal33 = float4( 0.0f, 0.0f, 0.0f, 1.0f );

  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = inputParameters.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal00 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = inputParameters.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal22 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = inputParameters.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal11 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = inputParameters.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal33 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  float4 Layer11 = Signal11;
  float4 Layer55 = Signal33;
  float4 Layer44 = 0.5*Signal11 + 0.5*Signal33;

  return Signal33;
}

VS_OUTPUT VS( VS_INPUT input )
{
  VS_OUTPUT output;
	float4x4 gWVP;
  gWVP            = mul(World,View);
  gWVP            = mul(gWVP,Projection);
  output.Pos      = mul( float4(input.pos,1.0f) , gWVP);
  output.Nor      = mul( float4(input.nor,0.0f), World);
  output.Nor.xyz  = normalize(output.Nor.xyz);
  output.PosLS    = float4(input.pos,1.0f);
  output.PosWS    = mul( float4(input.pos,1.0f) , World);
  return output;
}

float4 PS( VS_OUTPUT input ) : SV_Target
{
  float4 finalPixel = float4(0,0,0,1);
  float3 l1Pos = float3( 2, 0, 0 );
  float3 l2Pos = float3( 0, 2, 0 );
  float3 l3Pos = float3( 0, 0, 2 );
  float4 diffuseColour = float4(1,1,1,1);
  float4  noiseColor1;
  float4  noiseColor2;
  float3  ponderation; //snd
  float3  noiseParams;
  float   noiseT;
  float   value;  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  spherical;

  spherical.z = length(input.PosLS);
  spherical.x = acos(input.PosLS.z / spherical.z);
  spherical.y = atan(input.PosLS.y / input.PosLS.x);

  float4  Signal00 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal22 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal11 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal33 = float4( 0.0f, 0.0f, 0.0f, 1.0f );

  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal00 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal22 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal11 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(2.50000f, 2.50000f, 2.50000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += perlin01r( noiseIn, 1 , 1.00000f );
  noiseT = clamp( noiseT, 0, 1 );
  Signal33 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 

//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer33 = Signal22;
  float4 Layer22 = 0.5*Signal00 + 0.5*Signal22;

//BUMP CHANNEL
  float4 Layer11 = Signal11;
  float4 Layer55 = Signal33;
  float4 Layer44 = 0.5*Signal11 + 0.5*Signal33;

//FINAL PIXEL


  diffuseColour.rgb =  Layer22.rgb;
  float delta = 0.01f;
  float bump0 = bump ( input.PosLS ).x;
  float bumpx = bump ( input.PosLS +  float4(delta,0,0,0) ).x;
  float bumpy = bump ( input.PosLS +  float4(0,delta,0,0) ).x;
  float bumpz = bump ( input.PosLS +  float4(0,0,delta,0) ).x;
  float3 dF = float3( bumpx - bump0, bumpy - bump0, bumpz - bump0 );
  dF /= delta;
  float3 disturbedNormal = input.Nor.xyz - dF;
  disturbedNormal = normalize(disturbedNormal);
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, l1Pos, camPos );
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, l2Pos, camPos );
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, l3Pos, camPos );

  return finalPixel;
}

technique10 ForwardRender
{
    pass P0
    {
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );
    }
}


//  Procedural Texture Shader v0.001
//  Forward render
//  Not Gordian

#include "simplex.fx"
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

  float3  noiseParams;
  float   noiseT;
  float   value;  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  inPerlin;
  float   divPerlin;
  float3  spherical;

  spherical.z = length(input.PosLS);
  spherical.x = acos(input.PosLS.z / spherical.z);
  spherical.y = atan(input.PosLS.y / input.PosLS.x);


  noiseIn = input.PosLS.xyz*float3(2.60000f, 2.60000f, 2.60000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(2.50000f, 2.50000f, 2.50000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal00 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(0.40000f, 0.40000f, 0.40000f);
  noiseT = Snoise3D ( noiseIn);
  float4 triSignal0000 = lerp( float4(0.48235f, 0.29020f, 0.52941f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(3.00000f, 3.00000f, 3.00000f);
    inPerlin = noiseIn;
  divPerlin = 1;
  for( int i = 0; i < 2; ++i)
  {
    float r = Snoise3D( inPerlin );
    r /= divPerlin;
    noiseT += r;
    inPerlin *= 2;
    divPerlin *= 1;
  }
  float4 triSignal2222 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 0.54902f, 0.91765f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = triSignal0000;
  float4 Layer33 = triSignal2222;
  float4 Layer22;
  if ( triSignal0000.x < 0.5 )
  {
    Layer22.x = 2*triSignal0000.x*triSignal2222.x;
  }
  else
  {
    Layer22.x = 1- 2*(1 - triSignal0000.x)*(1 - triSignal2222.x);
  }
;
  if ( triSignal0000.y < 0.5 )
  {
    Layer22.y = 2*triSignal0000.y*triSignal2222.y;
  }
  else
  {
    Layer22.y = 1- 2*(1 - triSignal0000.y)*(1 - triSignal2222.y);
  }
;
  if ( triSignal0000.z < 0.5 )
  {
    Layer22.z = 2*triSignal0000.z*triSignal2222.z;
  }
  else
  {
    Layer22.z = 1- 2*(1 - triSignal0000.z)*(1 - triSignal2222.z);
  }
;

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL


  float4 diffuseColour = triSignal2222;
  finalPixel.rgb = Layer33;

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


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

  noiseIn = input.PosLS.xyz*float3(0.20000f, 0.20000f, 0.20000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal00 = lerp( float4(0.56078f, 0.36471f, 0.85098f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(3.00000f, 3.00000f, 3.00000f);
  noiseT = 0;
  vectorValue = noiseIn * float3(6.00000f, 3.00000f, 1.49254f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 0.00000f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.90000f)
    noiseT += 1 - ( (value/0.90000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.42353f, 0.87843f, 1.00000f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer33 = Signal22;
  float4 Layer22 = 0.5*Signal00 + 0.5*Signal22;

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL


  float4 diffuseColour = Signal22;
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


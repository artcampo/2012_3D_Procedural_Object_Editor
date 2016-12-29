//  Procedural Texture Shader v0.001
//  Forward render
//  Not Gordian

#include "simplex.fx"

matrix World;
matrix View;
matrix Projection;

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
};

VS_OUTPUT VS( VS_INPUT input )
{
  VS_OUTPUT output;
	float4x4 gWVP;
  gWVP            = mul(World,View);
  gWVP            = mul(gWVP,Projection);
  output.Pos 	   = mul ( float4(input.pos,1.0f) , gWVP);
  output.Nor      = mul( float4(input.nor,0.0f), World);
  output.Nor.xyz  = normalize(output.Nor.xyz);
  output.PosLS    = float4(input.pos,1.0f);
  return output;
}

float4 PS( VS_OUTPUT input ) : SV_Target
{
  float4 finalPixel;
  float4  baseColor = float4(1.00000f, 0.02745f, 0.02745f, 1.00000f);
  float   freq = 1.00000f;
  float4  noiseColor = float4(1.00000f, 0.07843f, 0.07843f, 1.00000f);
  float3  posw = input.PosLS.xyz;
  float c0  = Snoise3D ( posw*freq );
  float c1  = Snoise3D ( posw*(freq*0.5f) );
  float c2  = Snoise3D ( posw*(freq*0.25f) );
  finalPixel = baseColor;
  finalPixel += c0*noiseColor;
  finalPixel += c1*noiseColor;
  finalPixel += c2*noiseColor;
  return baseColor;
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


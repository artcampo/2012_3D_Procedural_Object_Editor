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
  output.Pos      = mul( float4(input.pos,1.0f) , gWVP);
  output.Nor      = mul( float4(input.nor,0.0f), World);
  output.Nor.xyz  = normalize(output.Nor.xyz);
  output.PosLS    = float4(input.pos,1.0f);
  return output;
}

float4 PS( VS_OUTPUT input ) : SV_Target
{
  float4 finalPixel;

  float4  noiseColor1;
  float4  noiseColor2;
  float3  ponderation; //snd
  float3  noiseParams;
  float   noiseT;
  float value;  float3  noiseIn;
  float4  Signal00 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal11 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal22 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal33 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
  float4  Signal44 = float4( 0.0f, 0.0f, 0.0f, 1.0f );

  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(1.00000f, 1.00000f, 1.00000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += Snoise3D( 1.00000f * noiseIn + (0 *0.1* float3(1,0,0))* Snoise3D (noiseIn)  );
  noiseT = clamp( noiseT, 0, 1 );
  Signal00 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(0.00000f, 1.00000f, 1.00000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += Snoise3D( 1.00000f * noiseIn + (0 *0.1* float3(1,0,0))* Snoise3D (noiseIn)  );
  noiseT = clamp( noiseT, 0, 1 );
  Signal11 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(1.00000f, 1.00000f, 1.00000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += Snoise3D( 1.00000f * noiseIn + (0 *0.1* float3(1,0,0))* Snoise3D (noiseIn)  );
  noiseT = clamp( noiseT, 0, 1 );
  Signal22 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(1.00000f, 1.00000f, 1.00000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += Snoise3D( 1.00000f * noiseIn + (0 *0.1* float3(1,0,0))* Snoise3D (noiseIn)  );
  noiseT = clamp( noiseT, 0, 1 );
  Signal33 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  noiseColor1 = float4(0.00000f, 0.00000f, 0.00000f, 1.00000f);
  noiseColor2 = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  ponderation = float3(1.00000f, 1.00000f, 1.00000f);
  noiseParams = input.PosLS.xyz;
  noiseIn = noiseParams*ponderation;
  noiseT = 0;
  noiseT += Snoise3D( 1.00000f * noiseIn + (0 *0.1* float3(1,0,0))* Snoise3D (noiseIn)  );
  noiseT = clamp( noiseT, 0, 1 );
  Signal44 .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );
 
  float4 Layer77 = 0.5*Signal33 + 0.5*Signal44;
  float4 Layer55 = 0.5*Signal22 + 0.5*Layer77;
  float4 Layer33 = 0.5*Signal11 + 0.5*Layer55;
  float4 Layer11 = 0.5*Signal00 + 0.5*Layer33;


  return Signal44;
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


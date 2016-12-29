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


RasterizerState CullBack
{
    CULLMODE = 2;
};


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


  noiseIn = input.PosLS.xyz*float3(1.60000f, 1.60000f, 1.60000f);
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.81176f, 0.20000f, 0.85490f, 1.00000f), float4(0.14510f, 0.13333f, 0.16863f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(2.20000f, 2.20000f, 2.20000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.17255f, 0.17255f, 0.17255f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(0.80000f, 1.80000f, 0.30000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;

//BUMP CHANNEL
  float4 Layer11 = Signal11;
  float4 Layer33 = Signal22;
  float4 Layer22 = 0.5*Signal11 + 0.5*Signal22;

//FINAL PIXEL


  float4 diffuseColour = Layer00;
  float3 vec[4] = {
      float3(0,0,0),  
      float3(0.01,0,0),
      float3(0,0.01,0),
      float3(0,0,0.01)    
      };
  float res[4];      

  for ( int i = 0; i < 4 ; ++i)
    {
    float3 inputParameters = input.PosWS + vec[i];

  noiseIn = inputParameters.xyz*float3(1.60000f, 1.60000f, 1.60000f);
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.81176f, 0.20000f, 0.85490f, 1.00000f), float4(0.14510f, 0.13333f, 0.16863f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*float3(2.20000f, 2.20000f, 2.20000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.17255f, 0.17255f, 0.17255f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*float3(0.80000f, 1.80000f, 0.30000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  float4 Layer11 = Signal11;
  float4 Layer33 = Signal22;
  float4 Layer22 = 0.5*Signal11 + 0.5*Signal22;

 res[i] = Layer22;
  }
  float3 dF = float3( res[1] - res[0], res[2] - res[0], res[3] - res[0] );
  dF /= 0.01;
  float3 disturbedNormal = input.Nor.xyz + dF;
  disturbedNormal = normalize(disturbedNormal);
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 2, 0, 0 ), camPos );
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 0, 2, 0 ), camPos );
  finalPixel.rgb += lightingP( input.PosWS.xyz, diffuseColour.rgb, disturbedNormal, float3( 0, 0, 2 ), camPos );

  return finalPixel;
}

technique10 ForwardRender
{
    pass P0
    {
        SetRasterizerState(CullBack);
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );
    }
}


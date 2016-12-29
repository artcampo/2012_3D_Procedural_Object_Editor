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
  float4 testcuSignal0000 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(2.50000f, 2.50000f, 2.50000f);
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 testcuSignal2222 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.65490f, 0.88235f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(1.70000f, 1.70000f, 1.70000f);
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 eqrtertSignal3333 = lerp( float4(0.29412f, 0.36471f, 0.34902f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(2.50000f, 2.50000f, 2.50000f);
  noiseT = 0;
  vectorValue = noiseIn * float3(3.33333f, 3.33333f, 3.33333f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 0.00000f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.30000f)
    noiseT += 1 - ( (value/0.30000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 eqrtertSignal0000 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.38824f, 0.14902f, 1.00000f, 1.00000f), noiseT );

  noiseIn = input.PosLS.xyz*float3(2.50000f, 2.50000f, 2.50000f);
  noiseT = Snoise3D ( noiseIn);
  float4 eqrtertSignal2222 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.84314f, 0.66275f, 1.00000f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = eqrtertSignal0000;
  float4 Layer33 = eqrtertSignal2222;
  float4 Layer55 = eqrtertSignal3333;
  float4 Layer44 = 0.195*eqrtertSignal2222 + 0.805*eqrtertSignal3333;
  float4 Layer22 = float4( 0.5, 0.5, 0.5, 1 ) - 2*(0.7475*eqrtertSignal0000 - float4( 0.5, 0.5, 0.5, 1 )) * (0.2525*Layer44 - float4( 0.5, 0.5, 0.5, 1 ));

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL


  float4 diffuseColour = eqrtertSignal2222;
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


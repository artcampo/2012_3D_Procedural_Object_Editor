#include "lighting.fx"
#include "shader_defines.fx"


matrix World;
matrix View;
matrix Projection;

float3    camPos;


struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
    float4 colour  : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
    float3 nor : NORMAL;
    float3 posw : POSITION;
    float4 colour  : COLOR;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;
    //Object to world

	  float4x4 gWVP;
	  gWVP = mul(World,View);
	  gWVP = mul(gWVP,Projection);
	  float4x4 gWV;
	  gWV = mul(World,View);

    output.Pos 	= mul ( float4(input.pos,1.0f) , gWVP);
    output.colour = input.colour;
    output.nor = mul ( float4(input.nor,0.0f) , gWVP).xyz;
    output.posw = mul ( float4(input.pos,1.0f) , World).xyz;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS( VS_OUTPUT input ) : SV_Target
{

  float4 col;
  col.xyz  = input.colour.rgb;
  col.xyz += lightingP( input.posw, input.colour.rgb, input.nor, float3( 1, 0, 0 ), camPos ) * 0.05;
  col.xyz += lightingP( input.posw, input.colour.rgb, input.nor, float3( 0, 1, 0 ), camPos ) * 0.05 ;
  col.a = 1;
  return col;
}

//--------------------------------------------------------------------------------------
technique10 FWDC
{

    pass P0
    {
        SetRasterizerState(NoCull);
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState(EnableDepthLess, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

        SetRasterizerState(rs_Back);

    }
}

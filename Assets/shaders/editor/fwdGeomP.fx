#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;

float4 colour;


struct VS_INPUT
{
    float3 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;

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

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS( VS_OUTPUT input ) : SV_Target
{
  float4 col = colour;
  return col;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P0
    {
        SetRasterizerState(NoCull);
        SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState(DisableDepthLess, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

//        SetRasterizerState(rs_Back);
//        SetDepthStencilState (EnableDepthLess, 0 );        
    }
}

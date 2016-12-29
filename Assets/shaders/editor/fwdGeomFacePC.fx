#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;


struct VS_INPUT
{
    float3 pos : POSITION;
    float4 colour  : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
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

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS( VS_OUTPUT input ) : SV_Target
{
  float4 col = input.colour;
  return col;
}

//--------------------------------------------------------------------------------------
technique10 FWDC
{

    pass P0
    {
        SetRasterizerState(CullBack);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );

    }
}

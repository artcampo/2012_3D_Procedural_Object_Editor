#include "simplex.fx"

matrix testmat;
matrix World;
matrix View;
matrix Projection;

DepthStencilState EnableDepthLess
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
    float4 Nor		: NORMAL;
    float4 PosV		: POSITION;
    float4 PosO		: POSITION1;
    float depth   : FOG;
};


struct PS_OUTPUT
{
	float4 pos : SV_Target0;
	float4 col : SV_Target1;
	float4 nor : SV_Target2;
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

	  output.PosV	= mul ( float4(input.pos,1.0f) , World);

//output.Nor = float4(input.nor,1.0);

    	output.Nor = mul( float4(input.nor,0.0f), World);
    	output.Nor.xyz = normalize(output.Nor.xyz);
	output.Nor.a = 1.0f;


    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	//float4 color = float4(1.0, 0.0f, 0.0f, 1.0f);

	float3 posw = input.PosO.xyz;
	posw *= 0.002f;
	float c = Snoise3D ( posw);

	posw = input.PosO.xyz;
	posw *= 0.003f;
	float c1 = Snoise3D ( posw);

	posw = input.PosO.xyz;
	posw *= 0.005f;
	float c2 = Snoise3D ( posw);

	float4 color = float4( 0.5f, 0.2f, 0.4f, 1.0f); 
	color += c* float4( 0.2f, 0.3f, 0.5f, 1.0f) + c1*float4( 0.4f, 0.4f, 0.4f, 1.0f);
	color += c2*float4( 0.3f, 0.7f, 0.2f, 1.0f) ;	

	color.a = 1.0f;
  color = float4 ( 0.3, 0.3, 0.4, 1.0 );

	output.col = color;
	output.nor = input.Nor;
  output.pos = input.PosV;

    return output;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P1
    {
        SetDepthStencilState(EnableDepthLess, 0);
        SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

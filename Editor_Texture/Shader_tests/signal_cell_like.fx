#include "simplex.fx"

matrix World;
matrix View;
matrix Projection;

RasterizerState rs_Front
{
CullMode=3;
};

RasterizerState rs_Back
{
CullMode=2;
};

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
    float4 Tan		: TANGENT;
    float4 PosO		: POSITION;
    float depth   : FOG;
};


struct PS_OUTPUT
{
	float4 pos : SV_Target0;
	float4 col : SV_Target1;
	float4 nor : SV_Target2;
	float4 tan : SV_Target3;
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

	  output.PosO	= mul ( float4(input.pos,1.0f) , World);

	  output.Nor	= mul ( float4(input.nor,0.0f) , World);
	  output.Tan	= mul ( float4(0,1,0,0) , World);


    return output;
}

// Assuming one point of the line is (0,0,0)
float distLine3d0( float3 p, float3 l)
{
/*
  float3 pl = p-l;
  float3 n  = length(cross(p,pl));
  float3 d  = length(l);
  */
  float3 n  = length(cross(l,-p));
  float3 d  = length(l);  
  return n/d;
}

float procLine( float3 val, float3 m, float aBaseDist )
{
  float d = distLine3d0( val, m );
  d /= aBaseDist;
  d *= 10;
  return d;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;
  float3  noiseParams;
  float   noiseT;
  float   value;  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  inPerlin;
  float   divPerlin;
  float3  spherical;	

	//float4 color = float4(1.0, 0.0f, 0.0f, 1.0f);

	float3 posw = input.PosO.xyz;
	
  noiseIn = input.PosO.xyz;
	
	float base = 0.6;
	float baseDist = base*sqrt(2);
	float3 vectorModulusCoord1 = float3(base, 0, base);  
  float3 vectorModulusCoord2 = float3(-base, 0, base);  
  float3 vectorModulusCoord3 = float3(base, 0, 0);  
  vectorModulus = float3(base, base, base);  
  noiseT = 0;

  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*vectorModulus;
  vectorValue.y = 0;
  //noiseT = distLine3d0( vectorValue, vectorModulusCoord1 );
  noiseT =  procLine( vectorValue, vectorModulusCoord1, baseDist );
  noiseT *=  procLine( vectorValue, vectorModulusCoord2, baseDist );
  noiseT *=  procLine( vectorValue, vectorModulusCoord3, baseDist );
  noiseT = 1 - noiseT;

  float4 Signal00 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );
  //float4 Signal00 = float4(1.0, 0.0f, 0.0f, 1.0f)*noiseT;

	output.col = Signal00;
	output.nor = input.Nor;
	output.tan = input.Tan;
  output.pos = input.PosO;

    return output;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P0
    {
            SetRasterizerState(rs_Front);
        SetDepthStencilState(EnableDepthLess, 0);
        SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        SetRasterizerState(rs_Back);
    }
}

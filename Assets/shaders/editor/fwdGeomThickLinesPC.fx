#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;
matrix invView;
float3 camPos;


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------


struct VS_IN
{
	float3 centerW : POSITION;
  float4 colour  : COLOR;

};

struct VS_OUT
{
	float3 centerW : POSITION;
  float4 colour  : COLOR;
};

struct GS_OUT
{
    float4 posH    : SV_POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
    uint primID    : SV_PrimitiveID;
  float4 colour  : COLOR;    
};

 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Just pass data into geometry shader stage.
	vOut.centerW  = vIn.centerW;
	vOut.colour   = vIn.colour;


	return vOut;
}


[maxvertexcount(6)]
void GS(line VS_OUT gIn[2], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GS_OUT> triStream)
{	
  matrix gViewProj; 
  gViewProj = mul( View, Projection);
  float3 direction = gIn[1].centerW - gIn[0].centerW;
  float3 mean = (gIn[1].centerW + gIn[0].centerW) * 0.5f;


  float halfHeight = length(direction)*0.5f;
  float halfWidth = 0.01;


	float4 v[4];

	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);

	float2 texC[4];


	texC[0] = float2(0.0f, 0.0f);
	texC[1] = float2(1.0f, 0.0f);
	texC[2] = float2(0.0f, 1.0f);
	texC[3] = float2(1.0f, 1.0f);

  //  Transformation matrix

//	float3 up = float3(0.0f, 1.0f, 0.0f);

	float3 up = direction;
//	up.y = 0;
	up = 	normalize(up);
	

	float3 look = camPos - mean;
//	look.y = 0.0f; // y-axis aligned, so project to xz-plane
	look = normalize(look);
	float3 right = cross(look, up);

	float4x4 W;
	W[0] = float4(right,              0.0f);
	W[1] = float4(up,                 0.0f);
	W[2] = float4(look,               0.0f);
	W[3] = float4( mean.xyz,1.0f);
	float4x4 WVP = mul(W,gViewProj);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GS_OUT gOut;
	[unroll]
	for(int i = 0; i < 4; ++i)
	{

		gOut.posH     = mul(v[i], WVP);
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID; 
	  gOut.colour    = gIn[0].colour;
		
		triStream.Append(gOut);
	}
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(GS_OUT pIn) : SV_Target
{

  float4 col = pIn.colour;
  col.a = 1;
  return col;
}
 

//--------------------------------------------------------------------------------------
technique10 FWDC
{

    pass P0
    {
        SetRasterizerState(NoCull);
        
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );        
        
        SetRasterizerState(rs_Back);
    }
}

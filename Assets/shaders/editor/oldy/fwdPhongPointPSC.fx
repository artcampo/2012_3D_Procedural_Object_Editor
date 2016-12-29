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

RasterizerState NoCull
{
    CULLMODE = 2;
};

cbuffer cbPerFrame
{
	float3    gEyePosW;
	float3	  camUp;
	float4x4  gViewProj; 
	float4x4  View; 
	float4x4  invView; 
	float4x4  invProj; 
	float4x4  gInvViewProj; 
};
 

struct VS_IN
{
	float3 centerW : POSITION;
  float2 sizeW   : SIZE;
  float4 colour  : COLOR;

};

struct VS_OUT
{
	float3 centerW : POSITION;
  float2 sizeW   : SIZE;
  float4 colour  : COLOR;

};

struct GS_OUT
{
    float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
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
	vOut.sizeW  = vIn.sizeW;
	vOut.colour  = vIn.colour;


	return vOut;
}

[maxvertexcount(4)]
void GS(point VS_OUT gIn[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GS_OUT> triStream)
{	
	//
	// Compute 4 triangle strip vertices (quad) in local space.
	// The quad faces down the +z axis in local space.
	//

  float4 unit;

//  unit	= mul ( unit , gInvViewProj);
 // unit	= mul ( unit , invView);
/*
	float x = 8.0f*gIn[0].sizeW - 1;
	float y = 1 - 8.0f*gIn[0].sizeW;

	float siz = 4.1f;
	float x = (4.0/590.0f) - 1;
	float y = 1 - (4.0/314.0f);
	unit = float4(  x, 
			y,
			0.5,
			1);
//	unit	= mul ( unit , gInvViewProj);
//	unit	= mul ( unit , invView);
//	unit	= mul ( unit , invProj);

	float halfWidth  = unit.x;
	float halfHeight = unit.y;
*/
	float halfWidth  = gIn[0].sizeW.x;
	float halfHeight = gIn[0].sizeW.y;

	
	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	//
	// Compute texture coordinates to stretch texture over quad.
	//
	float2 texC[4];
	texC[0] = float2(0.0f, 1.0f);
	texC[1] = float2(1.0f, 1.0f);
	texC[2] = float2(0.0f, 0.0f);
	texC[3] = float2(1.0f, 0.0f);
	//
	// Compute world matrix so that billboard is aligned with
	// the y-axis and faces the camera.
	//


	float3 up = camUp;
	float3 look = gEyePosW - gIn[0].centerW;
//	look.y = 0.0f; // y-axis aligned, so project to xz-plane
	look = normalize(look);
	float3 right = cross(up, look);
	
	float4x4 W;
	W[0] = float4(right,              0.0f);
	W[1] = float4(up,                 0.0f);
	W[2] = float4(look,               0.0f);
	W[3] = float4(gIn[0].centerW, 1.0f);

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
		gOut.posW     = mul(v[i], W);
		gOut.normalW  = look;
		gOut.texC     = texC[i];
		gOut.primID   = primID;
		gOut.colour   = gIn[0].colour;
		
		triStream.Append(gOut);
	}
}

float4 PS(GS_OUT pIn) : SV_Target
{
	// Get materials from texture maps.
	float cx = (pIn.texC.x ) - 0.5f;
	float cy = (pIn.texC.y )- 0.5f;
	cx = cx*cx;
	cy = cy*cy;
	float c = 0.5f - sqrt ( cx + cy );
//	c += 0.2f;
	c = sqrt(c);
	c = clamp(c, 0.0f, 1.0f);
	float4 diffuse = float4 ( pIn.colour.xyz, c );

//float4 diffuse = float4 ( c, c, c, 0.2 );
    
    return diffuse;
}
 
technique10 ParticleTest
{
    pass P0
    {
        SetRasterizerState(NoCull);
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState(EnableDepthLess, 0);

        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );

    }
}

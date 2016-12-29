
DepthStencilState EnableDepthLess
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

DepthStencilState DisableDepthLess
{
    DepthEnable = FALSE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

DepthStencilState StencilState_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;
};


BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};


BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};


RasterizerState rs_noCull
{
CullMode=None;
};

RasterizerState rs_Back
{
CullMode=3;
};


RasterizerState CullBack
{
CullMode=3;
};




RasterizerState NoCull
{
    CULLMODE = 1;
};


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

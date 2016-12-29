
Texture2D textPos;
Texture2D textNor;
Texture2D textCol;
Texture2D textLig;

Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Input
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;              
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
    VS_Output Output;
    Output.Pos = Input.Pos;
    Output.Tex = Input.Tex;

    return Output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    
    const float sampleRadius  = 6.0;
    const float depthBias     = 0.1;
    const float singularity   = 6.0;
    const float strength      = 2.0;
    const int   patternSize   = 8;
    static const float PI = 3.14159265f;


    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
    float2 vSamplePosition;
    
    vSamplePosition = Input.Tex;

    float4 pos      = textPos.Sample( samLinear, vSamplePosition);
    float3 normal1  = textNor.Sample( samLinear, vSamplePosition).xyz;


    float3 N      = normalize( normal1 );
//    float3 VBasis = normalize(float3(0.3561,0.4512,0.67));
    float3 VBasis = normalize(float3(0.3,0.3,0.3));
    float3 Tan    = normalize(cross(N, VBasis));
    float3 BiTan  = normalize(cross(N, Tan));

/*
    float3x3  matLocal = float3x3 ( N.x, N.y, N.z, 
                                    Tan.x, Tan.y, Tan.z, 
                                    BiTan.x,BiTan.y,BiTan.z );
*/
    float3x3  matLocal = float3x3 ( 1.0, 0.0, 0.0, 
                                    0.0, 1.0, 0.0, 
                                    0.0,0.0, 1.0 );

    float     ambientOcclusion    = 0.0;
    float3    directRadianceSum   = float3 ( 0.0, 0.0, 0.0 );
    float3    occRadianceSum      = float3 ( 0.0, 0.0, 0.0 );
    


    int patternIndex = int(vSamplePosition.x*1024) % patternSize +
                      (int(vSamplePosition.y*748) % patternSize) * patternSize;

    float numOccludedSamples = 0.0;

    for ( int i = 0; i < 8 ; ++i )
      {
      //float3 sample = offsets[i];
      float3 sample = textSeed.Sample( samLinear, float2( i, patternIndex ) );
      sample = float3 (2.0 ,3.0,0.0);

      sample = mul ( sample, matLocal );
      float3 normSample = normalize(sample);

      float4 worldSamOccPos = pos;
      worldSamOccPos +=sampleRadius * float4( sample.xyz, 0.0 );
      float4 occSamPos; 
      occSamPos= mul ( float4(worldSamOccPos.xyz, 1.0) , (float4x4)ModelViewProj );



      float2 occTexCord = ( occSamPos.x / occSamPos.w, -occSamPos.y / occSamPos.w );
      occTexCord = (occTexCord + float2( 1.0, 1.0 ) ) * 0.5;
      //occTexCord = occTexCord * float2( textPosWidth, textPosHeight );

      float4 occPos = textPos.Sample( samLinear, occTexCord );
      float3 occNor = textNor.Sample( samLinear, occTexCord ).xyz;

      float depth     = (mul ( worldSamOccPos , ModelView )).z;
      float depthSam  = (mul ( occPos , ModelView )).z + depthBias;

      float distanceTerm;
      if ( abs( depth - depthSam ) < singularity )
        distanceTerm = 1.0;
      else
        distanceTerm = 0.0;


      float visibility = 1.0 - strength * ( depthSam > depth ? 1.0 : 0.0 ) * distanceTerm;
      numOccludedSamples +=  ( depthSam > depth ? 1.0 : 0.0 ) ;

      float recGeometricTerm = max( 0.0, dot( normSample, normal1 ));

//      float3 radiance = float3(1.0, 1.0, 1.0 ) * visibility * recGeometricTerm;
      float3 radiance = float3(0.5, 0.5, 0.5 ) * visibility * recGeometricTerm;

      directRadianceSum += radiance;

      //Compute occluder radiance
      float3 directRadiance = textCol.Sample( samLinear, occTexCord);

      float3 delta = pos.xyz - occPos.xyz;
      float3 normDelta = normalize ( delta );

      float unclampedBounceGeometricTerm = 
            max ( 0.0, dot ( normDelta , - normal1 ))*
            max ( 0.0, dot ( normDelta , occNor )) /
            dot ( delta, delta );

      float3 occRadiance = directRadiance * unclampedBounceGeometricTerm;
      occRadianceSum += occRadiance;

      }

    directRadianceSum = max( directRadianceSum, float3 ( 0.0, 0.0, 0.0 ) );
//directRadianceSum = float3 ( 0.0, 0.0, 0.0 );
    occRadianceSum    = max( occRadianceSum, float3 ( 0.0, 0.0, 0.0 ) );

    float3 radiance = directRadianceSum + occRadianceSum;

//radiance = * 2.0 * PI / 8);

radiance = radiance * (2.0/PI/16);
    // output ao
    //vColor = float4 ( radiance.x, radiance.y, radiance.z, 1.0 );



    float4 vertexPos = textPos.Sample( samLinear, vSamplePosition);    
    float4 vertexLig = textLig.Sample( samLinear, vSamplePosition);    
    float4 vertexNor = textLig.Sample( samLinear, vSamplePosition);    


    vColor.xyz = textCol.Sample( samLinear, vSamplePosition);    
    vColor.xyz += vertexLig.xyz;

vColor.a = 1.0;

    /////////////////////////////////////////////
    ///       Debug

    ///////////////////
    //  Display num of occluded samples
    //numOccludedSamples /= 8.0;
//   vColor = float4 ( numOccludedSamples, numOccludedSamples, numOccludedSamples, 1.0 );

    // Dispatch non-geometry pixels
    //if ( pos.a < 0.0 )
    //  vColor = float4 ( 1.0, 1.0, 1.0, 1.0 );

    return vColor;

}

//--------------------------------------------------------------------------------------
technique10 SSDO
{

    pass P1
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}

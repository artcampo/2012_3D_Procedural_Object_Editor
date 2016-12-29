Texture2D<uint>   PermTexture;   // permutation texture

// Find which simplex we are in by magnitude sorting

void Simplex3D( const in float3 P, out float3 simplex[4] )
{
    float3 T = P.xzy >= P.yxz;
    simplex[0] = 0;
    simplex[1] = T.xzy > T.yxz;
    simplex[2] = T.yxz <= T.xzy;
    simplex[3] = 1;
}

void Simplex4D( const in float4 P, out float4 simplex[5] )
{
    float4 offset0;

    float3 isX = step( P.yzw, P.xxx );        // See comments in 3D simplex function
    offset0.x = dot( isX, float3(1, 1, 1) );
    offset0.yzw = 1 - isX;

    float2 isY = step( P.zw, P.yy );
    offset0.y += dot( isY, float2(1, 1) );
    offset0.zw += 1 - isY;

    float isZ = step( P.w, P.z );
    offset0.z += isZ;
    offset0.w += 1 - isZ;

    // offset0 now contains the unique values 0,1,2,3 in each channel

    simplex[4] = 1;
    simplex[3] = saturate (   offset0);
    simplex[2] = saturate (-- offset0);
    simplex[1] = saturate (-- offset0);
    simplex[0] = 0;
}

// Hashing functions

int Hash( float3 P )
{
    //P /= 256;   // normalize texture coordinate
    //return PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r ^ PermTexture.SampleLevel( SamplerRepeat, float2( P.z, 0 ), 0 ).r;
	return PermTexture.Load( int3(P.xy, 0) ).r ^ PermTexture.Load( int3( P.z, 0, 0 ) ).r;
}

int Hash( float4 P )
{
    //P /= 256;   // normalize texture coordinate
    //return PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r ^ PermTexture.SampleLevel( SamplerRepeat, P.zw, 0 ).r;
	return PermTexture.Load( int3(P.xy, 0) ).r ^ PermTexture.Load( int3(P.zw, 0) ).r;

    //return PermTexture.SampleLevel( SamplerRepeat,
    //    float2(PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r, P.z) + P.w, 0 ).r;
}

// gradient tables

cbuffer Gradients
{
    // 3D gradients
    
    const float3 Grad3[] = {
        float3( -1, -1, 0 ), // 0
        float3( -1, +1, 0 ), // 1
        float3( +1, -1, 0 ), // 2
        float3( +1, +1, 0 ), // 3

        float3( -1, 0, -1 ), // 4
        float3( -1, 0, +1 ), // 5
        float3( +1, 0, -1 ), // 6
        float3( +1, 0, +1 ), // 7

        float3( 0, -1, -1 ), // 8
        float3( 0, -1, +1 ), // 9
        float3( 0, +1, -1 ), // 10
        float3( 0, +1, +1 ), // 11

        // padding   
        float3( +1, +1, 0 ), // 12
        float3( -1, -1, 0 ), // 13
        float3( 0, -1, +1 ), // 14
        float3( 0, -1, -1 )  // 15
    };

    // 4D case is more regular

    const float4 Grad4[] = {
        // x, y, z
        float4( -1, -1, -1, 0 ), // 0
        float4( -1, -1, +1, 0 ), // 1
        float4( -1, +1, -1, 0 ), // 2
        float4( -1, +1, +1, 0 ), // 3
        float4( +1, -1, -1, 0 ), // 4
        float4( +1, -1, +1, 0 ), // 5
        float4( +1, +1, -1, 0 ), // 6
        float4( +1, +1, +1, 0 ), // 7
        // w, x, y
        float4( -1, -1, 0, -1 ), // 8
        float4( -1, +1, 0, -1 ), // 9
        float4( +1, -1, 0, -1 ), // 10
        float4( +1, +1, 0, -1 ), // 11
        float4( -1, -1, 0, +1 ), // 12
        float4( -1, +1, 0, +1 ), // 13
        float4( +1, -1, 0, +1 ), // 14
        float4( +1, +1, 0, +1 ), // 15
        // z, w, x
        float4( -1, 0, -1, -1 ), // 16
        float4( +1, 0, -1, -1 ), // 17
        float4( -1, 0, -1, +1 ), // 18
        float4( +1, 0, -1, +1 ), // 19
        float4( -1, 0, +1, -1 ), // 20
        float4( +1, 0, +1, -1 ), // 21
        float4( -1, 0, +1, +1 ), // 22
        float4( +1, 0, +1, +1 ), // 23
        // y, z, w
        float4( 0, -1, -1, -1 ), // 24
        float4( 0, -1, -1, +1 ), // 25
        float4( 0, -1, +1, -1 ), // 26
        float4( 0, -1, +1, +1 ), // 27
        float4( 0, +1, -1, -1 ), // 28
        float4( 0, +1, -1, +1 ), // 29
        float4( 0, +1, +1, -1 ), // 30
        float4( 0, +1, +1, +1 ) // 31
    };
};

// Basic 3D simplex noise

#define F3 0.333333333333
#define G3 0.166666666667

float Snoise3D( float3 P )
{
    // Skew the (x,y,z) space to determine which cell of 6 simplices we're in

    float s = dot( P, F3 );
    float3 Pi = floor( P + s );
    float t = dot( Pi, G3 );

    float3 P0 = Pi - t;               // Unskew the cell origin back to (x,y,z) space
    float3 Pf0 = P - P0;            // The x,y,z distances from the cell origin

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // To find out which of the six possible tetrahedra we're in, we need to
    // determine the magnitude ordering of x, y and z components of Pf0.

    float3 simplex[4];
    Simplex3D( Pf0, simplex );

    float n = 0;

    [unroll]
    for (int i = 0; i<4; i++)
    {
        float3 Pf = Pf0 - simplex[i] + G3 * i;
        int h = Hash( Pi + simplex[i] );
        float d = saturate( 0.6f - dot( Pf, Pf ) );
        d *= d; 
        n += d * d * dot(Grad3[ h & 31 ], Pf);
        //n += d * d * grad( Pf, h );
    }

    return 32.0f * n;
}

// Simplex flow noise - rotate base gradient vectors

float Snoise3DFlow( float4 P )
{
    // Skew the (x,y,z) space to determine which cell of 6 simplices we're in

    float s = dot( P.xyz, F3 );
    float3 Pi = floor( P.xyz + s );
    float t = dot( Pi, G3 );

    float3 P0 = Pi - t;               // Unskew the cell origin back to (x,y,z) space
    float3 Pf0 = P.xyz - P0;         // The x,y,z distances from the cell origin

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // To find out which of the six possible tetrahedra we're in, we need to
    // determine the magnitude ordering of x, y and z components of Pf0.

    float3 simplex[4];
    Simplex3D( Pf0, simplex );

    float n = 0;
    float sine, cosine;
    sincos( P.w, sine, cosine );

    [unroll]
    for (int i = 0; i<4; i++)
    {
        float3 Pf = Pf0 - simplex[i] + G3 * i;
        int h = Hash( Pi + simplex[i] );
        float d = saturate( 0.6f - dot( Pf, Pf ) );
        d *= d;
        float3 g = Grad3[ h & 31 ];
        g.xz = float2(g.x * cosine - g.z * sine, g.x * sine + g.z * cosine );   // rotate gradient vector
        n += d * d * dot( g, Pf );
    }

    return 32.0f * n;
}

// 4D simplex noise

#define F4 0.309016994375
#define G4 0.138196601125

float Snoise4D( float4 P )
{
    // Skew the (x,y,z) space to determine which cell of 6 simplices we're in

    float s = dot( P, F4 );
    float4 Pi = floor( P + s );
    float t = dot( Pi, G4 );

    float4 P0 = Pi - t;
    float4 Pf0 = P - P0;

    // For the 4D case, the simplex shape is a slightly irregular tetrahedron.
    // To find out which of the 24 possible tetrahedra we're in, we need to
    // determine the magnitude ordering of x, y, z and w components of Pf0.

    float4 simplex[5];
    Simplex4D( Pf0, simplex );

    float n = 0;

    [unroll]
    for ( int i = 0; i<5; i++ )
    {
        float4 Pf = Pf0 - simplex[i] + G4 * i;
        int h = Hash( Pi + simplex[i] );
        float d = saturate( 0.6f - dot( Pf, Pf ) );
        d *= d;
        n += d * d * dot( Grad4[ h & 31 ], Pf );
    }

    // Sum up and scale to fit [-1..1] range

    return 32.0f * n;   // FIXME: should this constant be 27.0f?
}

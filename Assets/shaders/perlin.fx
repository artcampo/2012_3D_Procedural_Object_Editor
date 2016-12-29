float perlin ( const in float3 vectInput, 
                  const in int nOctaves, 
                  const in float roughness )
{
  float result = 0;
  float3 inn = vectInput;
  float div = 1;
  for( int i = 0; i < nOctaves ; ++i)
    {
      float r = (Snoise3D( inn  ));
      r /= div;      
      result += r;      
      inn *= 1.4;
      div *= 2.4;      
    }
  return result;  
}

float perlin1 ( const in float3 vectInput)
{
  return Snoise3D( vectInput );  
}

float perlin2 ( const in float3 vectInput, const in float roughness  )
{
  float ret = Snoise3D( vectInput );
  ret += Snoise3D( vectInput*2 )*roughness;
  return ret;  
}

float perlin3 ( const in float3 vectInput, const in float roughness  )
{
  float ret = Snoise3D( vectInput );
  ret += Snoise3D( vectInput*2 )*roughness;
  ret += Snoise3D( vectInput*4 )*roughness*roughness;
  return ret;  
}
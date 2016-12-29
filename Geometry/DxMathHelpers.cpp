#include "DxMathHelpers.hpp"

namespace DxMath{

D3DXVECTOR3 unitVector( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2 )
{
  D3DXVECTOR3 u = aP1 - aP2;
  D3DXVec3Normalize( &u, &u );
  return u;
}

// ccw triangle
D3DXVECTOR3 normalOfTriangle ( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2, D3DXVECTOR3 aP3 )
{
  D3DXVECTOR3 u = aP2 - aP1;
  D3DXVECTOR3 v = aP3 - aP1;
  D3DXVECTOR3 n;
  D3DXVec3Cross( &n, &u, &v );
  D3DXVec3Normalize( &n, &n );
  DxMath::clean( n );
  return n;
}

//  right handed basis
D3DXVECTOR3 thirdVectorBasis ( D3DXVECTOR3 u, D3DXVECTOR3 v )
{
  D3DXVECTOR3 n;
  D3DXVec3Cross( &n, &u, &v );
  D3DXVec3Normalize( &n, &n );
  return n;
}

D3DXMATRIX  basisMatrix ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k )
{
  D3DXMATRIX m ( i.x, j.x, k.x, 0,
                 i.y, j.y, k.y, 0,
                 i.z, j.z, k.z, 0,
                 0,   0,   0,   1 );
  return m;
}

D3DXMATRIX  basisMatrix ( std::vector<D3DXVECTOR3> b )
{
  return basisMatrix( b[0], b[1], b[2] ) ;
}

void        componentMult ( D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2 )
{
  aP1.x *= aP2.x;
  aP1.y *= aP2.y;
  aP1.z *= aP2.z;
}

bool        equalsZero ( const D3DXVECTOR3& v )
{
  bool ret = false;
  if (  ( fabs ( v.x ) < 0.000001f ) &&
        ( fabs ( v.y ) < 0.000001f ) &&
        ( fabs ( v.z ) < 0.000001f ) )
        ret = true;
  return ret;
}

bool        equality( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const float epsilon )
{
  bool ret = false;
  if (  ( fabs ( v1.x - v2.x ) < epsilon ) &&
        ( fabs ( v1.y - v2.y ) < epsilon ) &&
        ( fabs ( v1.z - v2.z ) < epsilon ) )
        ret = true;
  return ret;
}

void        clean ( D3DXVECTOR3& v )
{
  float epsilon = 0.00000001f;
  if ( fabs(v.x) < epsilon )  
    v.x = 0.0f;
  if ( fabs(v.y) < epsilon )  
    v.y = 0.0f;
  if ( fabs(v.z) < epsilon )  
    v.z = 0.0f;
    
}

void        clean ( D3DXVECTOR3& v, const float aEpsilon )
{  
  if ( fabs(v.x) < aEpsilon )  
    v.x = 0.0f;
  if ( fabs(v.y) < aEpsilon )  
    v.y = 0.0f;
  if ( fabs(v.z) < aEpsilon )  
    v.z = 0.0f;    
}

void        reverse ( D3DXVECTOR3& v )
{
  if ( v.x != 0.0f )
    v.x *= -1.0f;
  if ( v.y != 0.0f )
    v.y *= -1.0f;
  if ( v.z != 0.0f )
    v.z *= -1.0f;
}

float distance ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 )
{
  D3DXVECTOR3 distance = v2 - v1;
  return D3DXVec3Length( &distance ); 
}

bool        equality( const float f1, const float f2, const float epsilon )
{
  bool ret = false;
  if ( fabs ( f1 - f2 ) < epsilon )
        ret = true;
  return ret;
}

D3DXVECTOR3 alignToTangents( const D3DXVECTOR3& aInitialTan, 
                            const D3DXVECTOR3& aFinalTan, 
                            const D3DXVECTOR3& aVector )
{
  if ( !DxMath::equality(aInitialTan, aFinalTan, 0.00001f ) )
  {
    float theta = acos( D3DXVec3Dot ( &aInitialTan, &aFinalTan ) ); 
    D3DXVECTOR3 axis; 
    D3DXVec3Cross ( &axis, &aInitialTan, &aFinalTan ) ; 

    D3DXMATRIX m;
    D3DXMatrixRotationAxis( &m, &axis, -theta);
    
    D3DXVECTOR3 result;
    D3DXVec3TransformCoord( &result, &aVector, &m );
    return result;
  }
  else
    return aVector;
}

};

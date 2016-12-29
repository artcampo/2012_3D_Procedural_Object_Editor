#ifndef DXMATHHELPERS_HPP
#define DXMATHHELPERS_HPP

#include "App/dx_misc.hpp"
#include <vector>

namespace DxMath{

D3DXVECTOR3 unitVector( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2 );
D3DXVECTOR3 normalOfTriangle ( D3DXVECTOR3 aP1, D3DXVECTOR3 aP2, D3DXVECTOR3 aP3 );
D3DXVECTOR3 thirdVectorBasis ( D3DXVECTOR3 u, D3DXVECTOR3 v );
D3DXMATRIX  basisMatrix ( std::vector<D3DXVECTOR3> b );
D3DXMATRIX  basisMatrix ( D3DXVECTOR3 i, D3DXVECTOR3 j, D3DXVECTOR3 k );
void        componentMult ( D3DXVECTOR3& aP1, const D3DXVECTOR3& aP2 );

bool        equalsZero ( const D3DXVECTOR3& v );
bool        equality( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const float epsilon );
bool        equality( const float f1, const float f2, const float epsilon );
void        clean ( D3DXVECTOR3& v );
void        clean ( D3DXVECTOR3& v, const float aEpsilon );
void        reverse ( D3DXVECTOR3& v );

float       distance ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );

D3DXVECTOR3 alignToTangents(const D3DXVECTOR3& aInitialTan, 
                            const D3DXVECTOR3& aFinalTan, 
                            const D3DXVECTOR3& aVector );

};

#endif // DXMATHHELPERS_HPP

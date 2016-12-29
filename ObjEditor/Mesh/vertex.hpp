#ifndef VERTEX_HPP
#define VERTEX_HPP

#include    "App/dx_misc.hpp"

class VertexObj
{
public:
    VertexObj(){};
    VertexObj( D3DXVECTOR3 aPos );

    D3DXVECTOR3 pos;

};

#endif // VERTEX_HPP

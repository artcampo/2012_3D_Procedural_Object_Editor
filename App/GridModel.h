#ifndef _GridModel_H
#define _GridModel_H

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"

class GridModel
{
public:
    GridModel( std::vector<D3DXVECTOR3>& mVertices,
               std::vector<D3DXVECTOR3>& mVerticesAxis,
               D3DXCOLOR& aColour,
               ID3D10Device* aMd3dDevice );
    ~GridModel();
    void display();
    void displayAxis();
    D3DXCOLOR& getColour();
    D3DXCOLOR& getColourAxis();
private:
    D3DXCOLOR mColour;
    D3DXCOLOR mColourAxis;
    DxMeshModelBatch<VertexPos>* mMesh;
    DxMeshModelBatch<VertexPos>* mMeshAxis;
};

#endif // GridModel_H

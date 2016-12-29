#ifndef LINELISTMODEL_H
#define LINELISTMODEL_H

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"

class LineListModel
{
public:
    LineListModel( std::vector<D3DXVECTOR3>& mVertices, D3DXCOLOR& aColour, ID3D10Device* aMd3dDevice );
    ~LineListModel();
    void display();

    D3DXCOLOR mColour;
private:
    DxMeshModelBatch<VertexPos>* mMesh;
};

#endif // LINELISTMODEL_H

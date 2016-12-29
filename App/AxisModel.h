#ifndef _AXISMODEL_H
#define _AXISMODEL_H

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"

class AxisModel
{
public:
    AxisModel( std::vector<D3DXVECTOR3>& mVertices, ID3D10Device* aMd3dDevice );
    ~AxisModel();
    void displayX();
    void displayY();
    void displayZ();
    D3DXCOLOR  getColourX();
    D3DXCOLOR  getColourY();
    D3DXCOLOR  getColourZ();
private:
    DxMeshModelBatch<VertexPos>* mMeshX;
    DxMeshModelBatch<VertexPos>* mMeshY;
    DxMeshModelBatch<VertexPos>* mMeshZ;
};

#endif

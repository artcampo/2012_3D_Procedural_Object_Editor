#ifndef IMAGEQUADMODEL_HPP
#define IMAGEQUADMODEL_HPP

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"

class ImageQuadModel
{
public:
    ImageQuadModel(std::vector<D3DXVECTOR3>& aPoints, float aAlpha, ID3D10Device* aMd3dDevice);
    ~ImageQuadModel();
    void display();
    float getAlpha();
private:
    DxMeshModelBatch<VertexPosUV>* mMesh;
    float mAlpha;
};

#endif // IMAGEQUADMODEL_HPP

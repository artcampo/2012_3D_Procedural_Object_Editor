#ifndef POINTLISTMODEL_H
#define POINTLISTMODEL_H

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"


class PointListModel
{
private:
      DxMeshModelBatch<VertexPointPSC>* mMesh;
public:
    PointListModel( std::vector<VertexPointPSC>& mVertices, ID3D10Device* aMd3dDevice );
    ~PointListModel();
    void display();

};

#endif // POINTLISTMODEL_H

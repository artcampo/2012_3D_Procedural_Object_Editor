#ifndef PAIRTANGENTSMODEL_H
#define PAIRTANGENTSMODEL_H

#include <vector>
#include "App/dx_misc.hpp"
#include "Geometry/DxMeshModel.hpp"
#include "pointlistmodel.h"
#include "linelistmodel.h"
#include "Editor_Bezier/EditorGlobal.hpp"

class PairTangentsModel
{
public:
    PairTangentsModel( std::vector<D3DXVECTOR3>& mVertices, D3DXCOLOR& aColour, ID3D10Device* aMd3dDevice );
    ~PairTangentsModel();
    D3DXCOLOR& getColour();
    void displayLines();
    void displayPoints();
private:
    D3DXCOLOR mColour;
    LineListModel*    mLines;
    PointListModel*   mPoints;

};

#endif // PAIRTANGENTSMODEL_H

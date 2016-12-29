#ifndef BEZIERMODEL_H
#define BEZIERMODEL_H

#include <vector>
#include "App/dx_misc.hpp"

#include "pointlistmodel.h"
#include "linelistmodel.h"
#include "Editor_Bezier/EditorGlobal.hpp"

class BezierModel
{
public:
    ~BezierModel();
    BezierModel( std::vector<D3DXVECTOR3>& aPoints,
                 std::vector<D3DXVECTOR3>& aPointsCurve,
                 D3DXCOLOR& aColour,
                 ID3D10Device* aMd3dDevice,
                 bool aPointSelected,
                 int aPointSelectedIndex);
    void displayLines();
    void displayPoints();
    D3DXCOLOR&  getColour();
private:
    D3DXCOLOR mColour;
    LineListModel*    mLines;
    PointListModel*   mPoints;
};

#endif // BEZIERMODEL_H

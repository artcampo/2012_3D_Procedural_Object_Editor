#include "beziermodel.h"

BezierModel::BezierModel( std::vector<D3DXVECTOR3>& aPoints,
                          std::vector<D3DXVECTOR3>& aPointsCurve,
                          D3DXCOLOR& aColour,
                          ID3D10Device* aMd3dDevice,
                          bool aPointSelected,
                          int aPointSelectedIndex )
{
  mColour = aColour;
  mLines  = new LineListModel( aPointsCurve, aColour, aMd3dDevice );

  std::vector<VertexPointPSC> points( aPoints.size() );
  for( unsigned int i = 0 ; i < aPoints.size() ; ++i )
  {
    points[i].centerW = aPoints[i];
    points[i].sizeW = D3DXVECTOR2( 0.5f, 0.0f );
    if (aPointSelected && aPointSelectedIndex == i)
      points[i].color = GlobalEditor::gColourPointSelected;
    else
      points[i].color = GlobalEditor::gColourPointNotSelected;
    //points[i].color = D3DXCOLOR(1,0,0,1);
  }

  mPoints = new PointListModel( points, aMd3dDevice );
}

BezierModel::~BezierModel()
{
  delete mLines;
  delete mPoints;
}

void BezierModel::displayLines()
{
  mLines->display();
}


void BezierModel::displayPoints()
{
  mPoints->display();
}

D3DXCOLOR& BezierModel::getColour()
{
  return mColour;
}

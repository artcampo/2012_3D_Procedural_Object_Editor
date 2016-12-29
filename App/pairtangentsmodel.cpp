#include "pairtangentsmodel.h"

PairTangentsModel::PairTangentsModel(  std::vector<D3DXVECTOR3>& mVertices, D3DXCOLOR& aColour, ID3D10Device* aMd3dDevice  )
{
  mColour = aColour;

  mLines  = new LineListModel( mVertices, aColour, aMd3dDevice );

  std::vector<VertexPointPSC> points( 2 );

  points[0].centerW = mVertices[0];
  points[0].sizeW = D3DXVECTOR2( 0.5f, 0.0f );
  points[0].color = GlobalEditor::gColourPointNotSelected;

  points[1].centerW = mVertices[2];
  points[1].sizeW = D3DXVECTOR2( 0.5f, 0.0f );
  points[1].color = GlobalEditor::gColourPointNotSelected;


  mPoints = new PointListModel( points, aMd3dDevice );
}

PairTangentsModel::~PairTangentsModel()
{
  delete mLines;
  delete mPoints;
}


void PairTangentsModel::displayLines()
{
  mLines->display();
}


void PairTangentsModel::displayPoints()
{
  mPoints->display();
}

D3DXCOLOR & PairTangentsModel::getColour()
{
  return mColour;
}

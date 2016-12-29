#include "GizmoR.hpp"


GizmoTMeshModel* GizmoR::convert( ID3D10Device* aDev, ColourMapping* aColours )
{
  GizmoTMeshModel* model;

  //  Single axis
  std::vector<VertexPosCol> vert;
  for ( int i = 0 ; i < 3; ++i )
  {
    VertexPosCol v,u;
    v.pos = mCenter;
    u.pos = mCenter + mBase[i]*length();

    D3DXCOLOR colour;
    if ( mHasHighlight &&
         (mHighlightedAxisU == i || mHighlightedAxisV == i) )
      colour = aColours->gizmoHigh[i];
    else
      colour = aColours->gizmo[i];

    v.col = colour;
    u.col = colour;

    vert.push_back(v);
    vert.push_back(u);
  }

  //  Doubly axis

  for ( int i = 0 ; i < 3; ++i )
  {
    VertexPosCol v,u;

    gizmoTLine line = composedAxis(i);
    v.pos = line.a;
    u.pos = line.b;

    D3DXCOLOR colour;
    if ( mHasHighlight && mTwoAxisSelection && mHighlighedCorner == i )
      colour = aColours->gizmoHigh[i];
    else
      colour = aColours->gizmo[i];

    v.col = colour;
    u.col = colour;

    vert.push_back(v);
    vert.push_back(u);
  }

  model = new GizmoTMeshModel( vert, aDev );
  return model;
}

GizmoR::GizmoR()
{
  mScale = 1;
  mLengthAxis = 2.0f;
  mHighlightedAxisU = 3;
  mHighlightedAxisV = 3;
  mBase[3] = D3DXVECTOR3(0,0,0);
  mHasHighlight = false;
}

D3DXVECTOR3 GizmoR::highlightedAxisU()
{
  return mBase[mHighlightedAxisU];
}

D3DXVECTOR3 GizmoR::highlightedAxisV()
{
  return mBase[mHighlightedAxisV];
}

bool GizmoR::check(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold)
{
  mHasHighlight = false;
  bool  existsMin = false;
  float distanceMin;

  //  Single Axis
  for ( int i = 0 ; i < 3; ++i )
  {
    D3DXVECTOR3 vb = mCenter;
    D3DXVECTOR3 ve = mCenter + mBase[i];

    D3DXVECTOR3 dummy;
    float distance = Intersections::distanceSegmentToLine(  aRayB, aRayE, vb, ve, dummy );
    bool update = false;
    if (!existsMin && distance < aDistThreshold ) update = true;
    if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

    if (update)
    {
      existsMin = true;
      distanceMin = distance;
      /*
      mHighlightedAxisU = i;
      mHighlightedAxisV = 3;
      mOneAxisSelection = true;
      */
      mHighlightedAxisU = i;
      mTwoAxisSelection = false;
      mOneAxisSelection = true;
      mHasHighlight = true;
    }
  }

  if (mHasHighlight) return mHasHighlight;

  //  Double Axis
  for ( int i = 0 ; i < 3; ++i )
  {
    gizmoTLine line = composedAxis(i);
    D3DXVECTOR3 vb = line.a;
    D3DXVECTOR3 ve = line.b;

    D3DXVECTOR3 dummy;
    float distance = Intersections::distanceSegmentToLine(  aRayB, aRayE, vb, ve, dummy );
    bool update = false;
    if (!existsMin && distance < aDistThreshold ) update = true;
    if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

    if (update)
    {
      existsMin = true;
      distanceMin = distance;
      /*
      mHighlightedAxisU = i;
      mHighlightedAxisV = 3;
      mOneAxisSelection = true;
      */
      mHighlightedAxisU = (i + 1 )%3;
      mHighlightedAxisV = (i + 2 )%3;
      mHighlighedCorner = i;
      mTwoAxisSelection = true;
      mOneAxisSelection = false;
      mHasHighlight = true;
    }
  }

  return existsMin;
}

bool GizmoR::hasHighlight()
{
  return mHasHighlight;
}

//  \return increment with respect to center of mass
D3DXVECTOR3 GizmoR::newPosition(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aCamU, D3DXVECTOR3 aCamV)
{
  D3DXVECTOR3 pos;
  if ( mOneAxisSelection )
  {
    D3DXVECTOR3 base;
    if ( D3DXVec3Dot( &highlightedAxisU(), &aCamU ) < 0.01f )
      base = aCamV;
    else
      base = aCamU;
    Intersections::Plane p( highlightedAxisU(), base, mCenter );

    bool unique;
    pos = Intersections::intersectionLinePlane( aRayB, aRayE, p, unique );
    pos = Intersections::distancePointToLineVector( mCenter, mCenter + highlightedAxisU(), pos );
    pos = pos - mCenter;
  }

  if ( mTwoAxisSelection )
  {

    Intersections::Plane p( highlightedAxisV(), highlightedAxisU(), mCenter );
    bool unique;
    pos = Intersections::intersectionLinePlane( aRayB, aRayE, p, unique );
    pos = pos - mCenter;
  }

  return pos;
}

gizmoTLine GizmoR::composedAxis(int i)
{
  gizmoTLine line;

  int indexU = (i + 1 )%3;
  int indexV = (i + 2 )%3;

  float len = sqrt(1.0f)*length();
  line.a = mCenter + len*mBase[indexU] + ( len*0.8f)* mBase[indexV];
  line.b = mCenter + len*mBase[indexV] + ( len*0.8f)* mBase[indexU];

  return line;
}

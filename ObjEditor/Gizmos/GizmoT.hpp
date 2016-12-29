#ifndef _GIZMO_T_H
#define _GIZMO_T_H

#include "App/dx_misc.hpp"

#include "ObjEditor/ObjEditorSharedData.hpp"
#include "App/EditableMeshModel.h"
#include "App/ObjEd/GizmoTMeshModel.hpp"

#include "Geometry/Intersections.hpp"

using namespace ObjEd;

struct gizmoTLine{
  D3DXVECTOR3 a;
  D3DXVECTOR3 b;
};

class GizmoT
{
public:
  GizmoT();


  //  Render
public:
  GizmoTMeshModel* convert( ID3D10Device* aDev, ColourMapping* aColours );

  bool  check( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold );

  bool          hasHighlight();


  D3DXVECTOR3 newPosition( D3DXVECTOR3 aRayB,
                           D3DXVECTOR3 aRayE,
                           D3DXVECTOR3 aCamU,
                           D3DXVECTOR3 aCamV);

  D3DXVECTOR3 mCenter;
  D3DXVECTOR3 mBase[4];
  int   mAxisHighlighted; //mAxisHighlighted=3 => no axis used, for displacement TODO: good?
  int       mHighlightedAxisU;
  int       mHighlightedAxisV;
  int       mHighlighedCorner;

  bool      mOneAxisSelection;
  bool      mTwoAxisSelection;
  bool      mThreeAxisSelection;

  bool      mHasHighlight;

  D3DXVECTOR3 highlightedAxisU();
  D3DXVECTOR3 highlightedAxisV();


  void setScale( float aScale )
  {
    mScale = aScale;
  };
  float mScale;
private:
  float     mLengthAxis;
  float   length()
  {
    //return mLengthAxis*(1.0f/mScale);
    return mLengthAxis*(mScale*0.1f*0.5f);
  };


  gizmoTLine  composedAxis( int i );

};

#endif // _GIZMO_T_H

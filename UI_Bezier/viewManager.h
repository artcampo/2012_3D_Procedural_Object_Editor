#ifndef VIEW_H
#define VIEW_H

#include "App/dx_misc.hpp"
#include "Geometry/MathConstants.hpp"
#include <iostream>
#include <sstream>
#include <windowsx.h>

#include <string>

class ViewManager
{
public:

  bool   allowsAppend();
  bool   allowsSelection();

  bool   allowsPan();
  bool   allowsOrbitation();

  void   pan( float aX, float aY );
  float  scale (float aScale );
  void   orbit( float aX, float aY );

  std::wstring getTitle();

public:
   static ViewManager ViewX();
   static ViewManager ViewY();
   static ViewManager ViewZ();
   static ViewManager ViewFree();

   ViewManager( std::wstring& aTitle,
                int aTypeView, bool aAllowPointInsertion, bool aAllowPointSelection,
                bool aAllowPan,  bool aAllowOrbitation,
         D3DXVECTOR3& aUnitVector,         
         D3DXVECTOR3& aUnitTangentU,
         D3DXVECTOR3& aUnitTangentV,
         D3DXVECTOR3& aRenderUpVector,
         D3DXVECTOR2& aCamPan, float aCamScale );

   ViewManager(std::wstring& aTitle, int aTypeView, bool aAllowPointInsertion, bool aAllowPointSelection,
               bool aAllowPan, bool aAllowOrbitation,
              D3DXVECTOR2& aCamPan, float aCamScale, D3DXVECTOR2& aCamOrbit );


   D3DXVECTOR3  projectPoint( float aX, float aY );
   D3DXVECTOR3  getSelectionMask();

   D3DXVECTOR3  getCamAt();
   D3DXVECTOR3  getCamPos();

   D3DXVECTOR3  adjustPointToGrid( const D3DXVECTOR3& aPoint );


   D3DXVECTOR3  getTangentVectorU();
   D3DXVECTOR3  getTangentVectorV();
   float        getScale();

   float        getGridScale();
   void         setGridScaleMultiplier( float aMult );

   D3DXVECTOR3  getUpVector();

   bool         isFreeView();   

private:

  int   mTypeView;
  bool  mAllowPointInsertion;
  bool  mAllowPointSelection;

  bool  mAllowPan;
  bool  mAllowOrbitation;


  float         mGridScaleMult;
  float         mGridMod;


  D3DXVECTOR2   mCamPan;
  float         mCamScale;
  D3DXVECTOR2   mCamOrbit;    //spherical coordinates: thita, phy

  D3DXVECTOR3   mUnitVector;
  D3DXVECTOR3   mUnitTangentU;
  D3DXVECTOR3   mUnitTangentV;  
  D3DXVECTOR3   mRenderUpVector;

  std::wstring   mTitle;

private:
  D3DXVECTOR3   projectPoint( D3DXVECTOR2& aP );
  float         adjustCoordinate (const float aCoord, float aMod );
};

#endif // VIEW_H

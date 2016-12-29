#ifndef _OBJ_VIEW_DG_H
#define _OBJ_VIEW_DG_H

#include "App/dx_misc.hpp"
#include "Geometry/MathConstants.hpp"
#include <iostream>
#include <sstream>
#include <windowsx.h>

#include <string>

class ObjViewDG
{
public:

  bool   allowsOrbitation();

  void   panUpdate    ( float aX, float aY );
  float  scaleUpdate  ( float aScale );
  void   orbitUpdate  ( float aX, float aY );

  std::wstring getTitle();

public:
   static ObjViewDG* ViewFree();
/*
   ObjViewDG( std::wstring& aTitle,
                int aTypeView, bool aAllowPointInsertion, bool aAllowPointSelection,
                bool aAllowPan,  bool aAllowOrbitation,
         D3DXVECTOR3& aUnitVector,         
         D3DXVECTOR3& aUnitTangentU,
         D3DXVECTOR3& aUnitTangentV,
         D3DXVECTOR3& aRenderUpVector,
         D3DXVECTOR2& aCamPan, float aCamScale );
*/
   ObjViewDG(std::wstring& aTitle, int aTypeView,
              bool aAllowOrbitation,
              D3DXVECTOR2& aCamPan, float aCamScale, D3DXVECTOR2& aCamOrbit );


   D3DXVECTOR3  projectPoint( float aX, float aY );   
   D3DXVECTOR3  adjustPointToGrid( const D3DXVECTOR3& aPoint );

   // Interface with dx
   D3DXVECTOR3  getCamAt();
   D3DXVECTOR3  getCamPos();


   D3DXVECTOR3  unitVector();
   D3DXVECTOR3  getCamTangentU();
   D3DXVECTOR3  getCamTangentV();

   D3DXVECTOR3  cameraTangetU();
   D3DXVECTOR3  cameraTangetV();

   // used for the grid
   D3DXVECTOR3  getTangentVectorU();
   D3DXVECTOR3  getTangentVectorV();
   float        getScale();

   float        getGridScale();
   void         setGridScaleMultiplier( float aMult );

   D3DXVECTOR3  getUpVector();

   bool         isFreeView();   

private:

  int   mTypeView;
  enum eType{
    eTypeX,
    eTypeY,
    eTypeZ,
    eTypeFree
  };
  bool  mAllowOrbitation;


  float         mGridScaleMult;
  float         mGridMod;
public:
  D3DXVECTOR2   mCamPan;
  float         mCamScale;
  D3DXVECTOR2   mCamOrbit;    //spherical coordinates: thita, phy

  D3DXVECTOR3   mUnitVector;
  D3DXVECTOR3   mUnitTangentU;
  D3DXVECTOR3   mUnitTangentV;  
  D3DXVECTOR3   mRenderUpVector;

  std::wstring   mTitle;

private:
  //  Configuration
  float mScaleMinimum;
  float mScaleMultiplier;

private:
  D3DXVECTOR3   projectPoint( D3DXVECTOR2& aP );
  float         adjustCoordinate (const float aCoord, float aMod );

  friend class QD3DWidgetDirG;
};

#endif // _OBJ_VIEW_DG_H

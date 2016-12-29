#include "viewManager.h"

ViewManager::ViewManager(
          std::wstring& aTitle,
          int aTypeView, bool aAllowPointInsertion, bool aAllowPointSelection,
          bool aAllowPan,  bool aAllowOrbitation,
          D3DXVECTOR3& aUnitVector,
          D3DXVECTOR3& aUnitTangentU,
          D3DXVECTOR3& aUnitTangentV,
          D3DXVECTOR3& aRenderUpVector,
          D3DXVECTOR2& aCamPan, float aCamScale )
{
  mTypeView   = aTypeView;
  mTitle      = aTitle;

  mAllowPointInsertion  = aAllowPointInsertion;
  mAllowPointSelection  = aAllowPointSelection;

  mAllowPan             = aAllowPan;
  mAllowOrbitation      = aAllowOrbitation;

  mUnitVector   = aUnitVector;  
  mUnitTangentU = aUnitTangentU;
  mUnitTangentV = aUnitTangentV;
  mRenderUpVector = aRenderUpVector;

  mCamPan     = aCamPan;
  mCamScale   = aCamScale;
  mGridScaleMult = 1.0f;
  mGridMod    = 10.0f;
}

ViewManager::ViewManager(
    std::wstring& aTitle,
    int aTypeView, bool aAllowPointInsertion, bool aAllowPointSelection,
    bool aAllowPan,  bool aAllowOrbitation,
    D3DXVECTOR2& aCamPan, float aCamScale, D3DXVECTOR2& aCamOrbit )
{
  mTypeView   = aTypeView;
  mTitle      = aTitle;

  mAllowPointInsertion = aAllowPointInsertion ;
  mAllowPointSelection = aAllowPointSelection;

  mAllowPan         = aAllowPan;
  mAllowOrbitation  = aAllowOrbitation;

  mCamPan     = aCamPan;
  mCamScale   = aCamScale;
  mCamOrbit   = aCamOrbit;

  mUnitTangentU = D3DXVECTOR3(1,0,0);
  mUnitTangentV = D3DXVECTOR3(0,0,1);

  mRenderUpVector = D3DXVECTOR3(0,1,0);

}

ViewManager ViewManager::ViewX()
{
  std::wstring title(L"Right");

  return ViewManager( title,0, true, true, true, false,
               D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
               D3DXVECTOR3( 0.0f, 0.0f, 1.0f ),
               D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
               D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
               D3DXVECTOR2( 0,0 ), 100.0f );
}

ViewManager ViewManager::ViewY()
{
  std::wstring title(L"Top");
  return ViewManager( title,1, true, true, true, false,
               D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
               D3DXVECTOR3( 0.0f, 0.0f, 1.0f ),
               D3DXVECTOR3( -1.0f, 0.0f, 0.0f ),
               D3DXVECTOR3( -1.0f, 0.0f, 0.0f ),
               D3DXVECTOR2( 0,0 ), 100.0f );
}

ViewManager ViewManager::ViewZ()
{
  std::wstring title(L"Front");
  return ViewManager( title, 2, true, true, true, false,
               D3DXVECTOR3( 0.0f, 0.0f, 1.0f ),
               D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
               D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
               D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
               D3DXVECTOR2( 0,0 ), 100.0f );
}

ViewManager ViewManager::ViewFree()
{
  std::wstring title(L"Free");
  return ViewManager( title , 3, false, false, false, true,
               D3DXVECTOR2( 0,0 ), 1.0f,
               //D3DXVECTOR2( Math::PI,Math::PI*1.25f ) );
                      D3DXVECTOR2( 1.2f,0.7f ) );
}

bool ViewManager::allowsAppend()
{
  return mAllowPointInsertion;
}
/*
View& View::operator =(const View &rhs)
{
}
*/

D3DXVECTOR3  ViewManager::projectPoint( float aX, float aY )
{
  D3DXVECTOR3 ret;

  switch (mTypeView)
  {
  case 0:
    //ret  = D3DXVECTOR3( 0.0f, -aY*sqrt(mCamScale*mCamScale), aX*sqrt(mCamScale*mCamScale) );
    ret  = D3DXVECTOR3( 0.0f, aY*mCamScale, aX*mCamScale );
    ret += projectPoint( mCamPan );
    break;
  case 1:
    ret = D3DXVECTOR3( -aY*mCamScale, 0.0f, aX*mCamScale );
    ret += projectPoint( mCamPan );
    break;
  case 2:
    ret = D3DXVECTOR3( -aX*mCamScale, aY*mCamScale, 0.0f );
    ret += projectPoint( mCamPan );
    break;
  case 3:
    break;
  };

  return ret;
}

D3DXVECTOR3  ViewManager::projectPoint( D3DXVECTOR2& aP )
{
  D3DXVECTOR3 ret;

  switch (mTypeView)
  {
  case 0:
    ret = D3DXVECTOR3( 0.0f, aP.y, -aP.x );
    break;
  case 1:
    ret = D3DXVECTOR3( aP.y, 0.0f, aP.x );
    break;
  case 2:
    ret = D3DXVECTOR3( -aP.x, -aP.y, 0.0f );
    break;
  };

  return ret;
}

D3DXVECTOR3  ViewManager::getSelectionMask()
{
  D3DXVECTOR3 ret;
  if (mTypeView == 3)
  {
    ret = D3DXVECTOR3(1,1,1);
  }
  else
  {
    if ( mUnitVector.x == 1.0f )
      ret.x = 0.0f;
    else
      ret.x = 1.0f;

    if ( mUnitVector.y == 1.0f )
      ret.y = 0.0f;
    else
      ret.y = 1.0f;

    if ( mUnitVector.z == 1.0f )
      ret.z = 0.0f;
    else
      ret.z = 1.0f;
  }
  return ret;
}

D3DXVECTOR3 ViewManager::getCamAt()
{
  D3DXVECTOR3 ret;
  if (mTypeView != 3)
    ret = projectPoint( mCamPan );
  else
  {
    ret = D3DXVECTOR3( 0, 0, 0 );
  }
  return ret;
}

D3DXVECTOR3 ViewManager::getCamPos()
{
  D3DXVECTOR3 ret;
  if (mTypeView != 3)
  {
    ret  = mUnitVector;
    ret *= mCamScale;
    ret += projectPoint( mCamPan );
  }
  else
  {
    float thita = mCamOrbit.y;
    float rho   = mCamOrbit.x;
    ret  = D3DXVECTOR3( sin(rho)*cos(thita),
                        cos(rho),
                        sin(rho)*sin(thita)
                        );
    ret *= mCamScale;
  }
  return ret;
}

void ViewManager::pan(float aX, float aY)
{
  //mCamPan += D3DXVECTOR2( aX*mCamScale, aY*mCamScale );
  mCamPan += D3DXVECTOR2( -aX*150.0f, -aY*150.0f );
}

float ViewManager::scale (float aScale )
{
 mCamScale += 0.25f*aScale;
 if (mCamScale <= 0.0)
   mCamScale = 0.01f;
 return mCamScale;
}

bool ViewManager::allowsPan()
{
  return mAllowPan;
}

bool ViewManager::allowsOrbitation()
{
  return mAllowOrbitation;
}

void ViewManager::orbit(float aX, float aY)
{
  float multiplier = 0.01f;

  mCamOrbit += D3DXVECTOR2( aX*multiplier, aY*multiplier );  
}

D3DXVECTOR3 ViewManager::getTangentVectorU()
{
  return mUnitTangentU;
}

D3DXVECTOR3 ViewManager::getTangentVectorV()
{
  return mUnitTangentV;
}

float ViewManager::getScale()
{
  return mCamScale;
}

bool ViewManager::allowsSelection()
{
  return mAllowPointSelection;
}

D3DXVECTOR3 ViewManager::getUpVector()
{
  return mRenderUpVector;
}

bool ViewManager::isFreeView()
{
  return (mTypeView == 3);
}

std::wstring ViewManager::getTitle()
{
  return mTitle;
}

float ViewManager::getGridScale()
{
  //float scale = floor( 10.0f / (mCamScale * 1.0f) ) ;
  float m = 10.0f;
  float scale =  floor( (m*mCamScale/100.0f))/m ;
  scale *= 0.1f*mCamScale;

  mGridMod = scale * mGridScaleMult;
  if (mGridMod < 3.0f)
    mGridMod = 3.0f;

  return mGridMod;
}

void ViewManager::setGridScaleMultiplier(float aMult )
{
  mGridScaleMult = aMult;
}


D3DXVECTOR3  ViewManager::adjustPointToGrid( const D3DXVECTOR3& aPoint )
{
  D3DXVECTOR3 ret;
  ret.x = adjustCoordinate( aPoint.x, mGridMod );
  ret.y = adjustCoordinate( aPoint.y, mGridMod );
  ret.z = adjustCoordinate( aPoint.z, mGridMod );
  return ret;
}

float ViewManager::adjustCoordinate (const float aCoord, float aMod )
{
  float div = floor(aCoord/aMod) * aMod; ;
  float mod = aCoord-div;

  if (mod >= (0.5f*aMod))
      return (div + aMod);
  else if (mod <= (0.5f*aMod))
      return div;
  else
      return (div + aMod) ;

}

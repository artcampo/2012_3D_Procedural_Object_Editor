#include "ObjView.h"

ObjView::ObjView(
    std::wstring& aTitle,
    int aTypeView,
    bool aAllowOrbitation,
    D3DXVECTOR2& aCamPan,
    float aCamScale,
    D3DXVECTOR2& aCamOrbit )
{
  mTypeView   = aTypeView;
  mTitle      = aTitle;

  mAllowOrbitation  = aAllowOrbitation;

  mCamPan     = aCamPan;
  mCamScale   = aCamScale;
  mCamOrbit   = aCamOrbit;

  mUnitTangentU = D3DXVECTOR3(1,0,0);
  mUnitTangentV = D3DXVECTOR3(0,0,1);

  mRenderUpVector = D3DXVECTOR3(0,1,0);


  //  Configuration
  mScaleMinimum     = 0.01f;
  mScaleMultiplier  = 0.01f;

  mGridScaleMult    = 1.0f;

}


ObjView* ObjView::ViewFree()
{
  std::wstring title(L"Free");
  return new ObjView( title , eTypeFree,
                      true,
               D3DXVECTOR2( 0,0 ), 9.0f,
               //D3DXVECTOR2( Math::PI,Math::PI*1.25f ) );
                      //D3DXVECTOR2( 1.2f,0.7f ) );
                      D3DXVECTOR2( 0.9f,0.6f ) );

}


D3DXVECTOR3  ObjView::projectPoint( float aX, float aY )
{
  D3DXVECTOR3 ret;

  switch (mTypeView)
  {
  case eTypeX:
    //ret  = D3DXVECTOR3( 0.0f, -aY*sqrt(mCamScale*mCamScale), aX*sqrt(mCamScale*mCamScale) );
    ret  = D3DXVECTOR3( 0.0f, aY*mCamScale, aX*mCamScale );
    ret += projectPoint( mCamPan );
    break;
  case eTypeY:
    ret = D3DXVECTOR3( -aY*mCamScale, 0.0f, aX*mCamScale );
    ret += projectPoint( mCamPan );
    break;
  case eTypeZ:
    ret = D3DXVECTOR3( -aX*mCamScale, aY*mCamScale, 0.0f );
    ret += projectPoint( mCamPan );
    break;
  case eTypeFree:
    ret = D3DXVECTOR3( -aX*mCamScale, aY*mCamScale, 0.0f );
    ret += projectPoint( mCamPan );
    break;
  };

  return ret;
}

D3DXVECTOR3  ObjView::projectPoint( D3DXVECTOR2& aP )
{
  D3DXVECTOR3 ret;

  switch (mTypeView)
  {
  case eTypeX:
    ret = D3DXVECTOR3( 0.0f, aP.y, -aP.x );
    break;
  case eTypeY:
    ret = D3DXVECTOR3( aP.y, 0.0f, aP.x );
    break;
  case eTypeZ:
    ret = D3DXVECTOR3( -aP.x, -aP.y, 0.0f );
    break;
  case eTypeFree:
    float r = mCamScale;
    float t = mCamOrbit.x;
    float p = mCamOrbit.y;
    D3DXVECTOR3 u = +(cos(t)*cos(p))* D3DXVECTOR3(1,0,0)
                    +(cos(t)*sin(p))* D3DXVECTOR3(0,0,1)
                    -(sin(t))* D3DXVECTOR3(0,1,0);
    D3DXVECTOR3 v = -(sin(p))* D3DXVECTOR3(1,0,0)
                    +(cos(p))* D3DXVECTOR3(0,0,1);

    ret = aP.x*v + -aP.y*u;
    break;
  };

  return ret;
}


D3DXVECTOR3 ObjView::getCamAt()
{
  return projectPoint( mCamPan );
}

D3DXVECTOR3 ObjView::getCamPos()
{
  D3DXVECTOR3 ret;

  ret  = unitVector();
  ret *= mCamScale;
  ret += projectPoint( mCamPan );

  return ret;
}

void ObjView::panUpdate(float aX, float aY)
{
  mCamPan += D3DXVECTOR2( -aX*15.0f, aY*15.0f );

  D3DXVECTOR3 ret;
  float r = mCamScale;
  float t = mCamOrbit.x;
  float p = mCamOrbit.y;
  D3DXVECTOR3 u = +(r*cos(t)*cos(p))* D3DXVECTOR3(1,0,0)
                  +(r*cos(t)*sin(p))* D3DXVECTOR3(0,0,1)
                  -(r*sin(t))* D3DXVECTOR3(0,1,0);
  D3DXVECTOR3 v = -(r*sin(p))* D3DXVECTOR3(1,0,0)
                  +(r*cos(p))* D3DXVECTOR3(0,0,1);


}

float ObjView::scaleUpdate (float aScale )
{
 mCamScale += mScaleMultiplier*aScale;
 if (mCamScale <= mScaleMinimum)
   mCamScale = mScaleMinimum;
 return mCamScale;
}

void ObjView::orbitUpdate(float aX, float aY)
{
  float multiplier = 0.005f;

  mCamOrbit += D3DXVECTOR2( aX*multiplier, aY*multiplier );  

  if ( mCamOrbit.x < 0.5 )
   mCamOrbit.x = 0.5;

  if ( mCamOrbit.x > 2.4 )
   mCamOrbit.x = 2.4;
}


bool ObjView::allowsOrbitation()
{
  return mAllowOrbitation;
}

D3DXVECTOR3 ObjView::getTangentVectorU()
{
  return mUnitTangentU;
}

D3DXVECTOR3 ObjView::getTangentVectorV()
{
  return mUnitTangentV;
}

float ObjView::getScale()
{
  return mCamScale;
}

D3DXVECTOR3 ObjView::getUpVector()
{
  return mRenderUpVector;
}

bool ObjView::isFreeView()
{
  return (mTypeView == 3);
}

std::wstring ObjView::getTitle()
{
  return mTitle;
}

float ObjView::getGridScale()
{
  //float scale = floor( 10.0f / (mCamScale * 1.0f) ) ;
  float m = 10.0f;
  float scale =  floor( (m*mCamScale/100.0f))/m ;
  scale *= 0.1f*mCamScale;

  mGridMod = scale * mGridScaleMult;
  if (mGridMod < 0.5f)
    mGridMod = 0.5f;

  return mGridMod;
}

void ObjView::setGridScaleMultiplier(float aMult )
{
  mGridScaleMult = aMult;
}


D3DXVECTOR3  ObjView::adjustPointToGrid( const D3DXVECTOR3& aPoint )
{
  D3DXVECTOR3 ret;
  ret.x = adjustCoordinate( aPoint.x, mGridMod );
  ret.y = adjustCoordinate( aPoint.y, mGridMod );
  ret.z = adjustCoordinate( aPoint.z, mGridMod );
  return ret;
}

float ObjView::adjustCoordinate (const float aCoord, float aMod )
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

/*
  unit vector from target to camera position
*/
D3DXVECTOR3 ObjView::unitVector()
{
  D3DXVECTOR3 ret;
  if (mTypeView != 3)
    ret  = mUnitVector;
  else
  {
    float thita = mCamOrbit.y;
    float rho   = mCamOrbit.x;
    ret  = D3DXVECTOR3( sin(rho)*cos(thita),
                        cos(rho),
                        sin(rho)*sin(thita)
                        );
  }
  return ret;
}


D3DXVECTOR3 ObjView::cameraTangetU()
{
  float r = mCamScale;
  float t = mCamOrbit.x;
  float p = mCamOrbit.y;
  D3DXVECTOR3 u = +(cos(t)*cos(p))* D3DXVECTOR3(1,0,0)
                  +(cos(t)*sin(p))* D3DXVECTOR3(0,0,1)
                  -(sin(t))* D3DXVECTOR3(0,1,0);
  return u;
}

D3DXVECTOR3 ObjView::cameraTangetV()
{
  float r = mCamScale;
  float t = mCamOrbit.x;
  float p = mCamOrbit.y;
  D3DXVECTOR3 v = -(sin(p))* D3DXVECTOR3(1,0,0)
      +(cos(p))* D3DXVECTOR3(0,0,1);

  return v;
}


D3DXVECTOR3 ObjView::getCamTangentU()
{
  float thita = mCamOrbit.y;
  float rho   = mCamOrbit.x;
  D3DXVECTOR3 ret  = D3DXVECTOR3( -sin(thita),
                      0,
                      cos(thita)
                      );
  return ret;
}

D3DXVECTOR3 ObjView::getCamTangentV()
{
  float thita = mCamOrbit.y;
  float rho   = mCamOrbit.x;
  D3DXVECTOR3 ret  = D3DXVECTOR3( cos(rho)*cos(thita),
                      -sin(rho),
                      cos(rho)*sin(thita)
                      );
  return ret;
}

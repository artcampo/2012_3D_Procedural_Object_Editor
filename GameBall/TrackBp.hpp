#ifndef _GameTrackBp_HPP_
#define _GameTrackBp_HPP_

#include "App/dx_misc.hpp"
#include <vector>

namespace TrackBp{

  class Vertex{
  public:
    Vertex(){};
    Vertex( const D3DXVECTOR3& aX, const D3DXVECTOR3& aT, const D3DXVECTOR3& aN )
    {
      pos = aX;
      tan = aT;
      nor = aN;
    }
    Vertex( const D3DXVECTOR3& aX )
    {
      pos = aX;
      tan = D3DXVECTOR3(0,0,0);
      nor = D3DXVECTOR3(0,0,0);
    }
    D3DXVECTOR3 pos;
    D3DXVECTOR3 tan;
    D3DXVECTOR3 nor;
  };

  class EdgeData{
  public:
    EdgeData(){};
    EdgeData( const float aDesiredLength )
    {
      mDesiredLength = aDesiredLength;
    };
  public:
    float   mDesiredLength;
  };

  struct TrackDescription{
    D3DXVECTOR3 posBeg;
    D3DXVECTOR3 posEnd;
    float curvatureMax;
    float curvatureMin;
    float totalLength;
    float numUnitsPerCurve;
    int   iterations;
  };

  struct TrackEntityDescription{
  float         archDensity;
  float         archDispersion;
  float         bumperDensity;
  float         bumperDispersion;
  float         archHeight;
  float         bumperHeight;
  };

  enum ePatternIns{
    ePatternAB
  };

};

#endif  //_GameTrackBp_HPP_

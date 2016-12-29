#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

#include "App/dx_misc.hpp"

namespace Intersections{

/////////////////////////////////////////////////////
//  3d Data structures
class Plane{
public:
  Plane(){};
  // TODO remove this constructor
  Plane( D3DXVECTOR3 u, D3DXVECTOR3 v, D3DXVECTOR3 p);
  D3DXVECTOR3 normal();
  D3DXVECTOR3 point();
  static Plane planeBy3Points( D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c );
  //Plane static planeBy2Vectors1Point( D3DXVECTOR3 u, D3DXVECTOR3 v, D3DXVECTOR3 p );
private:
  D3DXVECTOR3 mNormal;
  D3DXVECTOR3 mPoint;
};

struct BoundingSphere{
  D3DXVECTOR3 center;
  float       radius;
};

/////////////////////////////////////////////////////
//  3d algorithms

float distance ( D3DXVECTOR3& a, D3DXVECTOR3& b );
bool areCollinear ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c );

float distancePointToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint );
D3DXVECTOR3 distancePointToLineVector ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint );

float distanceSegmentToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                              D3DXVECTOR3 aSegB, D3DXVECTOR3 aSegE,
                              D3DXVECTOR3& aPointInSegment );

D3DXVECTOR3 intersectionLinePlane ( D3DXVECTOR3 aRayB,
                                    D3DXVECTOR3 aRayE,
                                    Plane& aPlane,
                                    bool& aUniqueSolution);

bool  frontFacing ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c, const D3DXVECTOR3 aCamPos );
bool  raySphere( const BoundingSphere& aSphere, D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE );

/////////////////////////////////////////////////////
//  2d algorithms
float signedArea( D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c);
bool  third_is_in_between (D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c);
bool  segmentIntersectionExclusive ( D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r, D3DXVECTOR2 s);
D3DXVECTOR2 pointOfIntersectingSegments ( D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4 );

};

#endif // INTERSECTIONS_HPP

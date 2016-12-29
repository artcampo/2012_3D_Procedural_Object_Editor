#include "bezier.h"

Bezier::Bezier()
{
  mNumSegments = 0;
  mNumPoints   = 0;
  mTesellationSegment = 100;
}


Bezier::Bezier( int aUniqueId, std::string& aName ) :
  EditorObject ( aUniqueId, aName )
{
  mNumSegments = 0;
  mNumPoints   = 0;
  mTesellationSegment = 100;
}

Bezier::Bezier( int aUniqueId, std::string& aName, const Bezier& b ) :
  EditorObject ( aUniqueId, aName )
{
  mNumSegments = b.mNumSegments;
  mNumPoints   = b.mNumPoints;
  mTesellationSegment = b.mTesellationSegment;
  mPoints.resize( b.mPoints.size() );
  for ( unsigned int i = 0 ; i < b.mPoints.size() ; ++i )
    mPoints[i] = b.mPoints[i];
}

/*
  \param aP     point to append
  \param aUnit  U-unit vector of the current view, multiplied by its scale
*/
void Bezier::appendPoint( const D3DXVECTOR3& aP, const D3DXVECTOR3& aUnit  )
{
  if ( mNumPoints == 0 )
  {
    mPoints.push_back( aP );
    mPoints.push_back( aP + aUnit*0.1f );
    ++mNumPoints;
  }
  else
  {
    mPoints.push_back( aP - aUnit*0.1f );
    mPoints.push_back( aP );
    mPoints.push_back( aP + aUnit*0.1f );
    ++mNumSegments;
    ++mNumPoints;
  }
}

/*
  Returns all the points of the curve
*/
std::vector<D3DXVECTOR3> Bezier::getPoints()
{
  std::vector<D3DXVECTOR3> points (mNumPoints);
  for ( int i = 0 ; i < mNumPoints ; ++i )
    points[i] = mPoints[ indexOfPoint(i) ];

  return points;
}

std::vector<D3DXVECTOR3> Bezier::getCurve()
{

  std::vector<D3DXVECTOR3> points (mNumSegments*mTesellationSegment);
  for ( int i = 0 ; i < mNumSegments ; ++i )
  {
    D3DXVECTOR3 p0 = mPoints[3*i];
    D3DXVECTOR3 p1 = mPoints[3*i + 1];
    D3DXVECTOR3 p2 = mPoints[3*i + 2];
    D3DXVECTOR3 p3 = mPoints[3*i + 3];
    for ( int j = 0 ; j < mTesellationSegment ; ++j )
    {
      float t = float(j) / float(mTesellationSegment - 1);
      points[i*mTesellationSegment + j]  =
          t*t*t*(p3 + 3.0f*(p1 - p2) - p0) +
          3*t*t*(p0 - 2.0f*p1 + p2) +
          3*t*  (p1 - p0) +
          p0;
    }
  }

  return points;
}

int Bezier::getNumPoints()
{
  return mNumPoints;
}

D3DXVECTOR3 Bezier::getPoint( int aIndex )
{
  return mPoints[ indexOfPoint(aIndex) ];
}

void Bezier::setPoint(int aIndex, const D3DXVECTOR3& p)
{
  mPoints[ indexOfPoint(aIndex) ] = p;
}


void Bezier::setPointForceTangentContinuity(int aIndex, const D3DXVECTOR3& p)
{
  D3DXVECTOR3 t0rel = mPoints[ indexOfPoint(aIndex) - 1] - mPoints[ indexOfPoint(aIndex) ];
  D3DXVECTOR3 t1rel = mPoints[ indexOfPoint(aIndex) + 1] - mPoints[ indexOfPoint(aIndex) ];

  D3DXVECTOR3 difference = p - mPoints[ indexOfPoint(aIndex) ];
  mPoints[ indexOfPoint(aIndex) ] = p;
/*
  mPoints[ indexOfPoint(aIndex) - 1] = t0rel + p;
  mPoints[ indexOfPoint(aIndex) + 1] = t1rel + p;*/
  mPoints[ indexOfPoint(aIndex) - 1] += difference;
  mPoints[ indexOfPoint(aIndex) + 1] += difference;
}

int Bezier::indexOfPoint( int aIndex )
{
  return (3*aIndex);
}

/*
  \param  aIndex  index of bezier point
  \param  aPrevTangent = true <=> will modify the previous tangent
*/
void Bezier::setTangent(int aIndex, bool aPreviousTangent, const D3DXVECTOR3& p)
{
  if (aPreviousTangent)
    mPoints[ indexOfPoint(aIndex) - 1 ] = p;
  else
    mPoints[ indexOfPoint(aIndex) + 1 ] = p;
}

/*
  \param  aIndex  index of bezier point
  \param  aPrevTangent = true <=> will modify the previous tangent
*/
void Bezier::setTangentCorrectOpposite(int aIndex, bool aPreviousTangent, const D3DXVECTOR3& p)
{
  if (aPreviousTangent)
  {
    D3DXVECTOR3 opposite = (
          mPoints[ indexOfPoint(aIndex)] +
          ( -1.0f* (mPoints[ indexOfPoint(aIndex) - 1] - mPoints[ indexOfPoint(aIndex)]))
          );
    mPoints[ indexOfPoint(aIndex) - 1 ] = p;
    mPoints[ indexOfPoint(aIndex) + 1 ] = opposite;
  }
  else
  {
    D3DXVECTOR3 opposite = (
          mPoints[ indexOfPoint(aIndex)] +
          ( -1.0f* (mPoints[ indexOfPoint(aIndex) + 1] - mPoints[ indexOfPoint(aIndex)]))
          );
    mPoints[ indexOfPoint(aIndex) + 1 ] = p;
    mPoints[ indexOfPoint(aIndex) - 1 ] = opposite;
  }
}



/*
    For display!!
*/
std::vector<D3DXVECTOR3> Bezier::getTangents( int aIndex )
{
  std::vector<D3DXVECTOR3> points (3);

  int index =  indexOfPoint(aIndex);
  points[0] = mPoints[index - 1];
  points[1] = mPoints[index];
  points[2] = mPoints[index + 1];

  return points;
}

/*
    For modify!
*/
std::vector<D3DXVECTOR3> Bezier::getTangentPoints( int aIndex )
{
  std::vector<D3DXVECTOR3> points (3);

  int index =  indexOfPoint(aIndex);
  points[0] = mPoints[index - 1];
  points[1] = mPoints[index + 1];

  return points;
}


void Bezier::exportToCLast(QTextStream &aStream)
{
  if ( mNumSegments >= 1)
  {
    int numelems = 1 + 3*mNumSegments;
    //last element
    aStream << "  D3DXVECTOR3( " << mPoints[numelems - 1].x << " , ";
    aStream << mPoints[numelems - 1].y << " , " << mPoints[numelems - 1].z << " )";
  }
}

void Bezier::exportToC(QTextStream &aStream)
{
  if ( mNumSegments >= 1)
  {
    int numelems = 1 + 3*mNumSegments;
    aStream << "D3DXVECTOR3 Spline"<<mUniqueId<<" [" << numelems << "] = {"<< endl;
    for ( int i = 0 ; i < (numelems - 1) ; ++i )
    {
      aStream << "  D3DXVECTOR3( " << mPoints[i].x << " , ";
      aStream << mPoints[i].y << " , " << mPoints[i].z << " ), " << endl;
    }
    //last element
    aStream << "  D3DXVECTOR3( " << mPoints[numelems - 1].x << " , ";
    aStream << mPoints[numelems - 1].y << " , " << mPoints[numelems - 1].z << " )"<< endl;
    //end array
    aStream << " };" << endl << endl;
  }
}

void Bezier::translate(const D3DXVECTOR3 &aVector)
{
  for ( int i = 0 ; i < mPoints.size() ; ++i )
  {
    mPoints[i] += aVector;
  }
}

void Bezier::deletePoint(const int aIndex )
{
  if ( aIndex == 0)
  {
    //  First point
    mPoints.erase (
          mPoints.begin() ,
          mPoints.begin() + 3);
  }
  else
    if ( aIndex == mNumPoints )
    {
      int index = indexOfPoint(aIndex);
      //  final
      mPoints.erase (
            mPoints.begin() + index - 2,
            mPoints.begin() + index + 1  );
    }
    else
    {
      int index = indexOfPoint(aIndex);
      //  Any point in-between
      mPoints.erase (
            (mPoints.begin() + index - 1 ),
            mPoints.begin() + index + 2);
    }

  --mNumSegments;
  --mNumPoints;
}

void Bezier::close()
{
  if (getNumPoints() > 2)
  {
    D3DXVECTOR3 p = getPoint(getNumPoints() - 1);
    setPoint( 0, p);
  }
}

//  \return   total number of elements to be stored. Not to confuse with num Points
int Bezier::getNumStoredElements()
{
  return ( 1 + 3*mNumSegments);
}



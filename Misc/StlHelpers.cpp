#include  "Misc/StlHelpers.hpp"

namespace StlHelpers {

bool  findInVector    ( const std::vector<int>& mVec, const int aVal, int& aIndex )
{
  for ( aIndex = 0; aIndex < mVec.size() ; ++aIndex )
  if ( mVec[aIndex] == aVal )
    return true;

  return false;
}

bool  isInVector( const std::vector<int>& mVec, const int aVal )
{
  for ( int i = 0; i < mVec.size() ; ++i )
    if ( mVec[i] == aVal )
      return true;
  return false;
}

void  removeFromVector( std::vector<int>& mVec, const int aVal )
{
  for ( int i = 0; i < mVec.size() ; ++i )
    if ( mVec[i] == aVal )
    {
      mVec.erase (mVec.begin() + i);
      return;
    }
}

};
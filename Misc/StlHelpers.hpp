#ifndef _STL_HELPERS_HPP_
#define _STL_HELPERS_HPP_

#include <vector>
#include <cmath>

namespace StlHelpers {

  bool  findInVector    ( const std::vector<int>& mVec, const int aVal, int& aIndex );
  bool  isInVector      ( const std::vector<int>& mVec, const int aVal );
  void  removeFromVector( std::vector<int>& mVec, const int aVal );

};
   
#endif  //  _STL_HELPERS_HPP_

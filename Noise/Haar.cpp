#include  "Noise/haar.hpp"

namespace Signals{

 int   log2integer ( int aNumber )
  {
    int log2 = 0;
    --aNumber ;
    while ( aNumber != 0)
    {
      aNumber = aNumber >> 1;
      ++log2;
    }
    return log2;
  }

 int   exp2integer ( int aExp )
 {
   return ( 1 << (aExp) );
 }
};
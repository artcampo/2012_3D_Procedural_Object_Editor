#ifndef _RANDOM_NUMBER_GENERATOR_
#define _RANDOM_NUMBER_GENERATOR_

#include <cstdlib>
#include <vector>

#include		"Noise/Mersenne.hpp"

namespace Rand{

class RandGen
{
  public:
    RandGen(){};
    RandGen( const unsigned long aSeed );

    float   randFloat_0_1();
    int     randInt( int aMax );
    unsigned long randLong();

    std::vector<int>  genPermutations( int aMax );
    void              genPermutations( std::vector<int>& aVector, int aMax );
  private:
    Mersenne::Mersenne     mMersenne;

};

}

#endif

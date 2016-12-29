#include  "Noise/RandomNumberGenerator.hpp"

using namespace Rand;

RandGen::RandGen( const unsigned long aSeed )
{
  mMersenne.init_genrand( aSeed );
}

unsigned long RandGen::randLong()
{
  return mMersenne.genrand_int32();
}

float   RandGen::randFloat_0_1()
{	
  return mMersenne.rand1();
}

int RandGen::randInt( int aMax )
{
	return (mMersenne.genrand_int32() % aMax);	
}

std::vector<int> RandGen::genPermutations( int aMax )
{
  std::vector<int> ints ( aMax );

  for ( int i = 0; i < aMax ; ++i )
    ints[i] = i;

  for ( int i = aMax - 1 ; i > 1 ; --i )
  {
    int p = randInt( i ); 
    std::swap<int>( ints[p], ints[i] );
  }

  return ints;
}

void RandGen::genPermutations( std::vector<int>& aVector, int aMax )
{
  for ( int i = aMax - 1 ; i > 1 ; --i )
  {
    int p = randInt( i ); 
    std::swap<int>( aVector[p], aVector[i] );
  }
}

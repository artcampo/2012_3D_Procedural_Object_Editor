#ifndef _MERSENNE_
#define _MERSENNE_

#include <cstdlib>
#include <vector>

namespace Mersenne{

class Mersenne
{
  public:
    unsigned long mt[624];
    int mti;
   
    Mersenne(){};
    void init_genrand( const unsigned long s )
      {
        mt[0]= s & 0xffffffffUL;
        for (mti=1; mti<624; mti++) {
            mt[mti] = 
          (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
            mt[mti] &= 0xffffffffUL;            
        }
      };
    unsigned long genrand_int32(void)
    {
      unsigned long y;
      static unsigned long mag01[2]={0x0UL, 0x9908b0dfUL};
      /* mag01[x] = x * MATRIX_A  for x=0,1 */

      if (mti >= 624) { /* generate 624 words at one time */
          int kk;

          for (kk=0;kk<624-397;kk++) {
              y = (mt[kk]&0x80000000UL)|(mt[kk+1]&0x7fffffffUL);
              mt[kk] = mt[kk+397] ^ (y >> 1) ^ mag01[y & 0x1UL];
          }
          for (;kk<624-1;kk++) {
              y = (mt[kk]&0x80000000UL)|(mt[kk+1]&0x7fffffffUL);
              mt[kk] = mt[kk+(397-624)] ^ (y >> 1) ^ mag01[y & 0x1UL];
          }
          y = (mt[624-1]&0x80000000UL)|(mt[0]&0x7fffffffUL);
          mt[624-1] = mt[397-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

          mti = 0;
      }
    
      y = mt[mti++];

      /* Tempering */
      y ^= (y >> 11);
      y ^= (y << 7) & 0x9d2c5680UL;
      y ^= (y << 15) & 0xefc60000UL;
      y ^= (y >> 18);

      return y;
    };
    float rand1()
    {
        return genrand_int32()*(1.0/4294967295.0);         
    };

    /* generates a random number on [0,1)-real-interval */
    float randNot1(void)
    {
        return genrand_int32()*(1.0/4294967296.0); 
    };
   
};

}

#endif  //_MERSENNE_
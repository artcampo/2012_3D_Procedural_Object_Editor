// haarNoise.cpp: define el punto de entrada de la aplicación de consola.
//

#include  "Noise/Haar.hpp"
#include <cmath>
#include <iostream>
using namespace std;

class Daubechies {
   private:
   /** forward transform scaling coefficients */
   float h0, h1, h2, h3;
   /** forward transform wave coefficients */
   float g0, g1, g2, g3;

   float Ih0, Ih1, Ih2, Ih3;
   float Ig0, Ig1, Ig2, Ig3;

   /**
     Forward Daubechies D4 transform
    */
   void transform( float* a, const int n )
   {
      if (n >= 4) {
         int i, j;
         const int half = n >> 1;

         float* tmp = new float[n];

         for (i = 0, j = 0; j < n-3; j += 2, i++) {
            tmp[i]      = a[j]*h0 + a[j+1]*h1 + a[j+2]*h2 + a[j+3]*h3;
            tmp[i+half] = a[j]*g0 + a[j+1]*g1 + a[j+2]*g2 + a[j+3]*g3;
         }

         tmp[i]      = a[n-2]*h0 + a[n-1]*h1 + a[0]*h2 + a[1]*h3;
         tmp[i+half] = a[n-2]*g0 + a[n-1]*g1 + a[0]*g2 + a[1]*g3;

         for (i = 0; i < n; i++) {
            a[i] = tmp[i];
         }
         delete [] tmp;
      }
   }

   /**
     Inverse Daubechies D4 transform
    */
   void invTransform( float* a, const int n )
   {
     if (n >= 4) {
       int i, j;
       const int half = n >> 1;
       const int halfPls1 = half + 1;

       float* tmp = new float[n];

       //      last smooth val  last coef.  first smooth  first coef
       tmp[0] = a[half-1]*Ih0 + a[n-1]*Ih1 + a[0]*Ih2 + a[half]*Ih3;
       tmp[1] = a[half-1]*Ig0 + a[n-1]*Ig1 + a[0]*Ig2 + a[half]*Ig3;
       for (i = 0, j = 2; i < half-1; i++) {
         //     smooth val     coef. val       smooth val    coef. val
         tmp[j++] = a[i]*Ih0 + a[i+half]*Ih1 + a[i+1]*Ih2 + a[i+halfPls1]*Ih3;
         tmp[j++] = a[i]*Ig0 + a[i+half]*Ig1 + a[i+1]*Ig2 + a[i+halfPls1]*Ig3;
       }
       for (i = 0; i < n; i++) {
         a[i] = tmp[i];
       }
       delete [] tmp;
     }
   }


   public:

   Daubechies()
   {
      const float sqrt_3 = sqrt( 3.0f );
      const float denom = 4 * sqrt( 2.0f );

      //
      // forward transform scaling (smoothing) coefficients
      //
      h0 = (1 + sqrt_3)/denom;
      h1 = (3 + sqrt_3)/denom;
      h2 = (3 - sqrt_3)/denom;
      h3 = (1 - sqrt_3)/denom;
      //
      // forward transform wavelet coefficients
      //
      g0 =  h3;
      g1 = -h2;
      g2 =  h1;
      g3 = -h0;

      Ih0 = h2;
      Ih1 = g2;  // h1
      Ih2 = h0;
      Ih3 = g0;  // h3

      Ig0 = h3;
      Ig1 = g3;  // -h0
      Ig2 = h1;
      Ig3 = g1;  // -h2
   }

   void daubTrans( float* ts, int N )
   {
      int n;
      for (n = N; n >= 4; n >>= 1) {
         transform( ts, n );
      }
   }


   void invDaubTrans( float* coef, int N )
   {
      int n;
      for (n = 4; n <= N; n <<= 1) {
         invTransform( coef, n );
      }
   }

}; // Daubechies


int  haarNoise1d_2( int aValue )
{
  int   numBits = 31;
  std::vector<float> signal (numBits + 1);    
  std::vector<float> signalReconstructed(numBits + 1);  

  signal[31] = 0;
  
  int value = aValue;
  //16
  signal[0] = value & 31;
  value >>= 5;

  //8
  signal[1] = value & 15;
  signal[2] = value & 15;
  value >>= 4;

  //4
  signal[3] = value & 7;
  signal[4] = value & 7;
  signal[5] = value & 7;
  signal[6] = value & 7;
  value >>= 3;

  //2
  signal[7] = value & 3;
  signal[8] = value & 3;
  signal[9] = value & 3;
  signal[10] = value & 3;
  signal[11] = value & 3;
  signal[12] = value & 3;
  signal[13] = value & 3;
  signal[14] = value & 3;
  value >>= 2;

  //1
  signal[15] = value & 1;
  signal[16] = value & 1;
  signal[17] = value & 1;
  signal[18] = value & 1;
  signal[19] = value & 1;
  signal[20] = value & 1;
  signal[21] = value & 1;
  signal[22] = value & 1;
  signal[23] = value & 1;
  signal[24] = value & 1;
  signal[25] = value & 1;
  signal[26] = value & 1;
  signal[27] = value & 1;
  signal[28] = value & 1;
  signal[29] = value & 1;
  signal[30] = value & 1;


  Haar::haar1Du<float> ( signal, signalReconstructed );

  float retValue = 0;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    retValue += fabs(signalReconstructed[numBits - i]);
  }

  return retValue;
}

int  haarNoise1d( int aValue )
{
  int   numBits = 31;
  std::vector<float> signal (numBits + 1);    
  std::vector<float> signalReconstructed(numBits + 1);  

  signal[31] = 0;
  
  int value = aValue;
  for ( int i = 0; i < numBits; ++i )
  {
    signal[i] = value & 1;
    value = value >> 1;
  }

  bool useHaar = false;
  if (useHaar)
    Haar::haar1Du<float> ( signal, signalReconstructed );
  else
  {
    Daubechies daub;
    daub.invDaubTrans( &signal[0], numBits + 1);
    for ( int i = 0; i < 64; ++i )
        signalReconstructed[i] = signal[i];
  }

  float retValue = 0;
  for ( int i = numBits - 1; i >= 0; --i )
  {
    retValue += fabs(signalReconstructed[numBits - i]);
  }

  return retValue;
}

int  haarNoise2d( int aValue1,int aValue2)
{
  int   numBits = 31;
  std::vector<float> signal (numBits*2 + 2);    
  std::vector<float> signalReconstructed(numBits*2 + 2);  

  signal[62] = 0;
  signal[63] = 0;
  
  int value1 = aValue1;
  int value2 = aValue2;
  for ( int i = 0; i < numBits; ++i )
  {
    
    signal[i*2] = (value1 & 3) + ( value2 & 3);
    value1 = value1 >> 2;
    
    value2 = value2 >> 2;
  }

  bool useHaar = false;
  if (useHaar)
    Haar::haar1Du<float> ( signal, signalReconstructed );
  else
  {
    Daubechies daub;
    daub.invDaubTrans( &signal[0], numBits + 1);
    for ( int i = 0; i < 64; ++i )
        signalReconstructed[i] = signal[i];
  }


  float retValue = 0;
  for ( int i = 0; i < 64; ++i )
  {
    retValue += fabs(signalReconstructed[63-i]);
    //retValue += (signalReconstructed[i]*0.5 + 0.5) ;
  }

  return retValue;
}


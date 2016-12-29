#ifndef _HAAR_HPP_
#define _HAAR_HPP_

#include <vector>


namespace Haar{
  template <class Scalar>
  void  haar1Dt ( std::vector<Scalar>& aSignal, std::vector<Scalar>& aSignalOut )
  {
    double sq2d2 = sqrt(2.0)/2.0;
    int size = Signals::extendLengthPower2<Scalar> ( aSignal );
    aSignalOut.resize( size );

    for ( int i = 0 ; i < size ; ++i )
      aSignalOut[i] = aSignal[i];

    for ( int level = Signals::log2integer( size ); level > 0 ; --level )
    {
      int sizeTemp = Signals::exp2integer(level);
      std::vector<Scalar> signalTemp;
      signalTemp.resize( sizeTemp );

      int j = 0;
      for ( int i = 0 ; i < sizeTemp ; i=(i + 2), ++j )
      {
        Scalar s = Scalar ( (aSignalOut[i] + aSignalOut[i + 1])*sq2d2 );
        Scalar d = Scalar ( (aSignalOut[i] - aSignalOut[i + 1])*sq2d2 );
        signalTemp[j] = s;
        signalTemp[j + (sizeTemp/2)] = d;
      }

      for ( int i = 0 ; i < sizeTemp ; ++i )
        aSignalOut[i] = signalTemp[i];
    }
  }
  
 template <class Scalar>
  void  haar1Du ( std::vector<Scalar>& aSignal, std::vector<Scalar>& aSignalOut )
  {
    double sq2d2 = sqrt(2.0)/2.0;
    double sq2 = sqrt(2.0);
    int size = Signals::extendLengthPower2<Scalar> ( aSignal );
    aSignalOut.resize( size );

    for ( int i = 0 ; i < size ; ++i )
      aSignalOut[i] = aSignal[i];

    int maxLevel = Signals::log2integer( size );
    for ( int level = 1; level <= maxLevel ; ++level )
    {
      int sizeTemp = Signals::exp2integer(level);
      std::vector<Scalar> signalTemp;
      signalTemp.resize( sizeTemp );

      int j = 0;
      for ( int i = 0 ; i < sizeTemp ; i=(i + 2), ++j )
      {
        Scalar s = aSignalOut[j];
        Scalar d = aSignalOut[j + (sizeTemp/2)];
        Scalar l = Scalar((s + d) / sq2);
        Scalar r = Scalar((s - d) / sq2);
        signalTemp[i] = l;
        signalTemp[i + 1] = r;
      }

      for ( int i = 0 ; i < sizeTemp ; ++i )
        aSignalOut[i] = signalTemp[i];
    }
  }
};

namespace Signals{
  template <class Scalar>
  int   extendLengthPower2 ( std::vector<Scalar>& aSignal )
  {
    int size = aSignal.size();
    return size;
  }

  int   log2integer ( int aNumber );
  int   exp2integer ( int aExp );

};


#endif

#ifndef _SIGNAL_MARBLE_HPP_
#define _SIGNAL_MARBLE_HPP_

#pragma once
#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include "Editor_Texture/DataLayer/Signal.hpp"

#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;


//===========================================================================
/*!
      Signal.
      \brief      ..
*/
//===========================================================================
class SignalMarble : public Signal
{
public:

  SignalMarble ( int aId );
  SignalMarble ( string aName );

  float   turbulence();
  void    setTurbulence( float aTurbulence );

private:
  void  defaultParametersDerived();

private:

  float   mTurbulence;
  //  IO stuff
  bool  writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal );

  friend class ProcShader;
  //  IO
public:
  void save( QDataStream& aStream );
  static Signal*  loadL( QDataStream& aStream );
};


////////////////////////////////////////////////////////////////////////////////////////////

namespace SignalMarbleData{

  const std::string signal1 =
 "noiseT =  sqrt(abs(cos( 3*noiseIn.x + 2*value ) ) ); ";


};

#endif

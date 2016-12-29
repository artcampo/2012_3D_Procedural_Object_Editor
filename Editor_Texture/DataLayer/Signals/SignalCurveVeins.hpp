#ifndef _SIGNAL_CURVE_VEINS_HPP_
#define _SIGNAL_CURVE_VEINS_HPP_

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
class SignalCurveVeins : public Signal
{
public:

  SignalCurveVeins ( int aId );
  SignalCurveVeins ( string aName );

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

namespace SignalCurveVeinsData{

  const std::string signal1 =
"\n  noiseIn = ";
  const std::string signal2 = "*";
  const std::string signal3 = ";\n";

  const std::string signal3x =
"  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));\n";
  const std::string signal3y =
"  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));"
"\n";

  const std::string signal4 =
      "  noiseT *= 0.5f;\n"
      "  noiseT = clamp( noiseT, 0, 1 );\n";

};

#endif

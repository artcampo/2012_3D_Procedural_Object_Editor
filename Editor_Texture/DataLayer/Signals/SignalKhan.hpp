#ifndef _SIGNAL_KHAN1_HPP_
#define _SIGNAL_KHAN1_HPP_

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
class SignalKhan1 : public Signal
{
public:

  SignalKhan1 ( int aId );
  SignalKhan1 ( string aName );

  void getNoiseParams( float& aFreq1, float& aFreq2, float& aThreshold );
  void setNoiseParams( float aFreq1, float aFreq2, float aThreshold );

private:
    float       mFreq1;
    float       mFreq2;
    float     mThreshold;

private:
    void  defaultParametersDerived();

private:

  //  IO stuff
  bool  writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal );

  friend class ProcShader;
  //  IO
public:
  void save( QDataStream& aStream );
  static Signal*  loadL( QDataStream& aStream );
};


////////////////////////////////////////////////////////////////////////////////////////////

namespace SignalKhan1Data{


  const std::string signal5_1 =
"  value = fmod( length( ";

  const std::string signal5_2 =
") , length(noiseIn* ";

  const std::string signal5_3 =
" ) ) ;\n"
"  if( value < ";


  const std::string signal5_4 =
" )\n"
"    noiseT += 1 -  2*abs( ( value /";

  const std::string signal5_5 =
") - 0.5 );\n";

  const std::string signal6_1 =
"  noiseT = clamp( noiseT, 0, 1 );\n";

  const std::string signal6_2 =
" .xyz += lerp( noiseColor1.xyz, noiseColor2.xyz, noiseT );\n ";

};

#endif

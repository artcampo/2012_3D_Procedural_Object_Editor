#ifndef _SIGNAL_STRIPS_HPP_
#define _SIGNAL_STRIPS_HPP_

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
class SignalStrips : public Signal
{
public:

  SignalStrips ( int aId );
  SignalStrips ( string aName );

  void getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold);
  void setNoiseParams( D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold );


private:
  D3DXVECTOR3   mFreq1;
  D3DXVECTOR3   mPhase;
  float         mThreshold;

private:  
  void  defaultParametersDerived();
  float px();
  float py();
  float pz();

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

namespace SignalStripsData{

const std::string signal1 =
"\n  noiseIn = ";
const std::string signal2 = "*";

const std::string signal3_1 =
"  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + ";

  const std::string signal3_2 =
" + Snoise3D( ";

  const std::string signal3_3 =
" *noiseIn )));\n"
"  if ( value < ";

  const std::string signal3_4 =
")\n"
"    noiseT += 1 - ( (value/";

  const std::string signal3_5 =
")*0.5 + 0.5 );\n";


};

#endif

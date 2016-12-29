#ifndef _SIGNAL_LINES_HPP_
#define _SIGNAL_LINES_HPP_

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
class SignalLines : public Signal
{
public:

  SignalLines ( int aId );
  SignalLines ( string aName );

  void getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold);
  void setNoiseParams( D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold );

private:
  D3DXVECTOR3   mFreq1;
  D3DXVECTOR3   mPhase;
  float       mThreshold;

private:
  void  defaultParametersDerived();

private:

  float px();
  float py();
  float pz();
  D3DXVECTOR3 threshold();


  //  IO stuff
  bool  writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal );

  friend class ProcShader;
  //  IO
public:
  void save( QDataStream& aStream );
  static Signal*  loadL( QDataStream& aStream );
};


////////////////////////////////////////////////////////////////////////////////////////////

namespace SignalLinesData{

  const std::string signal1 =
  "\n  noiseIn = ";
  const std::string signal2 = "*";
  const std::string signal3 =
      ";\n"
      "  vectorModulus = ";

  // "noiseIn" used to be "vectorValue"
  const std::string signal4_1 =
      "  noiseT = 0;\n";

  const std::string signal5_0_1 =
"  vectorModulus *= ";

  //old:
  const std::string signal5_0_3 =
"  noiseIn *= ";

   const std::string signal5_1 =
"  vectorValue = fmod( abs(noiseIn), vectorModulus );\n"
"  vectorValue -= 0.5*";

  const std::string signal5_2 =
"  vectorValue = abs(vectorValue);\n"
"  vectorValue = max( vectorValue, ";

  const std::string signal5_3 =
"  vectorValue = ( ";
  const std::string signal5_4 =
" / vectorValue);\n"
"  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;\n";


};

#endif

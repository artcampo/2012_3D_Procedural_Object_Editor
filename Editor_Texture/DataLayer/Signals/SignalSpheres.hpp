#ifndef _SIGNAL_SPHERES_HPP_
#define _SIGNAL_SPHERES_HPP_

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
class SignalSpheres : public Signal
{
public:

  SignalSpheres ( int aId );
  SignalSpheres ( string aName );

  void getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold);
  void setNoiseParams( D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold );


private:
  D3DXVECTOR3   mFreq1;
  D3DXVECTOR3   mPhase;
  float         mThreshold;

private:
  void  defaultParametersDerived();

  D3DXVECTOR3 threshold();

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

namespace SignalSpheresData{

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
"  noiseT += (1-length ( vectorValue ))/";


};

#endif

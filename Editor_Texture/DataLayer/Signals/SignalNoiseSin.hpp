#ifndef _SIGNAL_NOISESIN_HPP_
#define _SIGNAL_NOISESIN_HPP_

#pragma once
#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include "Editor_Texture/DataLayer/ProcShaderMisc.hpp"
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
class SignalNoiseSin : public Signal
{
public:

  SignalNoiseSin ( int aId );
  SignalNoiseSin ( string aName );

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



namespace SignalNoiseSinData{

  const std::string signal1 =
"  noiseT = abs( noiseIn.x + value );\n";

};


#endif

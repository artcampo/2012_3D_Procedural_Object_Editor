#ifndef _SIGNAL_IDENTITY_HPP_
#define _SIGNAL_IDENTITY_HPP_

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
class SignalIdentity : public Signal
{
public:

  SignalIdentity ( int aId );
  SignalIdentity ( string aName );

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

namespace SignalIdentityData{


  const std::string signal1 =
"\n  noiseIn = ";
  const std::string signal2 = "*";
  const std::string signal3 = ";\n";

};

#endif

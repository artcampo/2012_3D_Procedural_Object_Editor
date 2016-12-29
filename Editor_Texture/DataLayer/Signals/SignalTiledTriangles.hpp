#ifndef _SIGNAL_TILED_TRIANGLES_HPP_
#define _SIGNAL_TILED_TRIANGLES_HPP_

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
class SignalTiledTriangles : public Signal
{
public:

  SignalTiledTriangles ( int aId );
  SignalTiledTriangles ( string aName );

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

namespace SignalTiledTrianglesData{


};

#endif  //_SIGNAL_TILED_TRIANGLES_HPP_

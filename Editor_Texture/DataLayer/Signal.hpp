#ifndef _SIGNAL_HPP_
#define _SIGNAL_HPP_

#pragma once
#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include "Editor_Texture/DataLayer/ProcShaderMisc.hpp"
#include "Editor_Texture/DataLayer/IO/ProcShaderDisk.hpp"

#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;

// qt-io
#include <QDataStream>
#include <QTextStream>

class Signal;



//===========================================================================
/*!
      Signal.
      \brief      ..
*/
//===========================================================================
class Signal
{
public:

  Signal ( int aId, int aSignalType );
  Signal ( string aName, int aSignalType );

  void    defaultParams();
  virtual void defaultParametersDerived() = 0;

  //virtual ~Signal() = 0;

  int     signalType();
  void    setName ( string aName );  
  void    setParams ( int aInpParams, D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul );

  void    setDefaultValues();

  string      getName ()  const;
  void        getParams ( int& aInpParams, D3DXVECTOR3& aW, int& aNumOctaves, float& aOctavesMul );


  bool      hasWarp()     const;
  float     warpLength()  const;
  string    warpSignal()  const;
  void      setWarp( string& aWarpSignalName, float aWarpLength, bool aWarpInUse );

  //  IO - write shader
public:  
  bool virtual writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )  = 0;
public:
  bool    writeParams     ( bool aIsBumpFunction, std::ofstream& aFile );
  bool    writeParamsAtom ( bool aIsBumpFunction, std::ofstream& aFile );

  std::string prependChannelName( string& aName );

protected:
  string        mName;              /*!< Name */
  int           mSignalType;        /*!< pertains to eSingalTypes */

  D3DXVECTOR3   mW;           /*!< Ponderation of input parameters */
  int           mNumOctaves;  /*!< Number of octaves*/
  float         mOctavesMul;  /*!< Multiplier to get next octave*/
  int           mInputParams; /*!< Identifier of input parameters, pertains to eIdInputParams*/

  bool          mHasWarp;
  float         mWarpLength;
  string        mWarpSignalName;

  friend class ProcShader;

  //  IO
public:
  virtual void   save( QDataStream& aStream ) = 0;
  static Signal*  load( QDataStream& aStream );
  static Signal*  loadL( QDataStream& aStream );

public:
  static const std::string msInputParametersNames[];
  static const int msNumInputParameters = 7;
  static const std::string msSignalTypesNames[];
  static const int msNumSignalType = 10;

  enum eIdInputParams{
    eIdPos_xyz,
    eIdPos_xxx,
    eIdPos_yzx,
    eIdPos_zxy,
    eIdSph_xyz,
    eIdUV_uvu,
    eIdNor_xyz
  };

  enum eSingalTypes{
    eIdSignalNoise,
    eIdSignalNoiseSin,
    eIdSignalStrips,
    eIdSignalLines,
    eIdSignalKhan1,
    eIdSignalMarble,
    eIdSignalCurveVeins,
    eIdSignalSpheres,
    eIdSignalTiledTriangles,
    eIdSignalIdentity
  };

  //  IO  - to disk

  struct SignalData{
    std::string signalName;
    D3DXVECTOR3 mW;
    int mNumOctaves;
    float mOctavesMul;
    int mInputParams;
  };

  struct SignalData2{
    std::string signalName;

  };

  //  IO - write to file  (for all signals)
  void                signalIOWrite ( QDataStream &aStream );
  static SignalData   signalIORead  ( QDataStream &aStream );

  //  IO - for specific signals
  static void signalIOWrite ( QDataStream&        aStream,
                              const D3DXVECTOR3&  mFreq1,
                              const D3DXVECTOR3&  mPhase,
                              const float&        mThreshold );
  static void signalIORead  ( QDataStream&  aStream,
                              D3DXVECTOR3&  mFreq1,
                              D3DXVECTOR3&  mPhase,
                              float&        mThreshold);


};

class SignalNoise;
class SignalNoiseSin;
class SignalStrips;
class SignalLines;
class SignalKhan;
class SignalMarble;
class SignalCurveVeins;
class SignalSpheres;
class SignalTiledTriangles;
class SignalIdentity;

#endif

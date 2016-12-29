#include      "Editor_Texture/DataLayer/Signal.hpp"
#include      "Exterior/stringTokenizer.cpp"

const std::string Signal::msInputParametersNames[]=
{
  "PosOBJ xyz",
  "PosOBJ xxx",
  "PosOBJ yzx",
  "PosOBJ zxy",
  "SphOBJ xyz",
  "UvOBJ uvu",
  "NorWRD xyz"
};

const std::string Signal::msSignalTypesNames[]=
{
  "Noise",
  "NoiseSin",
  "Strips",
  "Lines",
  "Khan-1",
  "Marble",
  "Curve Veins",
  "Spheres / dots",
  "Tiled Triangles",
  "Identity"
};

#include "Editor_Texture/DataLayer/Signals/SignalNoise.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalNoiseSin.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalStrips.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalLines.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalKhan.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalMarble.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalCurveVeins.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalSpheres.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalTiledTriangles.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalIdentity.hpp"

//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

Signal::Signal( int aId, int aSignalType )
{
  defaultParams();
 mSignalType = aSignalType;
 mName = "Signal " + Misc::convertInt(aId);
 mHasWarp = false;
 mWarpLength = 0.12;
 mWarpSignalName = string("none");
}

Signal::Signal ( string aName, int aSignalType )
{
  defaultParams();
  mSignalType = aSignalType;
  mName = aName;
  mHasWarp = false;
  mWarpLength = 0.12;
  mWarpSignalName = string("none");
}


void Signal::setName ( string aName )
{
  mName = aName;
}


string  Signal::getName () const
{
  return mName;
}

//===========================================================================
/*!
    Set general parameters of signal

    \param    aW            ponderation to each input parameter
    \param    aNumOctaves   number of octaves
    \param    aOctavesMul   multiplier between octaves
*/
//===========================================================================
void Signal::setParams ( int aInpParams, D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul )
{
  mInputParams  = aInpParams;
  mW            = aW;
  mNumOctaves   = aNumOctaves;
  mOctavesMul   = aOctavesMul;
}


void Signal::getParams ( int &aInpParams, D3DXVECTOR3& aW, int& aNumOctaves, float& aOctavesMul )
{
  aInpParams  = mInputParams;
  aW          = mW;
  aNumOctaves = mNumOctaves;
  aOctavesMul = mOctavesMul;
}

int Signal::signalType()
{
  return mSignalType;
}

void Signal::defaultParams()
{
  mW[0] = 0.2f;
  mW[1] = 0.2f;
  mW[2] = 0.2f;
  mNumOctaves = 1;
  mOctavesMul = 2.0f;  
}

Signal* Signal::load(QDataStream &aStream)
{
  int id;
  aStream >> id;
  //ADD: for each signal
  switch(id)
  {
  case Signal::eIdSignalNoise:
    return SignalNoise::loadL(aStream);
  case Signal::eIdSignalKhan1:
    return SignalKhan1::loadL(aStream);
  case Signal::eIdSignalStrips:
    return SignalStrips::loadL(aStream);
  case Signal::eIdSignalCurveVeins:
    return SignalCurveVeins::loadL(aStream);
  case Signal::eIdSignalLines:
    return SignalLines::loadL(aStream);
  case Signal::eIdSignalMarble:
    return SignalMarble::loadL(aStream);
  case Signal::eIdSignalSpheres:
    return SignalSpheres::loadL(aStream);
  case Signal::eIdSignalTiledTriangles:
    return SignalTiledTriangles::loadL(aStream);
  case Signal::eIdSignalIdentity:
    return SignalIdentity::loadL(aStream);
  };
}

std::string Signal::prependChannelName(std::string &aName)
{
  //string::size_type position = str.find("Ben");

  // Split "channel::signal"
  vector<string> tokens;
  Tokenizer::Tokenize(mName, tokens, "::");

  std::string s;
  if ( tokens.size() == 1)
    s = aName + "::" + mName;
  else
    s = aName + "::" + tokens[1];
  return s;
}

void Signal::setDefaultValues()
{
  defaultParams();
  defaultParametersDerived();
}

void  Signal::signalIOWrite ( QDataStream &aStream )
{
  aStream << QString::fromStdString( mName );  

  aStream << mW.x;
  aStream << mW.y;
  aStream << mW.z;

  aStream << mNumOctaves;
  aStream << mOctavesMul;
  aStream << mInputParams;

  bool    warpPresent = false;
  aStream << warpPresent;
}

Signal::SignalData Signal::signalIORead(QDataStream &aStream)
{
  SignalData data;
  QString s;
  aStream >> s;
  data.signalName = s.toStdString();

  aStream >> data.mW.x;
  aStream >> data.mW.y;
  aStream >> data.mW.z;

  aStream >> data.mNumOctaves;
  aStream >> data.mOctavesMul;
  aStream >> data.mInputParams;

  bool    warpPresent;
  aStream >> warpPresent;

  return data;
}


void Signal::signalIOWrite ( QDataStream&        aStream,
                            const D3DXVECTOR3&  mFreq1,
                            const D3DXVECTOR3&  mPhase,
                            const float&        mThreshold )
{
  aStream << mFreq1.x;
  aStream << mFreq1.y;
  aStream << mFreq1.z;

  aStream << mPhase.x;
  aStream << mPhase.y;
  aStream << mPhase.z;

  aStream << mThreshold;
}

void Signal::signalIORead  ( QDataStream&  aStream,
                            D3DXVECTOR3&  mFreq1,
                            D3DXVECTOR3&  mPhase,
                            float&        mThreshold)
{
  aStream >> mFreq1.x;
  aStream >> mFreq1.y;
  aStream >> mFreq1.z;

  aStream >> mPhase.x;
  aStream >> mPhase.y;
  aStream >> mPhase.z;

  aStream >> mThreshold;
}


bool      Signal::hasWarp()     const
{
  return mHasWarp;
}

float     Signal::warpLength()  const
{
  return mWarpLength;
}

string    Signal::warpSignal()  const
{

  return mWarpSignalName;
}


void Signal::setWarp( string& aWarpSignalName, float aWarpLength, bool aWarpInUse )
{
  mHasWarp = aWarpInUse;
  mWarpLength = aWarpLength;
  mWarpSignalName = aWarpSignalName;
}

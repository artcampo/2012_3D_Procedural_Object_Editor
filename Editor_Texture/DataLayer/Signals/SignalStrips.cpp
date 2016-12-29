#include	"Editor_Texture/DataLayer/Signals/SignalStrips.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalStrips::SignalStrips( int aId ) : Signal( aId, eIdSignalStrips )
{
  mSignalType = Signal::eIdSignalStrips;
  defaultParams();
  defaultParametersDerived();
}

SignalStrips::SignalStrips ( string aName ) :
  Signal( aName, eIdSignalStrips )
{
  mSignalType = Signal::eIdSignalStrips;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalStrips::defaultParametersDerived()
{
  mFreq1 = D3DXVECTOR3(2,2,2);
  mPhase = D3DXVECTOR3(0,0,0);
  mThreshold = 0.3f;
}


void SignalStrips::setNoiseParams(D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold)
{
  mFreq1 = aFreq1;
  mPhase = aPhase;
  mThreshold = aThreshold;
}

void SignalStrips::getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold)
{
  aFreq1 = mFreq1;
  aPhase = mPhase;
  aThreshold = mThreshold;
}

float SignalStrips::px()
{
  return mPhase[0]*mFreq1[0];
}

float SignalStrips::py()
{
  return mPhase[1]*mFreq1[1];
}

float SignalStrips::pz()
{
  return mPhase[2]*mFreq1[2];
}

void SignalStrips::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalStrips);

  signalIOWrite( aStream );

  aStream << mFreq1.x;
  aStream << mFreq1.y;
  aStream << mFreq1.z;

  aStream << mPhase.x;
  aStream << mPhase.y;
  aStream << mPhase.z;

  aStream << mThreshold;
}

Signal * SignalStrips::loadL(QDataStream &aStream)
{  
  SignalData data = signalIORead ( aStream ) ;
  SignalStrips*  n = new SignalStrips( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  //specific
  aStream >> n->mFreq1.x;
  aStream >> n->mFreq1.y;
  aStream >> n->mFreq1.z;

  aStream >> n->mPhase.x;
  aStream >> n->mPhase.y;
  aStream >> n->mPhase.z;

  aStream >> n->mThreshold;

  return n;
}


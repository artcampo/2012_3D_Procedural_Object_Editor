#include	"Editor_Texture/DataLayer/Signals/SignalKhan.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalKhan1::SignalKhan1( int aId ) : Signal( aId, Signal::eIdSignalKhan1 )
{
  mSignalType = Signal::eIdSignalKhan1;
  defaultParams();
  defaultParametersDerived();
}

SignalKhan1::SignalKhan1 ( string aName ) :
  Signal( aName, eIdSignalKhan1 )
{
  mSignalType = Signal::eIdSignalKhan1;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalKhan1::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(2,10,10);
  mNumOctaves = 1;
  mOctavesMul = 0;
  mFreq1 = 1.8f;
  mFreq2 = 0.1f;
  mThreshold = 0.7f;
}


void SignalKhan1::setNoiseParams(float aFreq1, float aFreq2, float aThreshold)
{
  mFreq1 = aFreq1;
  mFreq2 = aFreq2;
  mThreshold = aThreshold;
}

void SignalKhan1::getNoiseParams(float &aFreq1, float &aFreq2, float &aThreshold)
{
  aFreq1 = mFreq1;
  aFreq2 = mFreq2;
  aThreshold = mThreshold;
}

void SignalKhan1::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalKhan1);

  signalIOWrite( aStream );

  aStream << mFreq1;
  aStream << mFreq2;
  aStream << mThreshold;
}

Signal * SignalKhan1::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalKhan1*  n = new SignalKhan1( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  //specific
  aStream >> n->mFreq1;
  aStream >> n->mFreq2;
  aStream >> n->mThreshold;

  return n;
}

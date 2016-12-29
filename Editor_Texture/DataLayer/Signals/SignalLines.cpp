#include	"Editor_Texture/DataLayer/Signals/SignalLines.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalLines::SignalLines( int aId ) : Signal( aId, Signal::eIdSignalLines )
{
  mSignalType = Signal::eIdSignalLines;
  defaultParams();
  defaultParametersDerived();
}

SignalLines::SignalLines ( string aName ) :
  Signal( aName, eIdSignalLines )
{
  mSignalType = Signal::eIdSignalLines;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalLines::defaultParametersDerived()
{  
  mFreq1 = D3DXVECTOR3(2,2,2);
  mPhase = D3DXVECTOR3(0,0,0);
  mThreshold = 0.1f;
}


void SignalLines::setNoiseParams(D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold)
{
  mFreq1 = aFreq1;
  mPhase = aPhase;
  mThreshold = aThreshold;
}

void SignalLines::getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold)
{
  aFreq1 = mFreq1;
  aPhase = mPhase;
  aThreshold = mThreshold;
}


float SignalLines::px()
{
  return mPhase[0]*mFreq1[0];
}

float SignalLines::py()
{
  return mPhase[1]*mFreq1[1];
}

float SignalLines::pz()
{
  return mPhase[2]*mFreq1[2];
}

D3DXVECTOR3 SignalLines::threshold()
{
  D3DXVECTOR3 t = mThreshold*0.5f*mFreq1;
  return t;
}

void SignalLines::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalLines);

  signalIOWrite( aStream );
  signalIOWrite( aStream, mFreq1, mPhase, mThreshold );
}

Signal * SignalLines::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalLines*  n = new SignalLines( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  signalIORead ( aStream, n->mFreq1, n->mPhase, n->mThreshold ) ;

  return n;
}


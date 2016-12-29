#include	"Editor_Texture/DataLayer/Signals/SignalSpheres.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalSpheres::SignalSpheres( int aId ) : Signal( aId, Signal::eIdSignalSpheres )
{
  mSignalType = Signal::eIdSignalSpheres;
  defaultParams();
  defaultParametersDerived();
}

SignalSpheres::SignalSpheres ( string aName ) :
  Signal( aName, eIdSignalSpheres )
{
  mSignalType = Signal::eIdSignalSpheres;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalSpheres::defaultParametersDerived()
{
  mFreq1 = D3DXVECTOR3(2,2,2);
  mPhase = D3DXVECTOR3(0,0,0);
  mThreshold = 0.1f;
}


void SignalSpheres::setNoiseParams(D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold)
{
  mFreq1 = aFreq1;
  mPhase = aPhase;
  mThreshold = aThreshold;
}

void SignalSpheres::getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold)
{
  aFreq1 = mFreq1;
  aPhase = mPhase;
  aThreshold = mThreshold;
}

void SignalSpheres::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalSpheres);

  signalIOWrite( aStream );
  signalIOWrite( aStream, mFreq1, mPhase, mThreshold );
}

Signal * SignalSpheres::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalSpheres*  n = new SignalSpheres( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  signalIORead ( aStream, n->mFreq1, n->mPhase, n->mThreshold ) ;

  return n;
}

D3DXVECTOR3 SignalSpheres::threshold()
{
  D3DXVECTOR3 t = mThreshold*0.5f*mFreq1;
  return t;
}

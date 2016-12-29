#include	"Editor_Texture/DataLayer/Signals/SignalTiledTriangles.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalTiledTriangles::SignalTiledTriangles( int aId ) :
  Signal( aId,
          Signal::eIdSignalTiledTriangles )
{
  mSignalType = Signal::eIdSignalTiledTriangles;
  defaultParams();
  defaultParametersDerived();
}

SignalTiledTriangles::SignalTiledTriangles ( string aName ) :
  Signal( aName, eIdSignalTiledTriangles )
{
  mSignalType = Signal::eIdSignalTiledTriangles;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalTiledTriangles::defaultParametersDerived()
{
  mFreq1 = D3DXVECTOR3(2,2,2);
  mPhase = D3DXVECTOR3(0,0,0);
  mThreshold = 0.1f;
}


void SignalTiledTriangles::setNoiseParams(D3DXVECTOR3  aFreq1, D3DXVECTOR3  aPhase, float  aThreshold)
{
  mFreq1 = aFreq1;
  mPhase = aPhase;
  mThreshold = aThreshold;
}

void SignalTiledTriangles::getNoiseParams(D3DXVECTOR3&  aFreq1, D3DXVECTOR3&  aPhase, float&  aThreshold)
{
  aFreq1 = mFreq1;
  aPhase = mPhase;
  aThreshold = mThreshold;
}

void SignalTiledTriangles::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalTiledTriangles);

  signalIOWrite( aStream );
  signalIOWrite( aStream, mFreq1, mPhase, mThreshold );
}

Signal * SignalTiledTriangles::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalTiledTriangles*  n = new SignalTiledTriangles( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  signalIORead ( aStream, n->mFreq1, n->mPhase, n->mThreshold ) ;

  return n;
}

D3DXVECTOR3 SignalTiledTriangles::threshold()
{
  D3DXVECTOR3 t = mThreshold*0.5f*mFreq1;
  return t;
}

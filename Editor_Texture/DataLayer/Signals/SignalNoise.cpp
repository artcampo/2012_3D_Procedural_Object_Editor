#include	"Editor_Texture/DataLayer/Signals/SignalNoise.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalNoise::SignalNoise( int aId )
  : Signal( aId,
            eIdSignalNoise )
{
  mSignalType = Signal::eIdSignalNoise;
  defaultParams();
  defaultParametersDerived();
}

SignalNoise::SignalNoise ( string aName ) :
  Signal( aName, eIdSignalNoise )
{
  mSignalType = Signal::eIdSignalNoise;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalNoise::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(2.6f,2.6f,2.6f);
  mNumOctaves = 1;
  mOctavesMul = 1;
}

float SignalNoise::turbulence()
{
  return mTurbulence;
}

void SignalNoise::setTurbulence(float aTurbulence)
{
  mTurbulence = aTurbulence;
}

void SignalNoise::save(QDataStream &aStream)
{
  aStream << int(eIdSignalNoise);

  signalIOWrite( aStream );

  aStream << mTurbulence;
}

Signal * SignalNoise::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalNoise*  n = new SignalNoise( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  //specific
  aStream >> n->mTurbulence;

  return n;
}

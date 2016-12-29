#include	"Editor_Texture/DataLayer/Signals/SignalMarble.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalMarble::SignalMarble( int aId )
  : Signal( aId,
            eIdSignalMarble)
{
  mSignalType = Signal::eIdSignalMarble;
  defaultParams();
  defaultParametersDerived();
}

SignalMarble::SignalMarble ( string aName ) :
  Signal( aName, eIdSignalMarble )
{
  mSignalType = Signal::eIdSignalMarble;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalMarble::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(1,1,1);
  mNumOctaves = 1;
  mOctavesMul = 1;
}

float SignalMarble::turbulence()
{
  return mTurbulence;
}

void SignalMarble::setTurbulence(float aTurbulence)
{
  mTurbulence = aTurbulence;
}

void SignalMarble::save(QDataStream &aStream)
{
  aStream << int(eIdSignalMarble);

  signalIOWrite( aStream );

  aStream << mTurbulence;
}

Signal * SignalMarble::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalMarble*  n = new SignalMarble( data.signalName);
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  //specific
  aStream >> n->mTurbulence;

  return n;
}

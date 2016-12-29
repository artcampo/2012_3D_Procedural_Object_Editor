#include	"Editor_Texture/DataLayer/Signals/SignalIdentity.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalIdentity::SignalIdentity( int aId )
  : Signal( aId,
            eIdSignalIdentity )
{
  mSignalType = Signal::eIdSignalIdentity;
  defaultParams();
  defaultParametersDerived();
}

SignalIdentity::SignalIdentity ( string aName ) :
  Signal( aName, eIdSignalIdentity )
{
  mSignalType = Signal::eIdSignalIdentity;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalIdentity::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(1,0,0);
  mNumOctaves = 1;
  mOctavesMul = 1;
}

float SignalIdentity::turbulence()
{
  return mTurbulence;
}

void SignalIdentity::setTurbulence(float aTurbulence)
{
  mTurbulence = aTurbulence;
}

void SignalIdentity::save(QDataStream &aStream)
{
  aStream << int(eIdSignalIdentity);

  signalIOWrite( aStream );

  aStream << mTurbulence;
}

Signal * SignalIdentity::loadL(QDataStream &aStream)
{
  SignalData data = signalIORead ( aStream ) ;
  SignalIdentity*  n = new SignalIdentity( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  //specific
  aStream >> n->mTurbulence;

  return n;
}

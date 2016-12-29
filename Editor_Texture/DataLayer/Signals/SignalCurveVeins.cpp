#include "Editor_Texture/DataLayer/Signals/SignalCurveVeins.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalCurveVeins::SignalCurveVeins( int aId )
  : Signal( aId,
            eIdSignalCurveVeins)
{
  mSignalType = Signal::eIdSignalCurveVeins;
  defaultParams();
  defaultParametersDerived();
}

SignalCurveVeins::SignalCurveVeins ( string aName) :
  Signal( aName, eIdSignalCurveVeins )
{
  mSignalType = Signal::eIdSignalCurveVeins;
  defaultParams();
  defaultParametersDerived();
}



//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalCurveVeins::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(1,1,1);
  mNumOctaves = 1;
  mOctavesMul = 1;
}

float SignalCurveVeins::turbulence()
{
  return mTurbulence;
}

void SignalCurveVeins::setTurbulence(float aTurbulence)
{
  mTurbulence = aTurbulence;
}

void SignalCurveVeins::save(QDataStream &aStream)
{
  aStream << int(Signal::eIdSignalCurveVeins);

  signalIOWrite( aStream );
}

Signal * SignalCurveVeins::loadL(QDataStream &aStream)
{  
  SignalData data = signalIORead ( aStream ) ;
  SignalCurveVeins*  n = new SignalCurveVeins( data.signalName );
  n->mW = data.mW;
  n->mNumOctaves = data.mNumOctaves;
  n->mOctavesMul = data.mOctavesMul;
  n->mInputParams = data.mInputParams;

  return n;
}

#include	"Editor_Texture/DataLayer/Signals/SignalNoiseSin.hpp"


//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================

SignalNoiseSin::SignalNoiseSin( int aId ) : Signal( aId, Signal::eIdSignalNoiseSin )
{
  mSignalType = Signal::eIdSignalNoiseSin;
  defaultParams();
  defaultParametersDerived();
}

SignalNoiseSin::SignalNoiseSin ( string aName ) :
  Signal( aName, eIdSignalNoiseSin )
{
  mSignalType = Signal::eIdSignalNoiseSin;
  defaultParams();
  defaultParametersDerived();
}

//===========================================================================
/*!
    Used to init with set of default parameters
*/
//===========================================================================
void SignalNoiseSin::defaultParametersDerived()
{
  mInputParams  = eIdPos_xyz;
  mW          = D3DXVECTOR3(1,1,1);
  mNumOctaves = 1;
  mOctavesMul = 1;
}

void SignalNoiseSin::save(QDataStream &aStream)
{
}

Signal * SignalNoiseSin::loadL(QDataStream &aStream)
{
  SignalNoiseSin*  n = new SignalNoiseSin(0);
  return n;
}

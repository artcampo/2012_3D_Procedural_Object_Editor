#include	"Editor_Texture/DataLayer/ProcShaderUnitTesting.hpp"
#include "Editor_Texture/DataLayer/Layer.hpp"

//===========================================================================
/*!
    Constructor:

    \param
*/
//===========================================================================



//===========================================================================
/*!
    Typical use:
    create a layer, two signals, and an operation between them
    Then revert this operation
*/
//===========================================================================

//  test basic insertion
void ProcShaderUnitTesting::init()
{

}

//  TEst del signal
void ProcShaderUnitTesting::init2()
{
  mShad = new ProcShader();

  string l0 = "Layer 0";
  string s0 = "Signal 0";
  string s1 = "Signal 1";
  string s2 = "Signal 2";

  mShad->createNewSignal();
  mShad->createNewSignal();
  mShad->createNewSignal();

  mShad->deleteSignal( s0 );
  
  delete mShad;
}

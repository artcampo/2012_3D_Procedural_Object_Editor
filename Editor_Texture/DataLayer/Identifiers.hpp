#ifndef _IDENTIFIERS_HPP_
#define _IDENTIFIERS_HPP_

#include "App/dx_misc.hpp"


#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include <sstream>




//===========================================================================
/*!
      Procedural Shader
      \brief      ..
*/
//===========================================================================
class Identifiers
{
public:

  Identifiers(){};

  void Init()
  {
    mNextLayerId = 0;
    mNextSignalId = 0;
  }

  int getLayerId ()
  {
    int ret = mNextLayerId;
    ++mNextLayerId;
    return ret;
  }

  int getSignalId ()
  {
    int ret = mNextSignalId;
    ++mNextSignalId;
    return ret;
  }

  void setSignalId ( int a )
  {
    mNextSignalId = a;
  }

  void setLayerId ( int a )
  {
    mNextLayerId = a;
  }

private:

  int  mNextLayerId;
  int  mNextSignalId;

};


#endif

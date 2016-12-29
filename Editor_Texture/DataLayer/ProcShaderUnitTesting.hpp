#ifndef _PROCSHADERUNITTESTING_HPP_
#define _PROCSHADERUNITTESTING_HPP_

#pragma once

#include "Editor_Texture/DataLayer/ProcShader.hpp"

#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include <sstream>
using namespace std;

class ProcShaderUnitTesting{

private:
  ProcShader*   mShad;

public:
  ProcShaderUnitTesting(){};

  void init();
  void init2();

};

#endif

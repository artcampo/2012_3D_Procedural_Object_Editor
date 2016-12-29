#include	"Editor_Texture/DataLayer/Signal.hpp"

bool Signal::writeParams( bool aIsBumpFunction, std::ofstream& aFile )
{

  switch ( mInputParams )
    {
    case eIdPos_xyz:
      if ( aIsBumpFunction )
        aFile << "objectScale*inputParameters";
      else
        aFile << "objectScale*input.posLS";
      aFile << ".xyz;";
      break;

    case eIdPos_xxx:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
      aFile << ".xxx;";
      break;

    case eIdPos_yzx:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
      aFile << ".yzx;";
      break;

    case eIdPos_zxy:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
      aFile << ".zxy;";
      break;


    case eIdSph_xyz:
      aFile << "objectScale*spherical.xyz;";
      break;

    case eIdUV_uvu:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
      aFile << ".zxy;";
      break;
    };

  return aFile.good();
}

bool Signal::writeParamsAtom( bool aIsBumpFunction, std::ofstream& aFile )
{
  switch ( mInputParams )
    {
    case eIdPos_xyz:
      if ( aIsBumpFunction )
        aFile << "objectScale*inputParameters";
      else
        aFile << "objectScale*input.posLS";
      aFile << ".xyz";
      break;

    case eIdPos_xxx:
      if ( aIsBumpFunction )
        aFile << "objectScale*inputParameters";
      else
        aFile << "objectScale*input.posLS";
      aFile << ".xxx";
      break;

  case eIdPos_yzx:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
    aFile << ".yzx";
    break;

  case eIdPos_zxy:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.posLS";
    aFile << ".zxy";
    break;

  case eIdUV_uvu:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.uv";
    aFile << ".xyx";
    break;

  case eIdNor_xyz:
    if ( aIsBumpFunction )
      aFile << "objectScale*inputParameters";
    else
      aFile << "objectScale*input.nor";
    aFile << ".xyz";
    break;


    case eIdSph_xyz:
      aFile << "objectScale*spherical.xyz";
      break;
    };

  return aFile.good();
}

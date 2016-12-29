#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"


bool SignalMarble::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << "\n  noiseIn = ";
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << "*"<< ProcShaderIO::formatDxVec3( mW ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  "<<ProcShaderIO::formatPerlinFunction( std::string("value"), std::string("noiseIn"), mNumOctaves, mOctavesMul );
  aFile << SignalMarbleData::signal1;

  aFile << "  noiseT = clamp( noiseT, 0, 1 );\n";
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  

  return aFile.good();
}

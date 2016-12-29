#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"


bool SignalNoise::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << SignalNoiseData::signal1;
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << SignalNoiseData::signal2;
  aFile << ProcShaderIO::formatDxVec3( mW ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  "<<ProcShaderIO::formatPerlinFunction( std::string("noiseT"), std::string("noiseIn"), mNumOctaves, mOctavesMul );
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  

  return aFile.good();
}

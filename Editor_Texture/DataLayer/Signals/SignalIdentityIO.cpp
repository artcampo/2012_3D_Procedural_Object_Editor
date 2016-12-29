#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"


bool SignalIdentity::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << SignalIdentityData::signal1;
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << SignalIdentityData::signal2;
  aFile << ProcShaderIO::formatDxVec3( mW ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  noiseT = fmod(noiseIn.x,1.0) + fmod(noiseIn.y,1.0) + fmod(noiseIn.z,1.0);\n";
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  

  return aFile.good();
}

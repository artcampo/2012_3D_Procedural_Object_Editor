#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"


bool SignalCurveVeins::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << SignalCurveVeinsData::signal1;
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << SignalCurveVeinsData::signal2;
  aFile << ProcShaderIO::formatDxVec3( mW ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  "<<ProcShaderIO::formatPerlinFunction( std::string("value"), std::string("noiseIn"), mNumOctaves, mOctavesMul );
  aFile << SignalCurveVeinsData::signal3x;
  aFile << SignalCurveVeinsData::signal3y;
  aFile << SignalCurveVeinsData::signal4;

  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";


  return aFile.good();
}

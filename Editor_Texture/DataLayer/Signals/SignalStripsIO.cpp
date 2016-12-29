#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"


bool SignalStrips::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << SignalStripsData::signal1;
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << SignalStripsData::signal2;
  aFile << ProcShaderIO::formatDxVec3( mW ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  noiseT = 0;\n";
  aFile << "  vectorValue = noiseIn * " << ProcShaderIO::formatDxVec3(mFreq1) << ";\n";

  float octave = 1.0f;
  for ( int i = 0 ; i < mNumOctaves; ++i)
  { 
    aFile << SignalStripsData::signal3_1;
    float currentPhase=  px() + py() + pz();
    aFile << ProcShaderIO::formatFloat( currentPhase );
    aFile << SignalStripsData::signal3_2;
    aFile << ProcShaderIO::formatFloat( octave );
    aFile << SignalStripsData::signal3_3;
    aFile << ProcShaderIO::formatFloat( mThreshold );
    aFile << SignalStripsData::signal3_4;
    aFile << ProcShaderIO::formatFloat( mThreshold );
    aFile << SignalStripsData::signal3_5;

    octave *= mOctavesMul;
  }
  aFile << "  noiseT = clamp( noiseT, 0, 1 );\n";
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  


  return aFile.good();
}

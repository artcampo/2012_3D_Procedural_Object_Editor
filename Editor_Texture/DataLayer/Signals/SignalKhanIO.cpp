#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"

bool SignalKhan1::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
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

  aFile << "  noiseT = 0;\n";

  float octave = 1.0f;
  for ( int i = 0 ; i < mNumOctaves; ++i)
  {
    aFile << SignalKhan1Data::signal5_1;
    aFile << mFreq1 << " * ";
    writeParamsAtom(aIsBumpFunction, aFile);
    aFile << SignalKhan1Data::signal5_2;
    aFile << mFreq2 << " * ";
    aFile << ProcShaderIO::formatFloat( octave );
    aFile << SignalKhan1Data::signal5_3;
    aFile << mThreshold;
    aFile << SignalKhan1Data::signal5_4;
    aFile << mThreshold;
    aFile << SignalKhan1Data::signal5_5;

    octave *= mOctavesMul;
  }

  aFile << "  noiseT = clamp( noiseT, 0, 1 );\n";
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";



  return aFile.good();
}

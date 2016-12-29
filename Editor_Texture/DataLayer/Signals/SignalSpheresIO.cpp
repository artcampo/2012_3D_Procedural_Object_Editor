#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"

bool SignalSpheres::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << SignalSpheresData::signal1;
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << SignalSpheresData::signal2;
  aFile << ProcShaderIO::formatDxVec3( mW );
  aFile << " + " << ProcShaderIO::formatDxVec3( mPhase ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << SignalSpheresData::signal3 << ProcShaderIO::formatDxVec3( mFreq1 ) << ";\n";

  aFile << SignalSpheresData::signal4_1 << ";\n";

  float octave = 1.0f;
  for ( int i = 0 ; i < mNumOctaves; ++i)
  {
    if (i != 0 )
    {
      aFile << SignalSpheresData::signal5_0_1;
      aFile << ProcShaderIO::formatFloat(mOctavesMul) << ";\n";
      aFile << SignalSpheresData::signal5_0_3;
      float deux = 2.0f;
      aFile <<ProcShaderIO::formatFloat(deux)<< ";\n";
    }

    {

      aFile << SignalSpheresData::signal5_1;
      aFile << ProcShaderIO::formatDxVec3( mFreq1 ) << ";\n";

      aFile << SignalSpheresData::signal5_2 ;
      aFile <<ProcShaderIO::formatFloat( mThreshold ) << ";\n";
    }
    octave *= mOctavesMul;
  }
  aFile << "\n  noiseT = saturate( noiseT );\n";
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  

  return aFile.good();
}

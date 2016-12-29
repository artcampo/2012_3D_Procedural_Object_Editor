#include  "Editor_Texture/DataLayer/Signal.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  "Editor_Texture/DataLayer/ProcShader.hpp"

bool SignalTiledTriangles::writeSignal ( bool aIsBumpFunction, std::ofstream& aFile, const Signal* aWarpSignal )
{
  aFile << "\n  noiseIn = ";
  writeParamsAtom(aIsBumpFunction, aFile);
  aFile << "*";
  aFile << ProcShaderIO::formatDxVec3( mW );
  aFile << " + " << ProcShaderIO::formatDxVec3( mPhase ) << ";\n";

  if ( hasWarp() )
  {
    aFile << "  noiseIn += float3(1,1,1)*";
    aFile << ProcShaderIO::formatFloat( warpLength() );
    aFile << "*" << ProcShaderIO::formatSignalName( aWarpSignal->getName() );
    aFile << ";\n" ;
  }

  aFile << "  vectorModulus = " << ProcShaderIO::formatDxVec3( mFreq1 ) << ";\n";

  D3DXVECTOR3 f1,f2,f3,f4;
  f1 = mFreq1; f1.y = 0;
  f2 = mFreq1; f2.y = 0; f2.x *= -1;
  f3 = mFreq1; f3.y = 0; f3.z = 0;
  f4 = mFreq1; f4.x = 0; f4.y = 0;
  float distCorner = f1.x*sqrt(2.0f);

  aFile <<     "  vectorValue = fmod( abs(noiseIn), vectorModulus );\n"
               "  vectorValue -= 0.5*vectorModulus;\n"
               "  vectorValue.y = 0;\n";
  aFile << "  noiseT = 0;\n";
  aFile << "  value2 = " << ProcShaderIO::formatFloat(f1.x)<< "*sqrt(2);\n";

  float octave = 1.0f;
  for ( int i = 0 ; i < mNumOctaves; ++i)
  {
    if (i != 0 )
    {
      aFile << "  vectorModulus *= ";
      aFile << ProcShaderIO::formatFloat(mOctavesMul) << ";\n";
      aFile << "  noiseIn *= ";
      float deux = 2.0f;
      aFile << ProcShaderIO::formatFloat(deux)<< "\n";
    }

    {

      aFile << "  value =  procLine( vectorValue, ";
      aFile << ProcShaderIO::formatDxVec3( f1 ) << ", value2);\n";

      aFile << "  value *=  procLine( vectorValue, ";
      aFile << ProcShaderIO::formatDxVec3( f2 ) << ", value2);\n";

      aFile << "  value *=  procLine( vectorValue, ";
      aFile << ProcShaderIO::formatDxVec3( f3 ) << ", value2);\n";

      aFile << "  value *=  procLine( vectorValue, ";
      aFile << ProcShaderIO::formatDxVec3( f4 ) << ", value2);\n";

      aFile << "  value = saturate(value);\n";
      aFile << "  value = 1 - value;\n";

      aFile << "  noiseT += value;\n";

    }
    octave *= mOctavesMul;
  }
  aFile << "  float "<< ProcShaderIO::formatSignalName( mName ) << " = noiseT;";
  

  return aFile.good();
}

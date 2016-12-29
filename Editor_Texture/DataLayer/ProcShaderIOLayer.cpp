#include  "Editor_Texture/DataLayer/ProcShader.hpp"
#include  "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include  <algorithm>


//===========================================================================
//
//===========================================================================
bool ProcShader::exportToHlsl ( LPCWSTR aHlslExportName )
{  
  return exportToHlsl( aHlslExportName, false, NULL );
}

bool ProcShader::exportToHlslDeferred ( LPCWSTR aHlslExportName, int aVertexLayout )
{
  bool ret;
  mExportToDeferred = true;
  mExportToDeferredVertexLayout = aVertexLayout;
  ret = exportToHlsl( aHlslExportName, false, NULL );
  mExportToDeferred = false;  
  return ret;
}


bool ProcShader::exportToHlsl ( LPCWSTR aHlslExportName, bool aDisplayLayer, string* aLayerSignalName )
{
  mDisplayLayer    = aDisplayLayer;
  mLayerSignalName = aLayerSignalName;

  mFile.open ( aHlslExportName );


  bool success = true;
  success &= writeHeader();
  success &= writeIncludes();
  success &= writeUniforms();
  success &= writeResources();
  success &= writeLayouts();
  success &= writeVertexShader();
  success &= writePixelShader();
  success &= writePass();

  mFile.close();
  success &= mFile.good();

  return success;
}

bool ProcShader::writePsSignalsVariableDeclarations ( bool aIsBumpFunction )
{ 
  mFile << ProcShadTextData::variables;

  if ( sphericalCoordinatesUsed() )
    mFile << ProcShadTextData::variablesSpherical;

  
  return mFile.good();
}

// TODO: remove?
bool ProcShader::writePsSignals ( bool aIsBumpFunction )
{ 
  std::vector<bool> signalWritten( mNumSignals );
  for ( int i = 0; i < mNumSignals ; ++i )
    signalWritten[i] = false;

  for ( int i = 0; i < mNumSignals ; ++i )
  {
    //TODO: can I call base func?
    if( !signalWritten[i] )
    {
      writePsSignalsRec( aIsBumpFunction, signalWritten, i );
    }
  }
  return mFile.good();
}

bool ProcShader::writePsSignalsRec ( bool aIsBumpFunction, std::vector<bool>& aSignalWritten, int aIndex )
{
  Signal* warpSignal = NULL;
  if ( mSignals[aIndex]->hasWarp() )
  {
    int indexWarpSignal = indexSignal(mSignals[aIndex]->warpSignal());
    if ( !aSignalWritten[indexWarpSignal] )
    {
      writePsSignalsRec( aIsBumpFunction, aSignalWritten, indexWarpSignal);
    }
    warpSignal = getSignal( mSignals[aIndex]->warpSignal() );
  }
  mSignals[aIndex]->writeSignal( aIsBumpFunction, mFile, warpSignal );
  mFile << "\n";
  aSignalWritten[aIndex] = true;

  return mFile.good();
}

bool ProcShader::writePsLayers ( Channel* aChannel  )
{
  return writePsLayerRec( aChannel, aChannel->getRootLayer() );
}

bool ProcShader::writePsLayerOperand ( Channel* aChannel, string aLayerId )
{
  Layer* layer = aChannel->getLayer( aLayerId );

  if ( layer->getLayerType() == Layer::eLayerSignal )
  {
    LayerSignal* l = dynamic_cast<LayerSignal*>( layer );
    mFile << ProcShaderIO::formatSignalName( l->getSignal() );
  }
  if ( layer->getLayerType() == Layer::eLayerOp )
  {
    mFile << ProcShaderIO::formatLayerName( layer->getName() );
  }

  return mFile.good();
}

bool ProcShader::writePsLayerRec   ( Channel* aChannel, string aLayerId )
{
  bool success = true;
  Layer* layer = aChannel->getLayer( aLayerId );

  if ( layer->getLayerType() == Layer::eLayerSignal )
  {

    mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
    LayerSignal* l = dynamic_cast<LayerSignal*>( layer );
    switch( l->getColMap().mColourMappingId )
    {
      case LayerSignal::eColMap_lerp2:
        mFile << " = " << ProcShaderIO::formatColourGradient(
                   (D3DXCOLOR) l->getColMap().mColorValue[0],
                   (D3DXCOLOR) l->getColMap().mColorValue[1],
                   ProcShaderIO::formatSignalName( l->getSignal() ));
        break;
      case LayerSignal::eColMap_spline4:
        mFile << ProcShaderIO::formatColourSplineN( l->getColMap().mColorValue,
                                                  ProcShaderIO::formatSignalName( l->getSignal() ),
                                                  4,
                                                  layer->getName() );
        break;
      case LayerSignal::eColMap_spline5:
      mFile << ProcShaderIO::formatColourSplineN( l->getColMap().mColorValue,
                                                ProcShaderIO::formatSignalName( l->getSignal() ),
                                                5,
                                                layer->getName()  );
        break;
      case LayerSignal::eColMap_spline6:
      mFile << ProcShaderIO::formatColourSplineN( l->getColMap().mColorValue,
                                                ProcShaderIO::formatSignalName( l->getSignal() ),
                                                6,
                                                layer->getName() );
        break;
    };


    mFile << "\n";

  }

  if ( layer->getLayerType() == Layer::eLayerOp )
  {
    //  Write all descendants that are non-signal
    LayerOp* l = dynamic_cast<LayerOp*>( layer );
    for ( int i = 0; i < l->getNumOperands() ; ++i )
    {
      std::string nameDesc = l->getIthOperand( i );
      // is the operand a layer?
      Layer* lDesc = aChannel->getLayer( nameDesc );
      //if ( lDesc->getLayerType() == eLayerOp )
      success &= writePsLayerRec( aChannel, lDesc->getName() );
    }

    //  Write operation
    int op = l->getOperation() ;
    switch( op )
    {
    case LayerOp::eOpHardLight:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() ) << ";\n";

      {
        //hardlight X
        mFile << "  if ( "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
        mFile<<".x < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".x = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
          mFile <<".x*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );mFile <<".x;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".x = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) ); mFile<<".x)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) ); mFile<<".x);\n";
          mFile << "  }\n";
        mFile << "\n";
      }

      {
        //hardlight Y
        mFile << "  if ( "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
        mFile<<".y < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".y = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
          mFile <<".y*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );mFile <<".y;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".y = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) ); mFile<<".y)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) ); mFile<<".y);\n";
          mFile << "  }\n";
        mFile << "\n";
      }

      {
        //hardlight Z
        mFile << "  if ( "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
        mFile<<".z < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".z = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
          mFile <<".z*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );mFile <<".z;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".z = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) ); mFile<<".z)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) ); mFile<<".z);\n";
          mFile << "  }\n";
        mFile << "\n";
      }
      break;

    case LayerOp::eOpOverlay:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() ) << ";\n";

      {
        //overlay X
        mFile << "  if ( "<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) );
        mFile<<".x < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".x = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
          mFile <<".x*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );mFile <<".x;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".x = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) ); mFile<<".x)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) ); mFile<<".x);\n";
          mFile << "  }\n";
        mFile << "\n";
      }

      {
        //overlay Y
        mFile << "  if ( "<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) );
        mFile<<".y < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".y = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
          mFile <<".y*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );mFile <<".y;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".y = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) ); mFile<<".y)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) ); mFile<<".y);\n";
          mFile << "  }\n";
        mFile << "\n";
      }

      {
        //overlay Z
        mFile << "  if ( "<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) );
        mFile<<".z < "<< l->blendFactor() << " )\n";
        mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".z = ";
          mFile << "2*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
          mFile <<".z*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );mFile <<".z;\n";
          mFile << "  }\n";
        mFile << "  else\n";
          mFile << "  {\n";
          mFile << "    "<< ProcShaderIO::formatLayerName( layer->getName() )<< ".z = 1- 2*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) ); mFile<<".z)*";
          mFile << "(1 - "<< ProcShaderIO::formatLayerName( l->getIthOperand( 1 ) ); mFile<<".z);\n";
          mFile << "  }\n";
        mFile << "\n";
      }
      break;

    case LayerOp::eOpAdd:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
      mFile << " = ";
      mFile << l->blendFactor() << "*"<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) );
      mFile << " + ";
      mFile << (1.0f - l->blendFactor()) << "*"<< ProcShaderIO::formatLayerName( l->getIthOperand( 1 ) );
      mFile << ";\n";
      break;

    case LayerOp::eOpMultiply:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
      mFile << " = ";
      mFile << l->blendFactor() << "*"<< ProcShaderIO::formatLayerName( l->getIthOperand( 0 ) );
      mFile << " * ";
      mFile << (1.0f - l->blendFactor()) << "*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
      mFile << ";\n";
      break;

    case LayerOp::eOpExclusion:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
      mFile << " = float4( 0.5, 0.5, 0.5, 1 ) - 2*(";
      mFile << l->blendFactor() << "*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
      mFile << " - float4( 0.5, 0.5, 0.5, 1 ))";
      mFile << " * (";
      mFile << (1.0f - l->blendFactor()) << "*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
      mFile << " - float4( 0.5, 0.5, 0.5, 1 ))";
      mFile << ";\n";
      break;

    case LayerOp::eOpScreen:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
      mFile << " = float4( 1, 1, 1, 1 ) - ( float4( 1, 1, 1, 1 ) - ";
      mFile << l->blendFactor() << "*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
      mFile << " )* ( float4( 1, 1, 1, 1 ) - ";
      mFile << (1.0f - l->blendFactor()) << "*"<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
      mFile << ");\n";
      break;


    case LayerOp::eOpMask:
      mFile << "  float4 " << ProcShaderIO::formatLayerName( layer->getName() );
      mFile << ";\n";
      mFile << "  if ( "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
      mFile << ".x < ";
      mFile << l->blendFactor() << " )\n";
        mFile << "  {\n";
        mFile << "    float tFade = "<< ProcShaderIO::formatLayerName(l->getIthOperand( 0 ) );
        mFile << " / ";
        mFile << l->blendFactor() <<";\n";
        mFile << "    tFade = (tFade - ";
        mFile << l->fadeFactor() << ") / (1 - " << l->fadeFactor() << ");\n";

        mFile << "    " << ProcShaderIO::formatLayerName( layer->getName() );
        mFile << " = lerp( float4(0,0,0,1), "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
        mFile << ", tFade);\n";
        mFile << "  }\n";
      mFile << "  else\n";
        mFile << "    " << ProcShaderIO::formatLayerName( layer->getName() );
        mFile << " = "<< ProcShaderIO::formatLayerName(l->getIthOperand( 1 ) );
        mFile << ";\n";
      break;
    };
    
  }

  success &= mFile.good();
  return success;
}

bool ProcShader::writeHeader()
{
  if (mExportToDeferred)
    mFile << ProcShadTextData::headerDef;
  else
    mFile << ProcShadTextData::header;
  return mFile.good();
}

bool ProcShader::writeIncludes()
{
  if (mExportToDeferred)
    mFile << ProcShadTextData::includesDef;
  else
    mFile << ProcShadTextData::includes;
  return mFile.good();
}

bool ProcShader::writeUniforms()
{
  if (mExportToDeferred)
    mFile << ProcShadTextData::uniformsDef;
  else
    mFile << ProcShadTextData::uniforms;
  return mFile.good();
}


bool ProcShader::writeResources()
{  
  return mFile.good();
}

bool ProcShader::writeLayouts()
{ 
  if (mExportToDeferred)
  {
    if (mExportToDeferredVertexLayout == 0)
      mFile << ProcShadTextData::layoutVsInDef1;
    else
      mFile << ProcShadTextData::layoutVsInDef2;
  }
  else
    mFile << ProcShadTextData::layoutVsIn;

  if (mExportToDeferred)
    if (mExportToDeferredVertexLayout == 0)
      mFile << ProcShadTextData::layoutVsOutDef1;
    else
      mFile << ProcShadTextData::layoutVsOutDef2;
  else
    mFile << ProcShadTextData::layoutVsOut;

  return mFile.good();
}

bool ProcShader::writeVertexShader ()
{ 
  if (mExportToDeferred)
    if ( mExportToDeferredVertexLayout == 0 )
      mFile << ProcShadTextData::vertexShaderDef1;
    else
      mFile << ProcShadTextData::vertexShaderDef2;
  else
    mFile << ProcShadTextData::vertexShader;

  return mFile.good();
}

bool ProcShader::writePixelShader ()
{ 
  bool success = true;

  mFile << ProcShadTextData::pixelShaderPreviousFuncLines;

  if (mExportToDeferred)
    mFile << ProcShadTextData::pixelShaderPart1DeF;
  else
    mFile << ProcShadTextData::pixelShaderPart1;


  success &= writePsSignalsVariableDeclarations( ProcShader::eIsGeneralShader );
  success &= writePsSignals( ProcShader::eIsGeneralShader );
  mFile << "\n";

  mFile << "\n  //RGBA CHANNEL\n";
  success &= writePsLayers( mRgbaChannel );
  success &= writePsLayers( mRgbaFarChannel );

 // if (mBumpEnabled && mDisplayLayer)
  {
    mFile << "\n  //BUMP CHANNEL\n";
    success &= writePsLayers( mBumpChannel );
  }

  mFile << "\n  //FINAL PIXEL\n";
  mFile << "  float  depthT = clamp( input.depth,"
        <<          ProcShaderIO::formatFloat( mRgbaChannel->depth()*2 )
        << ", " <<  ProcShaderIO::formatFloat( mRgbaFarChannel->depth() )
        << ")/" <<  ProcShaderIO::formatFloat( (mRgbaFarChannel->depth() - mRgbaChannel->depth()) )
        << ";\n";
  mFile << ProcShadTextData::pixelShaderPart2;

  //  Check if we display whole shader or single LayerSignal
  if ( !mDisplayLayer
       || (mPreviewChannelId != Channel::eChannel_rgba
           || mPreviewChannelId != Channel::eChannel_rgba_far ) )
  {
    Layer* layerRgbaNear = mRgbaChannel->getLayer( mRgbaChannel->getRootLayer() );
    Layer* layerRgbaFar = mRgbaFarChannel->getLayer( mRgbaFarChannel->getRootLayer() );
    mFile << ProcShaderIO::formatGradient(
               ProcShaderIO::formatLayerName( layerRgbaNear->getName() ),
               ProcShaderIO::formatLayerName( layerRgbaFar->getName() ),
               std::string("depthT")
               );

  }
  else
  {
    LayerSignal* l = dynamic_cast<LayerSignal*> (getLayer( *mLayerSignalName));
    std::string signalName = l->getSignal();
    mFile << ProcShaderIO::formatLayerName( signalName );
  }
  mFile << ";\n";
  mFile << ProcShadTextData::pixelShaderPart2a;


  //  Bump
  if ( mBumpEnabled )
  {
    mFile << ProcShadTextData::bumpFunction1;
    writePsSignals( ProcShader::eIsBumpFunction );

    mFile << "\n";
    writePsLayers( mBumpChannel );

    //  Check if we display whole shader or single LayerSignal
    mFile << ProcShadTextData::bumpFunction2;
    if ( !mDisplayLayer || (mPreviewChannelId != Channel::eChannel_bump ))
    {
      Layer* layer = mBumpChannel->getLayer( mBumpChannel->getRootLayer() );
      mFile << ProcShaderIO::formatLayerName( layer->getName() );
    }
    else
    {
      LayerSignal* l = dynamic_cast<LayerSignal*> (getLayer( *mLayerSignalName));
      std::string signalName = l->getSignal();
      mFile << ProcShaderIO::formatLayerName( signalName );
    }

    mFile << ProcShadTextData::bumpFunction5;
    if (mExportToDeferred)
      mFile << "  output.nor = float4(disturbedNormal,0);\n";
  }//end of bump writing
  else
  {
    if (mExportToDeferred)
      mFile << "  output.nor = input.nor;\n";
  }

  // If displaying a given layer do not use lighting
  if (mDisplayLayer)
  {
    mFile << "  finalPixel.rgb = " << ProcShaderIO::formatLayerName( *mLayerSignalName ) << ";\n";
  }
  else
  {
    if (!mExportToDeferred)
    {
      //  Lighting equation
      if ( !mBumpEnabled )
        mFile << ProcShadTextData::pixelShaderPart4a;
      else
        mFile << ProcShadTextData::pixelShaderPart4b;
    }
  }


  //  write out final pixel
  if (mExportToDeferred)
  {
    mFile << ProcShadTextData::pixelShaderPart5Def;
    D3DXCOLOR t = D3DXCOLOR(
                    mSpecChannel->mSpecColour.r,
                    mSpecChannel->mSpecColour.g,
                    mSpecChannel->mSpecColour.b,
                    mSpecChannel->mSpecPorcentage );

    mFile << ProcShaderIO::formatDxColor4( t );
    mFile << ";\n"
          << "  return output;\n"
          << "}\n\n";

  }
  else
    mFile << ProcShadTextData::pixelShaderPart5;
  
  return success;
}

bool ProcShader::writePass ()
{ 
  if (mExportToDeferred)
    mFile << ProcShadTextData::passDef;
  else
    mFile << ProcShadTextData::pass;
  return mFile.good();
}

std::string ProcShaderIO::formatInt       ( const int& aInt)
{
  stringstream strs ( stringstream::out);

  strs << aInt;

  return strs.str();
}

std::string ProcShaderIO::formatFloat(const  float aFloat )
{
 stringstream strs ( stringstream::out);

 strs.precision(5);
 strs.setf(ios::fixed,ios::floatfield); 
 
 strs << aFloat << "f";

 return strs.str();
}

std::string ProcShaderIO::formatDxColor4( const D3DXCOLOR& aColor )
{
 stringstream strs ( stringstream::out);

 strs.precision(5);
 strs.setf(ios::fixed,ios::floatfield); 
 strs << "float4("<< aColor.r <<", ";
 strs << aColor.g <<", ";
 strs << aColor.b <<", ";
 strs << aColor.a <<")";

 return strs.str();
}

std::string ProcShaderIO::formatDxColor3( const D3DXCOLOR& aColor )
{
 stringstream strs ( stringstream::out);

 strs.precision(5);
 strs.setf(ios::fixed,ios::floatfield);
 strs << "float3("<< aColor.r <<"f, ";
 strs << aColor.g <<", ";
 strs << aColor.b <<")";

 return strs.str();
}

std::string ProcShaderIO::formatDxVec3( const D3DXVECTOR3& aVec3 )
{
 stringstream strs ( stringstream::out);

 strs.precision(5);
 strs.setf(ios::fixed,ios::floatfield); 
 strs << "float3("<< aVec3.x <<"f, ";
 strs << aVec3.y <<"f, ";
 strs << aVec3.z <<"f)";

 return strs.str();
}

std::string ProcShaderIO::formatSignalName( std::string aString )
{
  std::string str = aString;
  remove_if(str.begin(), str.end(), isspace);
  remove_if(str.begin(), str.end(), ispunct);
  return str;
}

std::string ProcShaderIO::formatLayerName( std::string aString )
{
  std::string str = aString;
  remove_if(str.begin(), str.end(), isspace);
  remove_if(str.begin(), str.end(), ispunct);
  return str;
}
/*
bool  ProcShader::writeBumpFunction ()
{

  writePsSignalsVariableDeclarations( ProcShader::eIsBumpFunction);
  writePsSignals( ProcShader::eIsBumpFunction );

  mFile << "\n";
  writePsLayers( mBumpChannel );

  //  Check if we display whole shader or single LayerSignal
  mFile << ProcShadTextData::bumpFunction2;
  if ( !mDisplayLayer || (mPreviewChannelId != Channel::eChannel_bump ))
  {
    Layer* layer = mBumpChannel->getLayer( mBumpChannel->getRootLayer() );
    mFile << ProcShaderIO::formatLayerName( layer->getName() );
  }
  else
  {
    LayerSignal* l = dynamic_cast<LayerSignal*> (getLayer( *mLayerSignalName));
    std::string signalName = l->getSignal();
    mFile << ProcShaderIO::formatLayerName( signalName );
  }

  mFile << ProcShadTextData::bumpFunction3;
  return mFile.good();
}
*/


std::string ProcShaderIO::formatPerlinFunction( std::string& aOutput, std::string& aInput, int aNumOctaves, float aRoughness  )
{
  stringstream strs ( stringstream::out);

  if ( aNumOctaves == 1 || aNumOctaves == 0)
  {
    strs << aOutput << " = (Snoise3D( "<< aInput << ") + 1)*0.5;\n";
  }
  else
  {
    strs << ProcShadTextData::perlin1 << aInput;
    strs << ProcShadTextData::perlin2 << aNumOctaves;
    strs << ProcShadTextData::perlin3 << aOutput;
    strs << ProcShadTextData::perlin4 << aRoughness;
    strs << ProcShadTextData::perlin5;
  }

  return strs.str();
}

std::string  ProcShaderIO::formatColourGradient( D3DXCOLOR& aC1, D3DXCOLOR& aC2, std::string& aValueVariable )
{
  stringstream strs ( stringstream::out);

  strs << ProcShadTextData::col2Grad1 << formatDxColor4( aC1 );
  strs << ProcShadTextData::col2Grad2 << formatDxColor4( aC2 );
  strs << ProcShadTextData::col2Grad3 << aValueVariable;
  strs << ProcShadTextData::col2Grad4;

  return strs.str();
}

std::string  ProcShaderIO::formatGradient( std::string& aColorVariable1, std::string& aColorVariable2, std::string& aValueVariable )
{
  stringstream strs ( stringstream::out);

  strs << ProcShadTextData::col2Grad1 << aColorVariable1;
  strs << ProcShadTextData::col2Grad2 << aColorVariable2;
  strs << ProcShadTextData::col2Grad3 << aValueVariable;
  strs << ProcShadTextData::col2Grad4;

  return strs.str();
}

std::string  ProcShaderIO::formatColourSplineN( const std::vector<D3DXCOLOR>& aCol,
                                                const std::string& aValueVariable,
                                                const int aNum,
                                                const std::string& aLayerName)
{
  stringstream strs ( stringstream::out);
  stringstream nameSplineArraySS ( stringstream::out);

  nameSplineArraySS << formatLayerName(aLayerName) <<"_col";
  string nameSplineArray = nameSplineArraySS.str();

  //strs << "  float4 " << formatLayerName(aLayerName) << ";\n";
  strs << ";\n";
  strs << "  float3 " << nameSplineArray << "[" << formatInt(aNum) <<"]={\n";

  for ( int i = 0; i < aNum ; ++i )
  {
    strs << "    " << formatDxColor3( aCol[i] );
    if( i != (aNum - 1))  strs << ",\n";
    else                  strs << "};\n";
  }

  // body of spline
  strs << "  {\n";
  strs << "    float x = " << aValueVariable << ";\n";
  strs << "    int nSpans  = " << formatInt(aNum - 2) << ";\n";
  strs << "    float t     = clamp(x, 0, 1)*nSpans;\n";
  strs << "    int span    = int(t);\n";
  strs << "    t           -= float(span);\n";
  strs << "    float3 c3   = -0.5*" << nameSplineArray << "[span] + 1.5*" << nameSplineArray << "[span + 1] - 1.5*" << nameSplineArray << "[span + 2] + 0.5*" << nameSplineArray << "[span + 3];\n";
  strs << "    float3 c2   = " << nameSplineArray << "[span] - 2.5*" << nameSplineArray << "[span + 1] + 2*" << nameSplineArray << "[span + 2] - 0.5*" << nameSplineArray << "[span + 3];\n";
  strs << "    float3 c1   = -0.5*" << nameSplineArray << "[span] +  0.5*" << nameSplineArray << "[span + 2];\n";
  strs << "    float3 c0   = 1*" << nameSplineArray << "[span + 1];\n";
  strs << "    "<< formatLayerName(aLayerName) <<"  = float4(  ((c3*x + c2)*x + c1)*x + c0, 1);\n";
  strs << "  }\n";

  return strs.str();
}

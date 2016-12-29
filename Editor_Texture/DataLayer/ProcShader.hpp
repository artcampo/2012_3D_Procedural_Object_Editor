#ifndef _PROCSHADER_HPP_
#define _PROCSHADER_HPP_

//#pragma once

#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/Layer.hpp"
#include "Editor_Texture/DataLayer/Channel.hpp"
#include "Editor_Texture/DataLayer/Identifiers.hpp"
#include "Editor_Texture/DataLayer/ProcShaderMisc.hpp"

#include "Editor_Texture/DataLayer/Signal.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalNoise.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalNoiseSin.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalStrips.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalLines.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalKhan.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalMarble.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalCurveVeins.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalSpheres.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalTiledTriangles.hpp"
#include "Editor_Texture/DataLayer/Signals/SignalIdentity.hpp"


#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include <sstream>


//class ProcShader;
class ProcShaderIOBaseLayer;
class ProcShaderIOHlslLayer;

#include <QFile>

class WarpInfo{
public:
  bool        inUse;
  float       length;
  std::string warpSignalName;
};

//===========================================================================
/*!
      Procedural Shader
      \brief      ..
*/
//===========================================================================
class ProcShader
{
public:

  ProcShader ( );

  bool  exportToHlsl        ( LPCWSTR aHlslExportName );
  bool  exportToHlslDeferred( LPCWSTR aHlslExportName, int aVertexLayout );
  bool  previewLayerSignal  ( string aName, int aChannelId, LPCWSTR aHlslFileName);


  string  getRootLayer      ();

  void  setEditorVersion    ( string aEditorVersion );
  void  setName             ( string aName );
  void  selectChannel       ( int aChannelId );
  
  void  createNewSignal     ();
  void  createNewSignal     ( string aName );
  string createNewLayer     ( string aSignalName, std::string& aParent );

  void  deleteSignal        ( string aSignalName );
  void  modifySignal        ( string aName, int aNewType );
  void  modifySignal        ( string aName );
  void  modifySignal        ( string aName, int aNewType, int aInpParams,  D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul );
  void  modifySignalName    ( string aOldName, string aNewName );
  void  modifySignaltoDefault (string& aName );
  void  modifySignalWarp    (string& aName, string& aWarpSignalName, float aWarpLength, bool aWarpInUse );

  //  These go to their associated channel  
  void  modifiyLayer        ( string aName, string aSignal );
  void  modifiyLayer        ( string aName, int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 );
  void  modifiyLayer        ( string aName, int aOpId, float aBlendFactor );

  //  for the list structure
  void changePointedLayer ( std::string& aFather, std::string& aOldSon, std::string& aNewSon );

  //  General configuration
  void  setBumpCnfg ( bool aBumpEnabled );

  //  Channel handling
  std::string   channelName();
  float         channelDepth() const;
  void  setChannelName ( std::string& aName );
  void  setChannelDepth(const float aDepth );

  //  Signal and layer + load/save
  void  deleteAllLayers();
  void  deleteLayer     ( string& aLayerName );

  //  Communication with interface
  std::vector<std::string>  getLayersNames ();
  std::vector<std::string>  getSignalsNames ();
  Layer*        getLayer( std::string& aLayerName );
  Signal*       getSignal( std::string& aSignalName );

  int         getNumOperations ();
  std::string getNameOperation (int i);
  int         getNumInputParameters ();
  std::string getNameInputParameter (int i);
  int         getNumSignalTypes ();
  std::string getNameSignalType (int i);
  int         getNumChannelTypes ();
  std::string getNameChannelType (int i);
  int         getNumLayerColourMapping() const;
  std::string getNameLayerColourMapping(int i) const;

  bool  signalNameIsUnique( std::string& aSignalName );
  bool  signalExists( std::string& aSignalName );

  WarpInfo    getWarpInfo( std::string& aSignalName ) const;

public:
  //  Specific of specular channel
  void    setSpecPercentage( float aPerc );
  void    setSpecColour( float aSpecColour );

private:

  ChannelRgba*    mRgbaChannel;     /*!< Color channel*/
  ChannelRgba*    mRgbaFarChannel;
  ChannelBump*    mBumpChannel;
  ChannelSpec*    mSpecChannel;
  bool            mBumpEnabled;

  string      mName;            /*!< Shader name*/
  string      mEditorVersion;   /*!< Version of the editor*/

  Channel*    mSelectedChannelPointer;
  int         mSelectedChannel; /*!< selected channel, to operate with*/  
  Identifiers mNewIdentifiers;  /*!< holds new free identifiers*/

  std::vector<Signal*>  mSignals; /*!< holds all signals. Signals are used by all channels.*/ 
  int         mNumSignals;        /*!< number of different signals*/


  int     indexSignal   ( string aName ) const;
  void    signalDelete  ( int index );
  void    signalCreate  ( string aName );
  void    signalCreate  ( string aName, int aNewType, int aInpParams, D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul );

  //  related to shader compilation
  bool    sphericalCoordinatesUsed();

public:
  //    Save of a single channel
  void save( QDataStream& aStream );
  void load( QDataStream& aStream );

  //    Save of the whole shader
  void save ( QFile& aFile );
  ProcShader( QFile& aFile );


private:
  //  enum for boolean
  enum eIOPartofShader{
    eIsGeneralShader,
    eIsBumpFunction
  };
private:
  friend class ProcShaderIOBaseLayer;
  friend class ProcShaderIOHlslLayer;

  //  IO layer, temp variables
  bool              mExportToDeferred;
  int               mExportToDeferredVertexLayout;
  std::ofstream     mFile;
  bool              mDisplayLayer;
  int               mPreviewChannelId;
  std::string*      mLayerSignalName;

  bool  exportToHlsl ( LPCWSTR aHlslExportName,
                       bool aDisplayLayer,
                       string* aLayerSignalName );

  // Writing functions
  bool  writeHeader       ();
  bool  writeIncludes     ();
  bool  writeUniforms     ();
  bool  writeResources    ();
  bool  writeLayouts      ();
  bool  writeVertexShader ();
  bool  writePixelShader  ();
  bool  writePass         ();

  //bool  writeBumpFunction ();

  bool  writePsSignalsVariableDeclarations( bool aIsBumpFunction );
  bool  writePsSignals      ( bool aIsBumpFunction );
  bool  writePsSignalsRec   ( bool aIsBumpFunction, std::vector<bool>& aSignalWritten, int aIndex  );
  bool  writePsLayers       ( Channel* aChannel );
  bool  writePsLayerRec     ( Channel* aChannel, string aLayerId );
  bool  writePsLayerOperand ( Channel* aChannel, string aLayerId );


  //  used for profiling
  bool  writeDummyShader ();

  friend class SignalLayerWidget;

};

////////////////////////////////////////////////////////////////////////////////

namespace ProcShaderIO{
    //  Conversion to string
    std::string formatInt       ( const int& aInt);
    std::string formatFloat     ( const float aFloat );
    std::string formatDxColor4  ( const D3DXCOLOR& aColor );
    std::string formatDxColor3  ( const D3DXCOLOR& aColor );
    std::string formatDxVec3    ( const D3DXVECTOR3& aVec3 );
    std::string formatSignalName( std::string aString );
    std::string formatLayerName ( std::string aString );

    //  functions that write out specific chunks of code
    std::string formatPerlinFunction( std::string& aOutput, std::string& aInput, int aNumOctaves, float aRoughness  );
    std::string formatColourGradient( D3DXCOLOR& aC1, D3DXCOLOR& aC2, std::string& aValueVariable );
    std::string formatGradient      ( std::string& aColorVariable1, std::string& aColorVariable2, std::string& aValueVariable );
    std::string formatColourSplineN ( const std::vector<D3DXCOLOR>& aCol,
                                      const std::string& aValueVariable,
                                      const int aNum,
                                      const std::string& aLayerName );
};

#endif

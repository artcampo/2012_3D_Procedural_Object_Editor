#ifndef _LAYER_HPP_
#define _LAYER_HPP_

#pragma once

#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/ProcShaderTextData.hpp"
#include "Editor_Texture/DataLayer/Signal.hpp"
#include "Editor_Texture/DataLayer/Identifiers.hpp"
#include "Editor_Texture/DataLayer/ProcShaderMisc.hpp"

#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

// qt-io
#include <QDataStream>
#include <QTextStream>

class ColourMap;


//===========================================================================
/*!
      Procedural Shader
      \brief      ..
*/
//===========================================================================
class Layer
{
public:

  Layer ( int aId, std::string& aParent );
  Layer ( string aName, std::string& aParent );

  void          setName       ( string aName );
  std::string   parent();
  void          setParent( std::string& aParent );
  
  string        getName ();
  int           getLayerType();

  D3DXVECTOR4   virtual getFinalValue () = 0;
  ColourMap     virtual getColMap() const = 0;


protected:

  std::string   mName;              /*!< Layer name*/
  std::string   mParent;
  int           mLayerType;         /*!< value pertains to eLayerType enum*/

  friend class Channel;
  friend class ChannelRgba;

  //  IO
public:
  virtual bool    save( QDataStream& aStream, std::string& aSignal ) = 0;
  static Layer*  load( QDataStream& aStream );
  static Layer*  loadL( QDataStream& aStream );

  //  Definitions
public:
  static const std::string msOperationsNames[];
  static const int msNumOperations = 7;

  enum eLayerType{
    eLayerOp,
    eLayerSignal
  };

  ColourMap   static getDefaultColMap();
};

////////////////////////////////////////////////////////////////////////////////

class LayerOp : public Layer
{
public:


  LayerOp ( int aId, std::string& aParent );
  LayerOp  ( string aName, std::string& aParent );
  D3DXVECTOR4   getFinalValue();

  int     getOperation ();
  int     getNumOperands ();
  string  getIthOperand ( int index );
  float   blendFactor();
  float   fadeFactor();

  void    setOperation ( int aOpId, float aBlendFactor );
  void    setOperation ( int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 );
  void    setIthOperand ( int index, string& aLayer );

  ColourMap     virtual getColMap() const;

private:

  int           mOperation;           /*!< operation to perform */
  int           mNumOperands;         /*!< number of operands (signals or layers)*/
  std::vector<string>   mChildLayers; /*!< mNumOperands pointers to children layers*/
  float   mBlendFactor;       /*!< Blending factor between two operands */
  float   mFade;  /*!< Fade of mask operation */

  friend class Channel;
  friend class ChannelRgba;

  //  IO
public:
  bool    save( QDataStream& aStream, std::string& aSignal );
  static Layer*  loadL( QDataStream& aStream );

public:

  enum eLayerOperations{
    eOpOverlay,
    eOpAdd,
    eOpMultiply,
    eOpMask,
    eOpScreen,
    eOpHardLight,
    eOpExclusion
  };

};

////////////////////////////////////////////////////////////////////////////////

class ColourMap{

public:
  ColourMap(){};
  ColourMap( const int aColMapId, const std::vector<D3DXCOLOR>& aColorValue )
    : mColourMappingId(aColMapId),
      mColorValue(aColorValue) {};
  ColourMap( const int aColMapId, const D3DXCOLOR& aColorValue1, const D3DXCOLOR& aColorValue2 )
    : mColourMappingId(aColMapId)
  {
    mColorValue.resize(2);
    mColorValue[0] = aColorValue1;
    mColorValue[1] = aColorValue2;
  };

  int                     mColourMappingId;
  std::vector<D3DXCOLOR>  mColorValue;
};


////////////////////////////////////////////////////////////////////////////////

class LayerSignal : public Layer
{
public:

  LayerSignal ( int aId, std::string& aParent, const ColourMap& aColMap );
  LayerSignal ( string aName, std::string& aParent, const ColourMap& aColMap );

  D3DXVECTOR4   getFinalValue();

  void        setSignal ( string aSignalName );
  std::string getSignal ();

  void  setLayerColourMapping ( const int aId );
  int   getLayerColourMapping () const;
  //  2lerp colour mapping
  void        setColMap( const ColourMap& aColMap );
  ColourMap   getColMap() const;

private:

  string        mSignalName;        /*!< (mIsSignal=true) <=> pointer to the signal */
  ColourMap     mColMap;

  friend class Channel;
  friend class ChannelRgba;
  friend class ProcShader;
  //  IO
public:
  bool    save( QDataStream& aStream, std::string& aSignal );
  static Layer*  loadL( QDataStream& aStream );

  static const std::string msLayerColourMappingNames[];
  static const int msNumLayerColourMapping = 4;

  enum eColourMapping{
    eColMap_lerp2,
    eColMap_spline4,
    eColMap_spline5,
    eColMap_spline6
  };

};

////////////////////////////////////////////////////////////////////////////////

class LayerSignalIOData{
public:
  string      signal;
  string      parent;
  string      layerName;
  ColourMap   cmap;
};

#endif

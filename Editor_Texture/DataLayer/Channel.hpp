#ifndef _CHANNEL_HPP_
#define _CHANNEL_HPP_

#pragma once

#include "App/dx_misc.hpp"
#include "Editor_Texture/DataLayer/Layer.hpp"
#include "Editor_Texture/DataLayer/Identifiers.hpp"

#include <string>
#include <stdio.h>
#include <vector>
#include <list>

#include <iostream>
#include <fstream>
#include <cstring>

#include <sstream>
using namespace std;


class Channel;
class ChannelRgba;


//===========================================================================
/*!
      Procedural Shader
      \brief      ..
*/
//===========================================================================
class Channel
{
public:
  enum eChannels{
    eChannel_rgba,
    eChannel_rgba_far,
    eChannel_bump,
    eChannel_spec
  };
public:

  Channel ( Identifiers* aNewIdentifiers, const int aChannelId, const string& aChannelName  );


  string  createLayer       ( string aSignalName, std::string& aParent );  
  void  modifiyLayer        ( string aName, string aSignal);
  Layer*  getLayer          ( string aName );

  void  modifiyLayer        ( string aName, int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 );
  void  modifiyLayer        ( string aName, int aOpId, float aBlendFactor );

  string  getRootLayer      ();
  void    changePointedLayer ( std::string& aFather, std::string& aOldSon, std::string& aNewSon );
  std::vector<std::string>  getLayersNames();

  void          setChannelName( std::string& aChannelName );
  std::string   channelName();

  void    deleteAllLayers();
  void  deleteLayer     ( string& aLayerName );
  int numLayers();

  float depth() const;
  void setDepth( const float aDepth );

  void updateLayersAfterRenameOfSignal ( string& aOldName, string& aNewName );

  virtual void        setColMap( string aName, const ColourMap& aColMap ) = 0;
  virtual ColourMap   getColMap( string aName ) const = 0;

protected:


  std::string          mChannelName;
  int                  mChannelType;       /*!< Type of channel. */
  Identifiers*         mNewIdentifiers;

  std::vector<Layer*>  mLayers;     /*!< Array with the layers indexed via their id*/
  int                  mNumLayers;  /*!< Number of layers in previous array*/
  string               mRootLayer;  /*!< output layer: this is a m-ary tree */

  float                mDepth;      /*!< Distance perform frequency clamping */

  //LPCWSTR   mHlslExportName;    /*!< NAme of ouput file*/


  int       indexLayer ( string aName ) const;
  void      layerDeleteTemp ( int index );
  
  string    layerCreateEmpty( std::string& aParent );
  void      layerDelete( int index );

  void      deleteLayerRecursively( string aName );

  //  IO
public:
  std::list<std::string> save( QDataStream& aStream );
  void load( QDataStream& aStream );
public:
  static const std::string msChannelNames[];
  static const int msNumChannels = 4;
};



class ChannelRgba : public Channel
{
public:

  ChannelRgba ( Identifiers* aNewIdentifiers, const int aChannelId, const string& aChannelName   );

  void        setColMap( string aName, const ColourMap& aColMap )
  {
    int index = indexLayer( aName );
    int layerType = mLayers[index]->getLayerType();

    //Check if operands exists
    if ( layerType == Layer::eLayerSignal)
    {
      LayerSignal* l = dynamic_cast<LayerSignal*> (mLayers[index]);
      l->setColMap(aColMap);
    }
  }
  ColourMap   getColMap( string aName ) const
  {
    int index = indexLayer( aName );
    int layerType = mLayers[index]->getLayerType();

    //Check if operands exists
    if ( layerType == Layer::eLayerSignal)
    {
      LayerSignal* l = dynamic_cast<LayerSignal*> (mLayers[index]);
      return l->getColMap();
    }
    else
      return LayerSignal::getDefaultColMap();
  }

private:

};

class ChannelBump : public Channel
{
public:

  ChannelBump ( Identifiers* aNewIdentifiers, const int aChannelId, const string& aChannelName   );
  void  modifiyLayerColourMapping ( string aName, const int aId ){};
  int   getLayerColourMapping ( string aName ) const{ return 0;};
  void        setColMap( string aName, const ColourMap& aColMap ){};
  ColourMap   getColMap( string aName ) const { return LayerSignal::getDefaultColMap();};

private:

};

class ChannelSpec{
public:
  ChannelSpec( const int aChannelId, const string& aChannelName )
  {
    mSpecPorcentage = 1;
    mSpecColour     = D3DXCOLOR(1,1,1,1);
    mChannelType  = aChannelId;
    mChannelName  = aChannelName;
  }

  float       mSpecPorcentage;
  D3DXCOLOR   mSpecColour;
  std::string          mChannelName;
  int                  mChannelType;       /*!< Type of channel. */

  void  modifiyLayerColourMapping ( string aName, const int aId ){};
  int   getLayerColourMapping ( string aName ) const{ return 0;};

  void        setColMap( string aName, const ColourMap& aColMap ){};
  ColourMap   getColMap( string aName ) const { return LayerSignal::getDefaultColMap();};

};


#endif

#include	"Editor_Texture/DataLayer/Layer.hpp"

const std::string LayerSignal::msLayerColourMappingNames[]=
{
  "2 Lerp",
  "4 Spline",
  "5 Spline",
  "6 Spline"
};


LayerSignal::LayerSignal ( int aId, std::string& aParent, const ColourMap& aColMap )
  : Layer ( aId, aParent ),
    mColMap(aColMap)
{
  mLayerType = Layer::eLayerSignal;
}

LayerSignal::LayerSignal ( string aName, std::string& aParent, const ColourMap& aColMap )
  : Layer ( aName, aParent ),
    mColMap(aColMap)
{
  mLayerType = Layer::eLayerSignal;
}

std::string LayerSignal::getSignal ()
{
  return mSignalName;
}

//===========================================================================
/*!
    Gets the final value of the layer.

    \return    float4 with the value
*/
//===========================================================================
D3DXVECTOR4   LayerSignal::getFinalValue()
{
  D3DXVECTOR4 ret;

  return ret;
}


//===========================================================================
/*!
    Gets the final value of the layer.

    \return    float4 with the value
*/
//===========================================================================
void    LayerSignal::setSignal ( string aSignalName )
{
  mSignalName = aSignalName;
}

bool    LayerSignal::save( QDataStream& aStream, std::string& aSignal )
{
  aStream << int(eLayerSignal);
  aStream << QString::fromStdString( mName );
  aStream << QString::fromStdString( mParent );
  aStream << QString::fromStdString( mSignalName );

  aStream << mColMap.mColourMappingId;

  int numColors;
  switch( mColMap.mColourMappingId )
  {
    case LayerSignal::eColMap_lerp2:    numColors = 2; break;
    case LayerSignal::eColMap_spline4:  numColors = 4; break;
    case LayerSignal::eColMap_spline5:  numColors = 5; break;
    case LayerSignal::eColMap_spline6:  numColors = 6; break;
  };

  for ( int i = 0; i < numColors; ++i )
  {
    aStream << mColMap.mColorValue[i].r;
    aStream << mColMap.mColorValue[i].g;
    aStream << mColMap.mColorValue[i].b;
    aStream << mColMap.mColorValue[i].a;
  }

  aSignal = mSignalName;
  return true;
}

Layer*  LayerSignal::loadL( QDataStream& aStream )
{
  LayerSignalIOData d;
  QString s;

  aStream >> s;
  d.layerName = s.toStdString();
  aStream >> s;
  d.parent = s.toStdString();
  aStream >> s;
  d.signal = s.toStdString();


  aStream >> d.cmap.mColourMappingId;

  int numColors;
  switch( d.cmap.mColourMappingId )
  {
    case LayerSignal::eColMap_lerp2:    numColors = 2; break;
    case LayerSignal::eColMap_spline4:  numColors = 4; break;
    case LayerSignal::eColMap_spline5:  numColors = 5; break;
    case LayerSignal::eColMap_spline6:  numColors = 6; break;
  };

  for ( int i = 0; i < numColors; ++i )
  {
    aStream >> d.cmap.mColorValue[i].r;
    aStream >> d.cmap.mColorValue[i].g;
    aStream >> d.cmap.mColorValue[i].b;
    aStream >> d.cmap.mColorValue[i].a;
  }


  LayerSignal*  n = new LayerSignal( d.layerName, d.parent, d.cmap );

  n->mSignalName = d.signal;

  return n;
}


void  LayerSignal::setLayerColourMapping ( const int aId )
{
  mColMap.mColourMappingId = aId;
}

int   LayerSignal::getLayerColourMapping () const
{
  return mColMap.mColourMappingId;
}

void        LayerSignal::setColMap( const ColourMap& aColMap )
{
  mColMap = aColMap;
}

ColourMap   LayerSignal::getColMap() const
{
  return mColMap;
}

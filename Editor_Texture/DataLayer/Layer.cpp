#include	"Editor_Texture/DataLayer/Layer.hpp"

const std::string Layer::msOperationsNames[] =
{
  "Overlay",
  "Add",
  "Multiply",
  "Mask",
  "Screen",
  "HardLight",
  "Exclusion"
};


//===========================================================================
/*!
    Constructor, given a new id
    \param    aId    unique id
*/
//===========================================================================
Layer::Layer( int aId, std::string& aParent )
{
  mName = "Layer " + Misc::convertInt(aId);
  mParent = aParent;
}

//===========================================================================
/*!
    Constructor, given a unique name
    \param    aId    unique id
*/
//===========================================================================
Layer::Layer( string aName, std::string& aParent )
{
  mName = aName;
  mParent = aParent;
}

string  Layer::getName ()
{
  return mName;
}

int Layer::getLayerType()
{
  return mLayerType;
}



//===========================================================================
/*!
    Set name

    \param    aName   new name
*/
//===========================================================================

void    Layer::setName ( string aName )
{
  mName = aName;
}


std::string Layer::parent()
{
  return mParent;
}

void Layer::setParent(std::string &aParent)
{
  mParent = aParent;
}

float LayerOp::blendFactor()
{
  return mBlendFactor;
}

float LayerOp::fadeFactor()
{
  return mFade;
}

Layer* Layer::load( QDataStream& aStream )
{
  int id;
  aStream >> id;
  switch(id)
  {
  case eLayerSignal:
    return LayerSignal::loadL(aStream);
  case eLayerOp:
    return LayerOp::loadL(aStream);
  };

}

ColourMap Layer::getDefaultColMap()
{
  return ColourMap( LayerSignal::eColMap_lerp2, D3DXCOLOR(0,0,0,1), D3DXCOLOR(1,1,1,1) );
}

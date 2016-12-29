#include	"Editor_Texture/DataLayer/Layer.hpp"

LayerOp::LayerOp ( int aId, std::string& aParent ) : Layer ( aId, aParent )
{
  mLayerType = eLayerOp;
}

LayerOp::LayerOp ( string aName, std::string& aParent ) : Layer ( aName, aParent )
{
  mLayerType = eLayerOp;
}

int     LayerOp::getOperation ()
{
  return mOperation;
}

//===========================================================================
/*!
    Gets the number of descendant layers. They are the operands
    of the current layer's operation.

    \return    number of descendant layers
*/
//===========================================================================

int     LayerOp::getNumOperands ()
{
  return mNumOperands;
}

//===========================================================================
/*!
    Gets the ith operand of a layer

    \return   string    Operand's identifier
*/
//===========================================================================

string     LayerOp::getIthOperand ( int index )
{
  return mChildLayers[index];
}




//===========================================================================
/*!
    Gets the final value of the layer.

    \return    float4 with the value
*/
//===========================================================================

D3DXVECTOR4   LayerOp::getFinalValue()
{
  D3DXVECTOR4 ret;

  return ret;
}


//===========================================================================
/*!
    Sets the binary operation

    \param  aOpId         id of operation
    \param  aDescLayer1   operand #1
    \param  aDescLayer2   operand #2
*/
//===========================================================================


void  LayerOp::setOperation ( int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 )
{
  mOperation    = aOpId;
  mNumOperands  = 2;
  mChildLayers.resize ( mNumOperands );
  mChildLayers[0] = aDescLayer1;
  mChildLayers[1] = aDescLayer2;
  mBlendFactor    = aBlendFactor;
  mFade           = 0.8f;
}

void LayerOp::setOperation ( int aOpId, float aBlendFactor )
{
  mOperation    = aOpId;
  mBlendFactor    = aBlendFactor;
}

void LayerOp::setIthOperand(int index, std::string &aLayer)
{
  mChildLayers[index]=aLayer;
}


bool    LayerOp::save( QDataStream& aStream, std::string& aSignal )
{
  aStream << int(eLayerOp);
  aStream << QString::fromStdString( mName );
  aStream << QString::fromStdString( mParent );

  aStream << mOperation;
  aStream << mNumOperands;
  aStream << mBlendFactor;
  aStream << mFade;

  for ( unsigned int i = 0; i < mChildLayers.size() ; ++i)
    aStream << QString::fromStdString(mChildLayers[i]);

  return false;
}

Layer*  LayerOp::loadL( QDataStream& aStream )
{
  QString s;
  aStream >> s;
  std::string layerName = s.toStdString();
  aStream >> s;
  std::string parent = s.toStdString();

  LayerOp*  n = new LayerOp( layerName, parent );

  aStream >> n->mOperation;
  aStream >> n->mNumOperands;
  aStream >> n->mBlendFactor;
  aStream >> n->mFade;

  n->mChildLayers.resize( n->mNumOperands );
  for ( unsigned int i = 0; i < n->mChildLayers.size() ; ++i)
  {
    QString s;
    aStream >> s;
    n->mChildLayers[i] = s.toStdString();
  }

  return n;
}


ColourMap   LayerOp::getColMap() const
{
  return Layer::getDefaultColMap();
}

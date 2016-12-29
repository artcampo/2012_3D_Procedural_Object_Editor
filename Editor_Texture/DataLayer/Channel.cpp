#include	"Editor_Texture/DataLayer/Channel.hpp"

const std::string Channel::msChannelNames[]=
{
  "Diffuse - Near",
  "Diffuse - Far",
  "Bump",
  "Specular"
};

//===========================================================================
/*!
    Constructor:
*/
//===========================================================================

Channel::Channel ( Identifiers* aNewIdentifiers, const int aChannelId, const string& aChannelName )
{
  mNewIdentifiers = aNewIdentifiers;
  mChannelType  = aChannelId;
  mChannelName  = aChannelName;

  int layerId = mNewIdentifiers->getLayerId();
  mNumLayers  = 1;
  mLayers.resize(1);
  mLayers[0] = new LayerSignal( layerId, std::string("root"),
                                Layer::getDefaultColMap() );
  mRootLayer = mLayers[0]->getName();
}

//===========================================================================
/*!
    Returns root layer of this channel
    \return   id of root layer
*/
//===========================================================================
string  Channel::getRootLayer      ()
{
  string rootName;
  for ( unsigned int i = 0; i < mLayers.size() ; ++i)
  {
    if (mLayers[i]->parent() == string("root") )
    {
      rootName = mLayers[i]->getName();
    }
  }

  return rootName;
}

//===========================================================================
/*!
    Returns the index of layer identified by aName

    \param    aName  id of layer
    \return   index in array mLayers
*/
//===========================================================================

int      Channel::indexLayer ( string aName ) const
{
  int index  = 0;
  bool found = false;

  while (!found)
  {
    if ( mLayers[index]->getName() == aName  )
      found = true;
    else
      ++index;
  }
  
  return index;
}

Layer*  Channel::getLayer          ( string aName )
{
  int index = indexLayer( aName );
  return mLayers[index];
}


std::vector<std::string>  Channel::getLayersNames()
{
  std::vector<std::string> ret ( mLayers.size() );
  for ( unsigned int i = 0; i < mLayers.size() ; ++i)
  {
    ret[i] = mLayers[i]->getName();
  }

  return ret;
}

//===========================================================================
/*!
    Temporarily deleltes the layer, to allow mutation
    \param    index   index of layer in mLayers    
*/
//===========================================================================
void Channel::layerDeleteTemp ( int index )
{
  //  Delete old object
  if ( mLayers[index]->mLayerType == Layer::eLayerSignal )
  {
    LayerSignal* l = dynamic_cast<LayerSignal*>( mLayers[index] );
    delete l;
  }
  if ( mLayers[index]->mLayerType == Layer::eLayerOp )
  {
    LayerOp* l = dynamic_cast<LayerOp*>( mLayers[index] );
    delete l;
  }
}

//===========================================================================
/*!
    Deleltes the layer, thus reducing the layer count.
    \param    index   index of layer in mLayers    
*/
//===========================================================================
void Channel::layerDelete ( int index )
{
  //  Delete old object
  if ( mLayers[index]->mLayerType == Layer::eLayerSignal )
  {
    LayerSignal* l = dynamic_cast<LayerSignal*>( mLayers[index] );
    delete l;
  }
  if ( mLayers[index]->mLayerType == Layer::eLayerOp )
  {
    LayerOp* l = dynamic_cast<LayerOp*>( mLayers[index] );
    delete l;
  }

  //  Reorder vector and resize
  for ( int i = index + 1; i < mNumLayers ; ++i )
  {
      mLayers[i - 1] = mLayers[i];
  }
  --mNumLayers;
  mLayers.resize(mNumLayers);
}


//===========================================================================
/*!
    Verifies the existence of a layer
    \param    aName     id of layer
    \return   bool      true<=> it does exists
*/
//===========================================================================
/*
bool Channel::layerExists( string aName )
{

  bool found = false;
  int index  = 0;

  while ( !found && ( index < mNumLayers ))
  {
    if ( mLayers[index]->getName() == aName  )
      found = true;
    else
      ++index;
  }

  return found;
}
*/

//===========================================================================
/*!
    Creates an empty layer
    \return   string    layer's identifier
*/
//===========================================================================

string Channel::layerCreateEmpty( std::string& aParent )
{
  ++mNumLayers;
  mLayers.resize(mNumLayers);
  mLayers[mNumLayers - 1] = new LayerSignal( mNewIdentifiers->getLayerId(), aParent,
                                             Layer::getDefaultColMap() );
  return mLayers[mNumLayers - 1]->getName();
}

//===========================================================================
/*!
    Creates a layer assigning a signal to it.

    If it was a LayerOp its layers are deleted.

    \param    aSignalName   id of signal to be referenced
    \return   string        new layer's id
*/
//===========================================================================

string Channel::createLayer ( string aSignalName, std::string& aParent )
{
  ++mNumLayers;
  mLayers.resize(mNumLayers);
  mLayers[mNumLayers - 1] = new LayerSignal( mNewIdentifiers->getLayerId(), aParent,
                                             Layer::getDefaultColMap() );
  LayerSignal* l = dynamic_cast<LayerSignal*> (mLayers[mNumLayers - 1]);
  l->setSignal( aSignalName );
  return mLayers[mNumLayers - 1]->getName();
}

//===========================================================================
/*!
    Modifies a layer assigning a signal to it.

    If it was a LayerOp its layers are deleted.

    \param    aName     id of layer
    \param    aSignal   existant signal's name
*/
//===========================================================================

void  Channel::modifiyLayer        ( string aName, string aSignal )
{
  int index = indexLayer( aName );
  int layerType = mLayers[index]->getLayerType();

  ColourMap cmap = mLayers[index]->getColMap();

  //Check if operands exists
  if ( layerType == Layer::eLayerOp)
  {
    LayerOp* l = dynamic_cast<LayerOp*> (mLayers[index]);
    int numOperands = l->getNumOperands();
    for ( int i = 0; i < numOperands ; ++i)
    {
      deleteLayerRecursively( l->getIthOperand(i) );
    }
  }


  //  Retrieve info
  string name  = mLayers[index]->getName();
  std::string& parent = mLayers[index]->parent();

  //  Delete old object
  layerDeleteTemp( index );
  

  LayerSignal* l = new LayerSignal( name, parent, cmap );
  l->setSignal( aSignal );
  mLayers[index] = l;
  l->setColMap( cmap );

  //TODO
}



//===========================================================================
/*!
    Deletes a layer and its operands recursively.    

    \param  aName         id of layer
*/
//===========================================================================

void  Channel::deleteLayerRecursively( string aName )
{
  int index = indexLayer( aName );
  int layerType = mLayers[index]->getLayerType();

  //Check if operands exists
  if ( layerType == Layer::eLayerOp )
  {
    LayerOp* l = dynamic_cast<LayerOp*> (mLayers[index]);
    for ( int i = 0; i < l->getNumOperands() ; ++i)
    {
      deleteLayerRecursively( l->getIthOperand(i) );
    }
  }

  layerDelete( index );
}

//===========================================================================
/*!
    Modifies a layer assigning an operation to it.
    
    All layer modifying functions, delete the operands when the new number of
    operands is different.

    

    \param  aName         id of layer
    \param  aOpId         id of operation
    \param  aDescLayer1   operand #1
    \param  aDescLayer2   operand #2
*/
//===========================================================================

void  Channel::modifiyLayer        ( string aName, int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 )
{
  int index = indexLayer( aName );


  //  Retrieve info
  string name  = mLayers[index]->getName();
  std::string& parent = mLayers[index]->parent();

  //  Delete old object
  layerDeleteTemp( index );

  LayerOp* l = new LayerOp( name, parent );
  l->setOperation ( aOpId, aBlendFactor, aDescLayer1, aDescLayer2 );
  mLayers[index] = l;

}

//===========================================================================
/*!
    Modifies a layer assigning an operation to it, but leaving the
    operands empty.

    If it was a LayerSignal two new empty layers are created.

    PRE: Layer is of eLayerSignal.

    \param  aName         id of layer
    \param  aOpId         id of operation
*/
//===========================================================================

void  Channel::modifiyLayer        ( string aName, int aOpId, float aBlendFactor )
{
  int index     = indexLayer( aName );
  int layerType = mLayers[index]->getLayerType();

  if ( layerType == Layer::eLayerSignal )
  {
    //  Create empty layers
    string emptyLayer1;
    string emptyLayer2;
    if ( layerType == Layer::eLayerSignal)
    {
      emptyLayer1 = layerCreateEmpty(aName);
      emptyLayer2 = layerCreateEmpty(aName);
    }

    //  Retrieve info
    string name  = mLayers[index]->getName();
    std::string& parent = mLayers[index]->parent();

    //  Delete old object
    layerDeleteTemp( index );

    LayerOp* l = new LayerOp( name, parent );
    l->setOperation ( aOpId, aBlendFactor, emptyLayer1, emptyLayer2 );
    mLayers[index] = l;
  }
  else
  {
    LayerOp* lo = dynamic_cast<LayerOp*> (mLayers[index]);
    lo->setOperation( aOpId, aBlendFactor );
  }

}

/*
  Changes the son of "aFather"
*/
void Channel::changePointedLayer(std::string &aFather, std::string &aOldSon, std::string &aNewSon)
{
  if ( aFather == string("root"))
  {
    mRootLayer = aNewSon;
  }
  else
  {
    int index     = indexLayer( aFather );
    LayerOp* lo = dynamic_cast<LayerOp*> (mLayers[index]);
    for (int i = 0; i < lo->getNumOperands() ; ++i)
    {
      string son = lo->getIthOperand(i);
      if (son == aOldSon)
        lo->setIthOperand( i, aNewSon );
    }
  }
}

std::string Channel::channelName()
{
  return mChannelName;
}

void Channel::setChannelName(std::string &aChannelName)
{
  mChannelName = aChannelName;
}

void Channel::deleteAllLayers()
{
  mLayers.resize(0);
  mNumLayers = 0;
}

//  /return   list of signals that form the channel
std::list<std::string> Channel::save(QDataStream &aStream)
{
  aStream << int( mNumLayers );
  aStream << QString::fromStdString( mChannelName );
  aStream << QString::fromStdString( mRootLayer );
  aStream << float( mDepth );

  std::list<std::string> signalsToWrite;
  for ( unsigned int i = 0; i < mLayers.size() ; ++i)
  {
    std::string currentSignal;
    bool writeSignal = mLayers[i]->save(aStream, currentSignal );
    if ( writeSignal )
      signalsToWrite.push_back( currentSignal );
  }

  signalsToWrite.unique();

  return signalsToWrite;
}

void Channel::load(QDataStream &aStream)
{  
  aStream >> mNumLayers;

  QString s;
  aStream >> s;
  mChannelName = s.toStdString();
  aStream >> s;
  mRootLayer = s.toStdString();
  aStream >> mDepth;

  mLayers.resize(mNumLayers);
  for ( unsigned int i = 0; i < mLayers.size() ; ++i)
  {
    mLayers[i] = Layer::load(aStream);
  }

}

int Channel::numLayers()
{
  return mNumLayers;
}

void Channel::updateLayersAfterRenameOfSignal(std::string &aOldName, std::string &aNewName)
{
  for ( unsigned int i = 0; i < mLayers.size() ; ++i)
  {
    if ( mLayers[i]->getLayerType() == Layer::eLayerSignal )
    {
      LayerSignal* l = dynamic_cast<LayerSignal*> (mLayers[i]);
      if ( l->getSignal() == aOldName )
        l->setSignal(aNewName);
    }
  }
}

void Channel::deleteLayer(std::string &aLayerName)
{
  int index = indexLayer(aLayerName);
  layerDelete(index);
}


float Channel::depth() const
{
  return mDepth;
}

void  Channel::setDepth( const float aDepth )
{
  mDepth = aDepth;
}

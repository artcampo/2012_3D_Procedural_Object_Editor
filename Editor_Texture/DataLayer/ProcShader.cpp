#include	"Editor_Texture/DataLayer/ProcShader.hpp"


//===========================================================================
/*!
    Constructor.
    Creates an empty root layer for each channel.

    \param
*/
//===========================================================================
ProcShader::ProcShader (  )
{
  mNewIdentifiers.Init();

  mRgbaChannel    = new ChannelRgba( &mNewIdentifiers,  Channel::eChannel_rgba,     getNameChannelType(Channel::eChannel_rgba) );
  mRgbaFarChannel = new ChannelRgba( &mNewIdentifiers,  Channel::eChannel_rgba_far, getNameChannelType(Channel::eChannel_rgba_far) );
  mBumpChannel    = new ChannelBump( &mNewIdentifiers,  Channel::eChannel_bump,     getNameChannelType(Channel::eChannel_bump) );
  mSpecChannel    = new ChannelSpec(                    Channel::eChannel_spec,     getNameChannelType(Channel::eChannel_spec) );

  mNumSignals             = 0;
  mSelectedChannel        = Channel::eChannel_rgba;
  mSelectedChannelPointer = mRgbaChannel;

  mBumpEnabled  = false;
  mExportToDeferred = false;

  mRgbaChannel->setDepth(10);
  mRgbaFarChannel->setDepth(100);
  mBumpChannel->setDepth(10);
}

//===========================================================================
/*!
    Returns root layer of selected channel
    \return   id of root layer
*/
//===========================================================================
string  ProcShader::getRootLayer      ()
{
  return mSelectedChannelPointer->getRootLayer();
}

void  ProcShader::setEditorVersion ( string aEditorVersion )
{
  mEditorVersion = aEditorVersion;
}

void  ProcShader::setName ( string aName )
{
  mName = aName;
}

void  ProcShader::selectChannel       ( int aChannelId )
{
  mSelectedChannel = aChannelId;

  if (mSelectedChannel == Channel::eChannel_rgba )
    mSelectedChannelPointer = mRgbaChannel;

  if (mSelectedChannel == Channel::eChannel_rgba_far )
    mSelectedChannelPointer = mRgbaFarChannel;

  if (mSelectedChannel == Channel::eChannel_bump )
    mSelectedChannelPointer = mBumpChannel;


  // TODO: modify me
  if ( !( mSelectedChannel == Channel::eChannel_rgba ||
          mSelectedChannel == Channel::eChannel_rgba_far ||
          mSelectedChannel == Channel::eChannel_bump  ))
  {
    mSelectedChannel = Channel::eChannel_rgba;
    mSelectedChannelPointer = mRgbaChannel;
  }
}

void  ProcShader::createNewSignal     ()
{
  mSignals.resize( mNumSignals + 1);
  mSignals[mNumSignals] = new SignalNoise ( mNewIdentifiers.getSignalId() );

  ++mNumSignals;
}

void  ProcShader::createNewSignal     ( string aName )
{
  mSignals.resize( mNumSignals + 1);
  mSignals[mNumSignals] = new SignalNoise ( mNewIdentifiers.getSignalId() );
  std::string oldName = mSignals[mNumSignals]->getName();
  modifySignalName( oldName, aName);

  ++mNumSignals;
}

void  ProcShader::deleteSignal     ( string aSignalName )
{
  int index = indexSignal ( aSignalName );
  signalDelete( index );
}

//===========================================================================
/*!
    Returns the index of signal identified by aName

    \param    aName   id of signal
    \return   index   in array mSignals
*/
//===========================================================================

int      ProcShader::indexSignal ( string aName ) const
{
  int index  = 0;
  bool found = false;

  while (!found)
  {
    if ( mSignals[index]->getName() == aName  )
      found = true;
    else
      ++index;
  }
  
  return index;
}


//===========================================================================
/*!
    Deletes a signal array entry, just to be reassigned.
    \param    index   in array mSignals
*/
//===========================================================================
void ProcShader::signalDelete ( int index )
{
  //  Delete old object  
  delete mSignals[index];

  //  Rearrange mSignals
  for ( unsigned int i = index; i < (mSignals.size() - 1) ; ++i)
  {
    mSignals[i] = mSignals[i + 1];
  }

  --mNumSignals;
  mSignals.resize( mNumSignals );

}


//===========================================================================
/*!
  
    Deletes a signal array entry, just to be reassigned.
    \param    aName   name of new signal
*/
//===========================================================================
void ProcShader::signalCreate ( string aName )
{
  mSignals.resize( mNumSignals + 1);
  mSignals[mNumSignals] = new SignalNoise ( mNewIdentifiers.getSignalId() );
  ++mNumSignals;

}

// OFFENDING
void ProcShader::signalCreate ( string aName, int aNewType, int aInpParams, D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul )
{
  mSignals.resize( mNumSignals + 1);

  if ( aNewType == Signal::eIdSignalNoise )
  {
    SignalNoise* l = new SignalNoise( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalNoiseSin )
  {
    SignalNoiseSin* l = new SignalNoiseSin( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }

  if ( aNewType == Signal::eIdSignalStrips )
  {
    SignalStrips* l = new SignalStrips( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalLines )
  {
    SignalLines* l = new SignalLines( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalKhan1 )
  {
    SignalKhan1* l = new SignalKhan1( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalMarble )
  {
    SignalMarble* l = new SignalMarble( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == SignalCurveVeins::eIdSignalCurveVeins )
  {
    SignalCurveVeins* l = new SignalCurveVeins( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalSpheres )
  {
    SignalSpheres* l = new SignalSpheres( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }
  if ( aNewType == Signal::eIdSignalTiledTriangles )
  {
    SignalTiledTriangles* l = new SignalTiledTriangles( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }  
  if ( aNewType == Signal::eIdSignalIdentity )
  {
    SignalIdentity* l = new SignalIdentity( aName );
    mSignals[mNumSignals] = dynamic_cast<Signal*> (l);
  }

  mSignals[mNumSignals]->setParams (aInpParams, aW, aNumOctaves, aOctavesMul );

  ++mNumSignals;

}

//===========================================================================
/*!
    Write signal to an hlsl file so we can preview it

    \param    aName         LayerSignal name
    \param    aHlslFileName output file name

*/
//===========================================================================
bool  ProcShader::previewLayerSignal       ( string aName, int aChannelId, LPCWSTR aHlslFileName )
{
  mPreviewChannelId = aChannelId;
  Layer* l = mSelectedChannelPointer->getLayer( aName );
  string layerSignalName = l->getName();
  return exportToHlsl( aHlslFileName, true, &layerSignalName );
}

//===========================================================================
/*!
    Changes the name of a signal

    \param    aOldName    Old signal's name
    \param    aNewName    new signal's name (must be unique)
*/
//===========================================================================
void ProcShader::modifySignalName( string aOldName, string aNewName )
{
  int index = indexSignal ( aOldName );
  mSignals[index]->setName( aNewName );

  //TODO: add channel
  mRgbaChannel->updateLayersAfterRenameOfSignal ( aOldName, aNewName );
  mRgbaFarChannel->updateLayersAfterRenameOfSignal ( aOldName, aNewName );
  mBumpChannel->updateLayersAfterRenameOfSignal ( aOldName, aNewName );
}


//===========================================================================
/*!
    Change type of signal

    \param    aName         Signal's name
    \param    aNewType      new tpe
*/
//===========================================================================
void  ProcShader::modifySignal        ( string aName, int aNewType )
{
  int index = indexSignal ( aName );

  string      name     = mSignals[index]->getName();

  D3DXVECTOR3 w;
  int inpParams;
  int numOctaves;
  float octavesMul;
  mSignals[index]->getParams( inpParams, w, numOctaves, octavesMul);
  WarpInfo warp = getWarpInfo(aName);

  signalDelete(index);
  signalCreate( aName, aNewType, inpParams, w, numOctaves, octavesMul );
  mSignals[index]->setWarp( warp.warpSignalName, warp.length, warp.inUse);
}

//===========================================================================
/*!
    Set parameters of noise function

    \param    aName         Signal's name
    \param    aW            ponderation to each input parameter
    \param    aNumOctaves   number of octaves
    \param    aOctavesMul   multiplier between octaves
*/
//===========================================================================
void ProcShader::modifySignal ( string aName, int aNewType, int aInpParams,  D3DXVECTOR3 aW, int aNumOctaves, float aOctavesMul )
{
  int index = indexSignal ( aName );

  string      name    = mSignals[index]->getName();  

  WarpInfo warp = getWarpInfo(aName);
  signalDelete(index);

  signalCreate( aName, aNewType, aInpParams, aW, aNumOctaves, aOctavesMul );
  mSignals[index]->setWarp( warp.warpSignalName, warp.length, warp.inUse);


}

//===========================================================================
/*!
    Modify a layer assigning a signal to it

    \param
*/
//===========================================================================
void  ProcShader::modifiyLayer     ( string aName, string aSignal )
{
  mSelectedChannelPointer->modifiyLayer( aName, aSignal );
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
void  ProcShader::modifiyLayer        ( string aName, int aOpId, float aBlendFactor, string aDescLayer1, string aDescLayer2 )
{
  mSelectedChannelPointer->modifiyLayer( aName, aOpId, aBlendFactor, aDescLayer1, aDescLayer2 );
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
void  ProcShader::modifiyLayer        ( string aName, int aOpId, float aBlendFactor )
{
  mSelectedChannelPointer->modifiyLayer( aName, aOpId, aBlendFactor );
}

//===========================================================================
/*!
    Creates  layer assigning a signal to it.

    If it was a LayerOp its layers are deleted.

    \param    aSignalName   id of signal to be referenced
    \return   string        new layer's id
*/
//===========================================================================
string ProcShader::createNewLayer ( string aSignalName, std::string& aParent )
{
  string newLayer;

  newLayer = mSelectedChannelPointer->createLayer( aSignalName, aParent );

  return newLayer;
}

std::vector<std::string> ProcShader::getLayersNames()
{
  return mSelectedChannelPointer->getLayersNames();
}

Layer* ProcShader::getLayer(std::string &aLayerName)
{
  return mSelectedChannelPointer->getLayer( aLayerName );
}

Signal* ProcShader::getSignal(std::string &aSignalName)
{

  int index = indexSignal ( aSignalName );
  return mSignals[index];
}

std::vector<std::string> ProcShader::getSignalsNames()
{
  std::vector<std::string> ret (  mSignals.size() );

  for (unsigned int i = 0; i < mSignals.size() ; ++i)
    ret[i] = mSignals[i]->getName();

  return ret;
}

int ProcShader::getNumOperations()
{
  return Layer::msNumOperations;
}

std::string ProcShader::getNameOperation(int i)
{
  return Layer::msOperationsNames[i];
}

bool ProcShader::signalNameIsUnique(std::string &aSignalName)
{
  bool isUnique = true;

  for (unsigned int i = 0; i < mSignals.size() ; ++i)
    if (mSignals[i]->getName() == aSignalName )
      isUnique = false;
  return isUnique;
}

int ProcShader::getNumInputParameters()
{
  return Signal::msNumInputParameters;
}

std::string ProcShader::getNameInputParameter(int i)
{
  return Signal::msInputParametersNames[i];
}

void ProcShader::changePointedLayer ( std::string& aFather, std::string& aOldSon, std::string& aNewSon )
{
  mSelectedChannelPointer->changePointedLayer(aFather,aOldSon,aNewSon);
}

int ProcShader::getNumSignalTypes()
{
  return Signal::msNumSignalType;
}

std::string ProcShader::getNameSignalType(int i)
{
  return Signal::msSignalTypesNames[i];
}

void ProcShader::setBumpCnfg(bool aBumpEnabled)
{
  mBumpEnabled = aBumpEnabled;
}

bool ProcShader::sphericalCoordinatesUsed()
{
  //TODO
  return true;
}

int ProcShader::getNumChannelTypes()
{
  return Channel::msNumChannels;
}

std::string ProcShader::getNameChannelType(int i)
{
  return Channel::msChannelNames[i];
}

//TODO
std::string ProcShader::channelName()
{
  return mSelectedChannelPointer->channelName();
}

float ProcShader::channelDepth() const
{
 return mSelectedChannelPointer->depth();
}

void ProcShader::setChannelName(std::string &aName)
{
  mSelectedChannelPointer->setChannelName( aName );
}

void ProcShader::setChannelDepth(const float aDepth )
{
  mSelectedChannelPointer->setDepth( aDepth );
}


void ProcShader::deleteAllLayers()
{
  mSelectedChannelPointer->deleteAllLayers();
}

void ProcShader::save(QDataStream &aStream)
{
  std::list<std::string> signalsToWrite = mSelectedChannelPointer->save( aStream );

  aStream << int( signalsToWrite.size() );

  for (std::list<std::string>::iterator it=signalsToWrite.begin(); it!=signalsToWrite.end(); ++it)
  {
    int index = indexSignal( *it );
    mSignals[index]->save( aStream );
  }
}

void ProcShader::load(QDataStream &aStream)
{
  mSelectedChannelPointer->load( aStream );

  mNewIdentifiers.setLayerId( mRgbaChannel->numLayers()
                              + mRgbaFarChannel->numLayers()
                              + mBumpChannel->numLayers() );

  int newSignals;
  aStream >> newSignals;

  for ( int i = 0; i < newSignals ; ++i)
  {
    Signal* l = Signal::load( aStream );

    std::string newName = l->prependChannelName( mSelectedChannelPointer->channelName() );

    //  if a signal with the same name exists, "overwrite" it
    if ( signalExists( l->getName() ) )
    {
      deleteSignal( l->getName() );
    }

    mSignals.resize ( mNumSignals + 1 );
    mSignals[ mNumSignals ] = l;
    ++mNumSignals;

    //  Prepend the channel name to the signal
    modifySignalName ( l->getName(), newName );
  }

  mNewIdentifiers.setSignalId( mNumSignals + 2 );
}

bool ProcShader::signalExists(std::string &aSignalName)
{
  bool exists = false;

  for (unsigned int i = 0; i < mSignals.size() ; ++i)
    if (mSignals[i]->getName() == aSignalName )
      exists = true;

  return exists;
}

void ProcShader::modifySignaltoDefault(std::string &aName)
{
  int index = indexSignal ( aName );
  mSignals[index]->setDefaultValues();
}

void ProcShader::deleteLayer(std::string &aLayerName)
{
  mSelectedChannelPointer->deleteLayer( aLayerName );
}

void ProcShader::setSpecPercentage(float aPerc)
{
  mSpecChannel->mSpecPorcentage = aPerc;
}

void ProcShader::setSpecColour(float aSpecColour)
{
  mSpecChannel->mSpecColour = aSpecColour;
}

void ProcShader::save( QFile& aFile )
{
    QDataStream stream(&aFile);

    QString s;
    s = QString::fromStdString(mName);              stream << s;
    s = QString::fromStdString(mEditorVersion);     stream << s;
    stream << mSelectedChannel;
    stream << mBumpEnabled;
    stream << mExportToDeferred;

    std::list<std::string> signalsToWrite;
    std::list<std::string> s1 = mRgbaChannel->save( stream );
    std::list<std::string> s3 = mRgbaFarChannel->save( stream );
    std::list<std::string> s2 = mBumpChannel->save( stream );
    // TODO: SPEC
    //std::list<std::string> s3 = mSpecChannel->save( aStream );

    signalsToWrite = s1;
    signalsToWrite.merge( s2 );
    signalsToWrite.merge( s3 );

    stream << int( signalsToWrite.size() );

    for (std::list<std::string>::iterator it=signalsToWrite.begin(); it!=signalsToWrite.end(); ++it)
    {
      int index = indexSignal( *it );
      mSignals[index]->save( stream );
    }

    aFile.flush();
}

ProcShader::ProcShader( QFile& aFile )
{
    QDataStream stream(&aFile);

    QString s;
    stream >> s; mName = s.toStdString();
    stream >> s; mEditorVersion = s.toStdString();
    stream >> mSelectedChannel;
    stream >> mBumpEnabled;
    stream >> mExportToDeferred;

    mRgbaChannel    = new ChannelRgba( &mNewIdentifiers,  Channel::eChannel_rgba,     getNameChannelType(Channel::eChannel_rgba)  );
    mRgbaFarChannel = new ChannelRgba( &mNewIdentifiers,  Channel::eChannel_rgba_far, getNameChannelType(Channel::eChannel_rgba_far)  );
    mBumpChannel    = new ChannelBump( &mNewIdentifiers,  Channel::eChannel_bump,     getNameChannelType(Channel::eChannel_bump)  );
    mSpecChannel    = new ChannelSpec(                    Channel::eChannel_spec,     getNameChannelType(Channel::eChannel_spec) );

    mRgbaChannel->load( stream );
    mRgbaFarChannel->load( stream );
    mBumpChannel->load( stream );
    // TODO: SPEC
    //mSpecChannel->load( stream );

    mNewIdentifiers.setLayerId( mRgbaChannel->numLayers()
                                + mRgbaFarChannel->numLayers()
                                + mBumpChannel->numLayers() );


    stream >> mNumSignals;


    for ( int i = 0; i < mNumSignals ; ++i)
    {
      Signal* l = Signal::load( stream );

      //std::string newName = l->prependChannelName( mSelectedChannelPointer->channelName() );

      //  if a signal with the same name exists, "overwrite" it
      if ( signalExists( l->getName() ) )
      {
        deleteSignal( l->getName() );
      }

      mSignals.push_back( l );

      //  Prepend the channel name to the signal
      //modifySignalName ( l->getName(), newName );
    }

    mNewIdentifiers.setSignalId( mNumSignals + 2 );

    mSelectedChannelPointer = mRgbaChannel;
}


void  ProcShader::modifySignalWarp    (string& aName, string& aWarpSignalName, float aWarpLength, bool aWarpInUse )
{
  int index = indexSignal ( aName );
  mSignals[index]->setWarp( aWarpSignalName, aWarpLength, aWarpInUse );

}

WarpInfo ProcShader::getWarpInfo( std::string& aSignalName ) const
{
  WarpInfo w;
  int index = indexSignal ( aSignalName );
  w.inUse           = mSignals[index]->hasWarp();
  w.length          = mSignals[index]->warpLength();
  w.warpSignalName  = mSignals[index]->warpSignal();
  return w;
}

int         ProcShader::getNumLayerColourMapping() const
{
  return LayerSignal::msNumLayerColourMapping;
}

std::string ProcShader::getNameLayerColourMapping(int i) const
{
  return LayerSignal::msLayerColourMappingNames[i];
}

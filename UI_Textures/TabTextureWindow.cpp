#include "TabTextureWindow.hpp"
#include "Timing/Timing_windows_x64.hpp"

TabTextureWindow::TabTextureWindow(QWidget *parent):
  QFrame(parent),
  mAddNewSignalLayer(NULL),
  mDisableRedraw(false)
{
  mEditor = new EditorMaterial();
  mEditor->newFile();
  mIndexSelectedChannel = 0;

  std::vector<std::string> blocks (2);
  blocks[0] = std::string("Generate shader (IO included)");
  blocks[1] = std::string("Compile shader via DX");
  mProfiler = new ProfilerDp( 2, blocks );

  if ( !initDxWidgets() )
    QMessageBox::warning (this,tr("Fatal error"),tr("Mat: Dx init") );

  mMainLayout = new QHBoxLayout (this);
  mMainLayout->setAlignment( Qt::AlignTop );
  setLayout(mMainLayout);
  isVisible();

  mChannelLayout = new QVBoxLayout(this);
  mSignLayLayout = new QVBoxLayout(this);
  mPreviewLayout = new QVBoxLayout(this);
  mSignLayLayoutPreScroll = new QVBoxLayout(this);

  mChannelLayout->setAlignment( Qt::AlignTop );
  mSignLayLayout->setAlignment( Qt::AlignTop );  
  mPreviewLayout->setAlignment( Qt::AlignTop );
  mSignLayLayoutPreScroll->setAlignment( Qt::AlignTop );

  mChannelLayout->setSizeConstraint( QLayout::SetMinimumSize );
  mSignLayLayout->setSizeConstraint( QLayout::SetMinimumSize );  
  mPreviewLayout->setSizeConstraint( QLayout::SetMinimumSize );
  mSignLayLayoutPreScroll->setSizeConstraint( QLayout::SetMinimumSize );

  mMainLayout->addLayout( mChannelLayout );
  mMainLayout->addLayout( mSignLayLayoutPreScroll );
  mMainLayout->addLayout( mPreviewLayout );


  //  Channel Layout  
  mRGB    = new ChannelButton(this, QString("RGB - near"),  Channel::eChannel_rgba);
  mRGBFar = new ChannelButton(this, QString("RGB - far"),   Channel::eChannel_rgba_far);
  mBump   = new ChannelButton(this, QString("Bump"),        Channel::eChannel_bump);
  mSpec   = new ChannelButton(this, QString("Spec"),        Channel::eChannel_spec);
  //mAlpha  = new ChannelButton(this, QString("Alpha"));

  mBumpCheckBox = new QCheckBox(this);
  mBumpCheckBox->setText( "Enable Bump" );
  mBumpCheckBox->setChecked( false );

  connect( mRGB,  SIGNAL(selectChannel(int)), this, SLOT(channelSelected(int)));
  connect( mRGBFar,  SIGNAL(selectChannel(int)), this, SLOT(channelSelected(int)));
  connect( mBump, SIGNAL(selectChannel(int)), this, SLOT(channelSelected(int)));
  connect( mSpec, SIGNAL(selectChannel(int)), this, SLOT(channelSelected(int)));
  connect( mBumpCheckBox, SIGNAL(stateChanged ( int) ), this, SLOT(changeShaderPropierties()));


  mChannelLayout->addWidget( mRGB );
  mChannelLayout->addWidget( mRGBFar );
  //mChannelLayout->addWidget( mAlpha );
  mChannelLayout->addWidget( mBump );
  mChannelLayout->addWidget( mSpec );

  mChannelLayout->addWidget( mBumpCheckBox );

  mAddNewSignalLayer = new QPushButton(this);
  mAddNewSignalLayer->setText( QString("Add new") );
  mAddNewSignalLayer->setFixedWidth( 550 );
  connect ( mAddNewSignalLayer, SIGNAL(clicked()), this, SLOT(addNewSignal()) );
  mAddNewSignalLayer->show();

  //  Scroll area for signals
  mSignalsScrollArea = new QScrollArea(this);

  mSignalsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mSignalsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mSignalsScrollArea->setWidgetResizable(false);

  //QVBoxLayout * scrollAreaVLayout = new QVBoxLayout();
  mSignalsScrollAreaContents = new QWidget(this);
  mSignalsScrollAreaContents->setLayout(mSignLayLayout);
  //scrollAreaVLayout->setSizeConstraint(QLayout::SetFixedSize);

  mSignalsScrollArea->setWidget(mSignalsScrollAreaContents);
  mSignLayLayoutPreScroll->addWidget(mSignalsScrollArea);


  //  Channels
  initChannelWidgets();

  //  SignLay Layout  
  getSignalsAndLayouts();


  //  Preview Layout
  mPreviewLayout->addWidget( mFinalMaterial );
  mPreviewLayout->addWidget( mCurrentSignLay );
}

TabTextureWindow::~TabTextureWindow()
{
  mProfiler->dumpToFile( L"profile.txt" );
}

bool TabTextureWindow::initDxWidgets()
{
  int width  = 600;
  int height = 400;

  mFinalMaterial = new QD3DWidgetTexture(this , width, height, false);
  if (!mFinalMaterial->init()) {
    QMessageBox::warning (this,tr("Init error"),tr("Init didn't go well"));
    mFinalMaterial->close();
    return false;
  }
  else {
    mFinalMaterial->setFocusPolicy( Qt::StrongFocus );
    mFinalMaterial->setFixedSize (width, height);
  }

  mCurrentSignLay = new QD3DWidgetTexture( this , width, height, false);
  if (!mCurrentSignLay->init()) {
    QMessageBox::warning (this,tr("Init error"),tr("Init didn't go well"));
    mCurrentSignLay->close();
    return false;
  }
  else {
    mCurrentSignLay->setFocusPolicy( Qt::StrongFocus );
    mCurrentSignLay->setFixedSize (width, height);
  }



  //mFinalMaterial->setCamera( D3DXVECTOR3(2,2,1), D3DXVECTOR3(0,0,0) );
  //mCurrentSignLay->setCamera( D3DXVECTOR3(3,3,2), D3DXVECTOR3(0,1,0) );
  return true;
}


void TabTextureWindow::getSignalsAndLayouts()
{
  if ( mIndexSelectedChannel == Channel::eChannel_rgba ||
       mIndexSelectedChannel == Channel::eChannel_rgba_far ||
       mIndexSelectedChannel == Channel::eChannel_bump  )
    getSignalsAndLayouts_ModeList();
}

void TabTextureWindow::removeSignalsAndLayouts()
{
  if ( mSignLayers.size() != 0)
  {
    for (unsigned int i = 0; i < mSignLayers.size() ; ++i)
    {
      disconnect( mSignLayers[i], SIGNAL(forceDeleteListLayerAndSignal()), this, SLOT(deleteListLayerAndSignal()));
      disconnect( mSignLayers[i], SIGNAL(forceReloadLayersAndSignals()), this, SLOT(reloadSignalsAndLayers()));
      disconnect( mSignLayers[i], SIGNAL(forceReloadSignals()), this, SLOT(reloadSignals()));
      mSignLayLayout->removeWidget( mSignLayers[i] );
      //delete mSignLayers[i];
      mSignLayers[i]->deleteLater();

    }
    if (mAddNewSignalLayer != NULL)
      mSignLayLayout->removeWidget( mAddNewSignalLayer );
  }
  mSignLayers.resize(0);
}

void TabTextureWindow::getSignalsAndLayouts_ModeList()
{
  layersSig = mEditor->getLayersSigNames_ModeList();
  layersOp = mEditor->getLayersOpNames_ModeList();
  layersOp.push_back( std::string ("none") );

  removeSignalsAndLayouts();

  mSignLayers.resize( layersSig.size() );
  for (unsigned int i = 0; i < mSignLayers.size() ; ++i)
  {
    Layer* l = mEditor->mProcShader->getLayer(layersSig[i]);
    bool isLast  = (i == (mSignLayers.size() - 1) );
    bool isAlone = (1 == mSignLayers.size() );
    mSignLayers[i] = new SignalLayerWidget(this,
                                           l,
                                           isLast,
                                           isAlone,
                                           layersOp[i],
                                           mEditor->mProcShader,
                                           mIndexSelectedChannel,
                                           mFinalMaterial,
                                           mCurrentSignLay,
                                           mProfiler);

    connect( mSignLayers[i], SIGNAL(forceDeleteListLayerAndSignal()), this, SLOT(deleteListLayerAndSignal()));
    connect( mSignLayers[i], SIGNAL(forceReloadLayersAndSignals()), this, SLOT(reloadSignalsAndLayers()));
    connect( mSignLayers[i], SIGNAL(forceReloadSignals()), this, SLOT(reloadSignals()));
    connect( mSignLayers[i], SIGNAL(setDragSource(const std::string)),
             this, SLOT(getSourceDrag( const std::string)));
    connect( mSignLayers[i], SIGNAL(setDragDestination(const std::string, const bool)),
             this, SLOT(getDestinationDrag( const std::string, const bool)));

    mSignLayLayout->addWidget( mSignLayers[i] );

  }

  mSignLayLayout->addWidget( mAddNewSignalLayer );

}

void    TabTextureWindow::addNewSignal_ModeList()
{
  /*
    // old code with slider for new op

  int opId = mSignLayers[index]->getOpToCompositeId();
  float blendFactor = mSignLayers[index]->compositeLayerBlendFactor();
  string father = mSignLayers[index]->getLayerName();
  mEditor->createComposition( opId, blendFactor , father );
  */
  int index = mSignLayers.size() - 1;
  string father = mSignLayers[index]->getLayerName();
  mEditor->createComposition( LayerOp::eOpAdd, 0.5f , father );
}

void    TabTextureWindow::addNewSignal()
{
  addNewSignal_ModeList();
  getSignalsAndLayouts();
}

void TabTextureWindow::reloadSignalsAndLayers()
{
  getSignalsAndLayouts();
}

/*
  Used to reload all signals, so if a signal is edited in
  two layers, changing one will affect the other.
*/
void TabTextureWindow::reloadSignals()
{
  for (unsigned int i = 0; i < mSignLayers.size() ; ++i)
    mSignLayers[i]->loadSignalVariables();
}

void TabTextureWindow::getSourceDrag(const std::string aName, const bool aIsLastLayer )
{
  mDragSourceLayer = aName;
  mDraggingOverLastLayer = aIsLastLayer;

  for (unsigned int i = 0; i < mSignLayers.size() ; ++i)
    mSignLayers[i]->setDragging(true);
}

void TabTextureWindow::getDestinationDrag(const std::string aDragDestinationLayer, const bool aIsLastLayer )
{
  for (unsigned int i = 0; i < mSignLayers.size() ; ++i)
    mSignLayers[i]->setDragging(false);

  mDraggingOverLastLayer |= aIsLastLayer;
  if ( mDragSourceLayer != aDragDestinationLayer &&
       mDragSourceLayer != std::string("") &&
       aDragDestinationLayer != std::string("") )
  {
    mEditor->swapLayerSignal( mDragSourceLayer, aDragDestinationLayer, !mDraggingOverLastLayer );
    reloadSignalsAndLayers();
  }
}

void TabTextureWindow::channelSelected( int aChannelId )
{
  if ( mIndexSelectedChannel != aChannelId)
  {
    int previousChannel = mIndexSelectedChannel;
    mIndexSelectedChannel = aChannelId;    
    mEditor->mProcShader->selectChannel( aChannelId );
    setChannelWidget( previousChannel );
    getSignalsAndLayouts();
  }
}

void TabTextureWindow::changeShaderPropierties()
{
  mEditor->mProcShader->setBumpCnfg( mBumpCheckBox->isChecked() );
  getSignalsAndLayouts();
}

void TabTextureWindow::setChannelWidget( const int aPreviousChannel )
{
  mDisableRedraw = true;
  if ( mIndexSelectedChannel == Channel::eChannel_rgba ||
       mIndexSelectedChannel == Channel::eChannel_rgba_far ||
       mIndexSelectedChannel == Channel::eChannel_bump )
  {
    //  remove previous widget
    if ( aPreviousChannel == Channel::eChannel_spec )
    {
      mSignLayLayout->removeWidget( mChannelSpecWidget );
      mSignLayLayout->addWidget( mChannelWidget );
      mChannelSpecWidget->hide();
      mChannelWidget->show();
      mAddNewSignalLayer->show();
    }

    mChannelWidget->changeName(
      QString::fromStdString(mEditor->mProcShader->channelName()) );
    mChannelWidget->changeDepth(
       mEditor->mProcShader->channelDepth() );
  }
  else
  {
    if ( mIndexSelectedChannel == Channel::eChannel_spec )
    {
      //  remove previous widget
      if ( aPreviousChannel == Channel::eChannel_rgba ||
           aPreviousChannel == Channel::eChannel_rgba_far ||
           aPreviousChannel == Channel::eChannel_bump )
      {
        mSignLayLayout->removeWidget( mChannelWidget );
        mSignLayLayout->addWidget( mChannelSpecWidget );
        mChannelWidget->hide();
        mAddNewSignalLayer->hide();
        mChannelSpecWidget->show();
        removeSignalsAndLayouts();
      }
    }
  }
  mDisableRedraw = false;
}

void TabTextureWindow::initChannelWidgets()
{

  //  init channel widget: bump/color
  mChannelWidget = new ChannelWidget( this, 0,
    QString::fromStdString(mEditor->mProcShader->channelName()),
    mEditor->mProcShader->channelDepth()
    );
  connect( mChannelWidget->mNameButton, SIGNAL(changed()), this, SLOT(channelNameChanged()));
  connect( mChannelWidget->mDepthSpin, SIGNAL(valueChanged(double)), this, SLOT(channelDepthChanged()));
  mSignLayLayout->addWidget( mChannelWidget );

  connect( mChannelWidget, SIGNAL(channelSelectedForLoading(QString)), this, SLOT(channelLoad(QString)) );
  connect( mChannelWidget, SIGNAL(channelSelectedForSaving(QString)), this, SLOT(channelSave(QString)) );
  connect( mChannelWidget, SIGNAL(channelAskList()), this, SLOT(channelList()) );

  //  init spec channel widget
  mChannelSpecWidget = new ChannelSpecWidget( this, 0,
                                              QString::fromStdString(std::string("Specular")) );
  mChannelSpecWidget->hide();
  connect( mChannelSpecWidget, SIGNAL(changed()), this, SLOT(channelSpecChanged()) );
  connect( mChannelSpecWidget, SIGNAL(changed()), this, SLOT(channelSpecChanged()) );


}

void TabTextureWindow::channelNameChanged()
{
  mEditor->mProcShader->setChannelName(
        mChannelWidget->mNameButton->text().toStdString() );
}

void TabTextureWindow::channelDepthChanged()
{
  if ( !mDisableRedraw )
  {
    mEditor->mProcShader->setChannelDepth(
          mChannelWidget->mDepthSpin->value() );

    mSignLayers[0]->redrawSignalPreview();
    mSignLayers[0]->redrawFinal();
  }
}


void TabTextureWindow::channelLoad(QString aName)
{  
  QString path =  QCoreApplication::applicationDirPath() +
                  QString("./ChannelLibrary/")+
                  aName;

  QFile file( path );
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for load"));
    return;
  } else {
      mEditor->channelLoad( file );
      reloadSignalsAndLayers();
      setChannelWidget( mIndexSelectedChannel );
  }
  file.close();
}

void TabTextureWindow::channelSave(QString aName)
{
  QString path =  QCoreApplication::applicationDirPath() +
                  QString("./ChannelLibrary/")+
                  aName +
                  QString(".ngc");

  QFile file( path );
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for save"));
    return;
  } else {
      mEditor->channelSave( file );
  }
 file.close();
}

void TabTextureWindow::shaderExport( QString& aName )
{
  mEditor->exportToFile( aName );
}

void TabTextureWindow::shaderExportDeferred( QString& aName, int aVertexLayout )
{
  mEditor->exportToFileDeferred( aName, aVertexLayout );
}


std::vector<QString> TabTextureWindow::channelLibraryList()
{
  std::vector<QString> ret;

  QDir dir;
  dir.cd( QCoreApplication::applicationDirPath() +
          QString("./ChannelLibrary/") );
  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  dir.setSorting(QDir::Size | QDir::Reversed);

  QFileInfoList list = dir.entryInfoList();
  std::cout << "     Bytes Filename" << std::endl;
  for (int i = 0; i < list.size(); ++i)
  {
    QFileInfo fileInfo = list.at(i);
    ret.push_back( fileInfo.fileName() );
  }

  return ret;
}

void TabTextureWindow::channelList()
{
  mChannelWidget->getListChannels( channelLibraryList() );
}

void TabTextureWindow::deleteListLayerAndSignal()
{
  mEditor->deleteLastLayerSignal( layersOp[layersOp.size()-2],
                                  layersSig[layersSig.size()-1],
                                  layersSig[layersSig.size()-2]);
  reloadSignalsAndLayers();
}

void TabTextureWindow::channelSpecChanged()
{
  mEditor->mProcShader->setSpecColour( mChannelSpecWidget->mSpecColour );
  mEditor->mProcShader->setSpecPercentage( mChannelSpecWidget->mSpecPercentage );
}


void    TabTextureWindow::loadFromFile( QFile& aFile )
{
    mEditor->loadFromFile(aFile);
    reloadSignalsAndLayers();
}

void    TabTextureWindow::saveToFile  ( QFile& aFile )
{
    mEditor->saveToFile(aFile);
}

#include "UI_Textures/SignalLayerWidget.hpp"
#include "Editor_Texture/DataLayer/ProcShader.hpp"
//#include "Editor_Texture/EditorMaterial.hpp"
#include "Timing/Timing_windows_x64.hpp"
#include "UI_Textures/MinorWidgets/SplineNWidget.hpp"

using namespace std;

SignalLayerWidget::~SignalLayerWidget()
{

}

SignalLayerWidget::SignalLayerWidget(QWidget *parent,
                                     Layer* aLayer,
                                     bool aIsLastLayer,
                                     bool aIsAlone,
                                     std::string& aLayerOpName,
                                     ProcShader* aProcShader,
                                     int aChannelId,
                                     QD3DWidgetTexture *aFinalMaterial,
                                     QD3DWidgetTexture *aCurrentSignLay,
                                     ProfilerDp* aProfiler)
  : QFrame(parent)
{
  blockSignals(true);
  mDragging = false;
  setAcceptDrops(true);

  mProfiler = aProfiler;  

  mChannelId = aChannelId;
  mIsLastLayer = aIsLastLayer;
  mIsAlone = aIsAlone;
  mProcShader = aProcShader;
  mFinalMaterial  = aFinalMaterial;
  mCurrentSignLay = aCurrentSignLay;

  mInhibitSignalChangedSlot = false;
  mNewLayerOpModeSet = false;

  mCompositeLayerOpName = aLayerOpName;
  mNameLayer = aLayer->getName();
  LayerSignal *ls;
  ls = dynamic_cast<LayerSignal*> (aLayer);
  mSignal = ls->getSignal();
  mColor.resize( SplineNWidget::mMaxColours );

  ///////////////////////////
  //  other config
  mFreq1SliderSingleValue = true;
  mPhaseSliderSingleValue = true;

  ////////////////////////////////
  //  Main layout
  setFrameStyle(QFrame::Panel);

  mMainLayout = new QVBoxLayout (this);  
  setLayout(mMainLayout);
  mMainLayout->setAlignment( Qt::AlignTop );
  mMainLayout->setSizeConstraint( QLayout::SetMinimumSize );
  setLayout(mMainLayout);
  isVisible();

  mSignalLayout = new QGridLayout(this);
  mCompOpLayout = new QHBoxLayout(this);
  mLayerSignalSuperLayout = new QHBoxLayout(this);

  mLayerSignalSuperLayout->addLayout(mSignalLayout);
  mMainLayout->addLayout(mLayerSignalSuperLayout);
  if ( !mIsLastLayer )
    mMainLayout->addLayout(mCompOpLayout);

  mCompOpLayout->setAlignment( Qt::AlignLeft);

  ////////////////////////////////////////
  // mLayerSignalSuperLayout
  mLayerSignalSuperLayout->setAlignment( Qt::AlignTop);
  mLayerSignalButtonsLayout = new QVBoxLayout(this);
  mLayerSignalButtonsLayout->setAlignment( Qt::AlignTop);
  mLayerSignalSuperLayout->addLayout( mLayerSignalButtonsLayout );

  mDeleteLayerButtton = new QPushButton(this);
  mReinitSignalButtton = new QPushButton(this);
  mDeleteLayerButtton->setText( QString("Del") );
  mReinitSignalButtton->setText( QString("Reinit") );
  connect(mDeleteLayerButtton, SIGNAL(clicked()), this, SLOT(signalToBeDeleted()));
  connect(mReinitSignalButtton, SIGNAL(clicked()), this, SLOT(signalToBeReinitialized()));

  mLayerSignalButtonsLayout->addWidget( mDeleteLayerButtton );
  mLayerSignalButtonsLayout->addWidget( mReinitSignalButtton );

  ////////////////////////////////////////
  // mLayerLayout
  {

    //  Signal choosing
    mChooseSignal = new QComboBox(this);
    std::vector<std::string> signalsNames = mProcShader->getSignalsNames();
    for (unsigned int i = 0; i < signalsNames.size() ; ++i)
    {
      mChooseSignal->addItem( QString::fromStdString( signalsNames[i]) );
      if (mSignal == signalsNames[i])
        mIndexOfLayerSignal = i;
    }
    mChooseSignal->setFixedWidth( mChooseSignal->sizeHint().width() );


    //  Type
    mChooseSignalType = new QComboBox(this);
    for (int i = 0; i < mProcShader->getNumSignalTypes() ; ++i)
    {
      mChooseSignalType->addItem(
            QString::fromStdString(
              mProcShader->getNameSignalType(i)) );
    }
    mChooseSignalType->setFixedWidth( mChooseSignalType->sizeHint().width() );

    connect( mChooseSignal, SIGNAL(currentIndexChanged ( const QString&)), this, SLOT(changeSignalOfLayer(const QString&)));
    connect( mChooseSignalType, SIGNAL( currentIndexChanged(int) ), this, SLOT(signalTypeChanged()));


  }

  ////////////////////////////////////////
  // mSignalLayout
  {
    Signal* sp = mProcShader->getSignal( mSignal );
    int signalType = sp->signalType();

    if ( signalType == Signal::eIdSignalNoise ||
         signalType == Signal::eIdSignalMarble ||
         signalType == Signal::eIdSignalCurveVeins)
    {
      loadWidgetsSignalGeneral();
      loadWidgetsSignalNoise();
    }

    if ( signalType == Signal::eIdSignalNoiseSin )
    {
      loadWidgetsSignalGeneral();
      loadWidgetsSignalNoiseSin();
    }

    if ( signalType == Signal::eIdSignalKhan1 )
    {
      loadWidgetsSignalGeneral();
      loadWidgetsSignalKhan1();
    }

    if ( signalType == Signal::eIdSignalLines ||
         signalType == Signal::eIdSignalStrips ||
         signalType == Signal::eIdSignalSpheres ||
         signalType == Signal::eIdSignalTiledTriangles )
    {
      loadWidgetsSignalGeneral();
      loadWidgetsSignalLines();
    }



  }

  ////////////////////////////////////////
  // mCompOpLayout  
  if ( !mIsLastLayer )
  {
    mLayerLabel = new QLabel(this);
    mLayerLabel->setText(QString::fromStdString( mNameLayer) );
    mLayerLabel->setFrameStyle(QFrame::Panel);

    mOpBlendFactorSlider = new QSlider(Qt::Horizontal, this);
    mOpBlendFactorSlider->setRange ( 0, 400 );
    mOpBlendFactorSlider->setSliderPosition( 200 );
    mOpBlendFactorSlider->setSingleStep( 1 );
    mOpBlendFactorSlider->setFixedWidth( 150 );

    //  Operation
    mChooseOperation = new QComboBox(this);
    for (int i = 0; i < mProcShader->getNumOperations() ; ++i)
    {
      mChooseOperation->addItem(
            QString::fromStdString(
              mProcShader->getNameOperation(i)) );
    }
    mChooseOperation->setFixedWidth( mChooseOperation->sizeHint().width() );

    connect( mOpBlendFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(signalOpBlendChanged()));
    connect( mChooseOperation, SIGNAL(currentIndexChanged(int)), this, SLOT(signalOpChanged(int)));

    mCompOpLayout->addWidget( mLayerLabel,  Qt::AlignLeft );
    mCompOpLayout->addWidget( mChooseOperation, Qt::AlignLeft );
    mCompOpLayout->addWidget( mOpBlendFactorSlider, Qt::AlignLeft );

  }


  loadSignalVariables();
  loadCompositeOperationVariables();
  //signalChanged();
  blockSignals(false);

  redrawSignalPreview();
  redrawFinal();


  //  Only for profile, not real execution
  //profileShaderCompile();

}


void SignalLayerWidget::loadWidgetsSignalGeneral()
{
  mInputParamsLabel = new QLabel( this );
  mInputParamsLabel->setText( QString("Input Params: ") );


  //  Input Params choosing
  mChooseInputParams = new QComboBox(this);
  int numParm = mProcShader->getNumInputParameters();
  for (int i = 0; i < numParm ; ++i)
    mChooseInputParams->addItem(
          QString::fromStdString(
            mProcShader->getNameInputParameter(i))  );
  mChooseInputParams->setFixedWidth( mChooseInputParams->sizeHint().width() );
  connect( mChooseInputParams, SIGNAL(currentIndexChanged(int)), this, SLOT(signalChanged()));

  mWeightLabel = new QCheckBox(tr("Weight:"), this);
  mWeightLabel->setChecked( true );

  int rangeWeight = 3*10;
  mChooseWeightX = new QSlider(Qt::Horizontal, this);
  mChooseWeightX->setRange ( 0, rangeWeight );
  mChooseWeightX->setSliderPosition( 1 );
  mChooseWeightX->setSingleStep( 1 );
  mChooseWeightX->setFixedWidth( 100 );

  mChooseWeightY = new QSlider(Qt::Horizontal, this);
  mChooseWeightY->setRange ( 0, rangeWeight );
  mChooseWeightY->setSliderPosition( 1 );
  mChooseWeightY->setSingleStep( 1 );
  mChooseWeightY->setFixedWidth( 100 );

  mChooseWeightZ = new QSlider(Qt::Horizontal, this);
  mChooseWeightZ->setRange ( 0, rangeWeight );
  mChooseWeightZ->setSliderPosition( 1 );
  mChooseWeightZ->setSingleStep( 1 );
  mChooseWeightZ->setFixedWidth( 100 );

  mOctSpin      = new QSpinBox(this);
  mOctMultSpin  = new QDoubleSpinBox(this);

  Layer* l = mProcShader->getLayer( mNameLayer );
  ColourMap cmap = l->getColMap();
  mColorId  = cmap.mColourMappingId;
  mColor    = cmap.mColorValue;

  m2GradientPicker = new SplineNWidget(this, mColor, mColorId, mProcShader);

  //  Warp Signal choosing
  mChooseSignalWarp = new QComboBox(this);
  std::vector<std::string> signalsNames = mProcShader->getSignalsNames();
  for (unsigned int i = 0; i < signalsNames.size() ; ++i)
  {
    if ( QString::fromStdString( signalsNames[i]).compare( QString::fromStdString(mSignal)) )
      mChooseSignalWarp->addItem( QString::fromStdString( signalsNames[i]) );
  }
  mChooseSignalWarp->setFixedWidth( mChooseSignalWarp->sizeHint().width() );

  mWarpLengthSpin  = new QDoubleSpinBox(this);
  mWarpLengthSpin->setSingleStep(0.05);
  mWarpLengthSpin->setValue(0.05);

  mWarpCheckBox = new QCheckBox(this);
  mWarpCheckBox->setText( "Warp" );
  mWarpCheckBox->setChecked( false );
  
  mSignalLayout->addWidget( mChooseSignal,      0, 0, Qt::AlignTop );
  mSignalLayout->addWidget( mChooseSignalType,  1, 0, Qt::AlignLeft );
  mSignalLayout->addWidget( mOctSpin,           2, 0, Qt::AlignTop );
  mSignalLayout->addWidget( mOctMultSpin,       2, 1, Qt::AlignTop );
  mSignalLayout->addWidget( mInputParamsLabel,  3, 0, Qt::AlignTop );
  mSignalLayout->addWidget( mChooseInputParams, 3, 1, Qt::AlignTop );
  mSignalLayout->addWidget( mWarpCheckBox,      5, 0, Qt::AlignTop );
  mSignalLayout->addWidget( mChooseSignalWarp,  5, 1, Qt::AlignTop );
  mSignalLayout->addWidget( mWarpLengthSpin,    5, 2, Qt::AlignLeft );

  mSignalLayout->addWidget( m2GradientPicker,   0, 1, Qt::AlignLeft );
  mSignalLayout->addWidget( mWeightLabel,       1, 2, Qt::AlignTop );

  mSignalLayout->addWidget( mChooseWeightX,     2, 2, Qt::AlignTop );
  mSignalLayout->addWidget( mChooseWeightY,     3, 2, Qt::AlignTop );
  mSignalLayout->addWidget( mChooseWeightZ,     4, 2, Qt::AlignTop );

  connect( mChooseWeightX, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
  connect( mChooseWeightY, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
  connect( mChooseWeightZ, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
  connect( mWeightLabel, SIGNAL(stateChanged ( int) ), this, SLOT(signalChanged()));

  connect( mOctSpin,     SIGNAL(valueChanged(int)), this, SLOT(signalChanged()));
  connect( mOctMultSpin, SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));

  connect( m2GradientPicker, SIGNAL(changed()), this, SLOT(signalChanged()));

  connect( mChooseSignalWarp, SIGNAL(currentIndexChanged ( const QString&)), this, SLOT(changeWarpSignalOfLayer(const QString&)));
  connect( mWarpLengthSpin, SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));
  connect( mWarpCheckBox, SIGNAL(stateChanged ( int) ), this, SLOT(signalChanged()));
}


void SignalLayerWidget::loadWidgetsSignalNoise()
{
/*
  mTurbulenceSlider = new QSlider(Qt::Horizontal, this);
  mTurbulenceSlider->setRange ( 0, 100 );
  mTurbulenceSlider->setSliderPosition( 1 );
  mTurbulenceSlider->setSingleStep( 1 );
  mTurbulenceSlider->setFixedWidth( 100 );
  connect( mTurbulenceSlider, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));

  mSignalLayout->addWidget( mTurbulenceSlider,       1, 2, Qt::AlignTop );
  */
}


void SignalLayerWidget::loadWidgetsSignalNoiseSin()
{

}

void SignalLayerWidget::loadWidgetsSignalStrips()
{

}

void SignalLayerWidget::loadWidgetsSignalKhan1()
{
  mFreq1floatSpin = new QDoubleSpinBox(this);
  mFreq2floatSpin = new QDoubleSpinBox(this);
  mThresholdSpin  = new QDoubleSpinBox(this);
  mThresholdSpin->setSingleStep( 0.01 );
  mFreq1floatSpin->setSingleStep( 0.1 );
  mFreq2floatSpin->setSingleStep( 0.1 );
  mFreq1floatSpin->setRange( 0.0, 100.0);
  mFreq2floatSpin->setRange( 0.0, 100.0);
  connect( mFreq1floatSpin, SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));
  connect( mFreq2floatSpin, SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));
  connect( mThresholdSpin,  SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));

  mSignalLayout->addWidget( mFreq1floatSpin,     2, 3, Qt::AlignTop );
  mSignalLayout->addWidget( mFreq2floatSpin,     3, 3, Qt::AlignTop );
  mSignalLayout->addWidget( mThresholdSpin,     4, 3, Qt::AlignTop );
}

void SignalLayerWidget::loadWidgetsSignalLines()
{
  mFreq1Label = new QCheckBox(tr("Freq1"), this);
  mPhaseLabel = new QCheckBox(tr("Phase"), this);
  mFreq1Label->setChecked( true );
  mPhaseLabel->setChecked( true );

  mThresholdLabel = new QLabel(tr("Threshold"), this);

  mThresholdSpin  = new QDoubleSpinBox(this);
  mThresholdSpin->setSingleStep( 0.01 );
  connect( mThresholdSpin, SIGNAL(valueChanged(double)), this, SLOT(signalChanged()));

  {
    mFreq1SliderX  = new QSlider(Qt::Horizontal, this);
    mFreq1SliderY  = new QSlider(Qt::Horizontal, this);
    mFreq1SliderZ  = new QSlider(Qt::Horizontal, this);
    mPhaseSliderX  = new QSlider(Qt::Horizontal, this);
    mPhaseSliderY  = new QSlider(Qt::Horizontal, this);
    mPhaseSliderZ  = new QSlider(Qt::Horizontal, this);

    {
      mFreq1SliderX->setRange ( 0, 300 );
      mFreq1SliderX->setSliderPosition( 0 );
      mFreq1SliderX->setSingleStep( 1 );
      mFreq1SliderX->setFixedWidth( 100 );

      mFreq1SliderY->setRange ( 0, 300 );
      mFreq1SliderY->setSliderPosition( 0 );
      mFreq1SliderY->setSingleStep( 1 );
      mFreq1SliderY->setFixedWidth( 100 );

      mFreq1SliderZ->setRange ( 0, 300 );
      mFreq1SliderZ->setSliderPosition( 0 );
      mFreq1SliderZ->setSingleStep( 1 );
      mFreq1SliderZ->setFixedWidth( 100 );
    }

    {
      mPhaseSliderX->setRange ( 0, 100 );
      mPhaseSliderX->setSliderPosition( 0 );
      mPhaseSliderX->setSingleStep( 1 );
      mPhaseSliderX->setFixedWidth( 100 );

      mPhaseSliderY->setRange ( 0, 100 );
      mPhaseSliderY->setSliderPosition( 0 );
      mPhaseSliderY->setSingleStep( 1 );
      mPhaseSliderY->setFixedWidth( 100 );

      mPhaseSliderZ->setRange ( 0, 100 );
      mPhaseSliderZ->setSliderPosition( 0 );
      mPhaseSliderZ->setSingleStep( 1 );
      mPhaseSliderZ->setFixedWidth( 100 );
    }

    connect( mFreq1SliderX, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
    connect( mFreq1SliderY, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
    connect( mFreq1SliderZ, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
    connect( mPhaseSliderX, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
    connect( mPhaseSliderY, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));
    connect( mPhaseSliderZ, SIGNAL(sliderReleased()), this, SLOT(signalChanged()));

    connect( mFreq1Label, SIGNAL(stateChanged ( int) ), this, SLOT(signalChanged()));
    connect( mPhaseLabel, SIGNAL(stateChanged ( int) ), this, SLOT(signalChanged()));
    //void QCheckBox::stateChanged ( int state )
  }

  mSignalLayout->addWidget( mFreq1Label,     0, 3, Qt::AlignTop );
  mSignalLayout->addWidget( mFreq1SliderX,   1, 3, Qt::AlignTop );
  mSignalLayout->addWidget( mFreq1SliderY,   2, 3, Qt::AlignTop );
  mSignalLayout->addWidget( mFreq1SliderZ,   3, 3, Qt::AlignTop );

  mSignalLayout->addWidget( mPhaseLabel,     0, 4, Qt::AlignTop );
  mSignalLayout->addWidget( mPhaseSliderX,   1, 4, Qt::AlignTop );
  mSignalLayout->addWidget( mPhaseSliderY,   2, 4, Qt::AlignTop );
  mSignalLayout->addWidget( mPhaseSliderZ,   3, 4, Qt::AlignTop );

  mSignalLayout->addWidget( mThresholdLabel,  1, 5, Qt::AlignTop );
  mSignalLayout->addWidget( mThresholdSpin,   2, 5, Qt::AlignTop );

}


void SignalLayerWidget::loadSignalVariables()
{
  blockSignals(true);
  mInhibitSignalChangedSlot = true;

  Signal* sp = mProcShader->getSignal( mSignal );
  mSignalType = sp->signalType();
  mChooseSignalType->setCurrentIndex( mSignalType );

  sp->getParams( mInputParams, mNoiseW, mNoiseNoct, mNoiseMoct);

  bool  weightIsChecked = ( mNoiseW.x == mNoiseW.y && mNoiseW.x == mNoiseW.z );
  mWeightLabel->setChecked( weightIsChecked );

  mChooseWeightX->setValue( mNoiseW.x*10.0 );
  mChooseWeightY->setValue( mNoiseW.y*10.0 );
  mChooseWeightZ->setValue( mNoiseW.z*10.0 );


  mOctSpin->setValue    ( mNoiseNoct);
  mOctMultSpin->setValue( mNoiseMoct);

  WarpInfo w = mProcShader->getWarpInfo( mSignal );
  mWarpLengthSpin->setValue( w.length );
  mWarpCheckBox->setChecked( w.inUse );
  mChooseSignalWarp->setCurrentIndex( mChooseSignalWarp->findText( QString::fromStdString(w.warpSignalName )) );

  Layer* l = mProcShader->getLayer( mNameLayer );
  ColourMap cmap = l->getColMap();
  mColorId  = cmap.mColourMappingId;
  mColor    = cmap.mColorValue;

  for ( int i = 0; i < mColor.size(); ++i )
    m2GradientPicker->mStoredColour[i] = &mColor[i];
  m2GradientPicker->changePen();

  // TODO: specific code
  if ( mSignalType == Signal::eIdSignalNoise )
  {
    SignalNoise* sn = dynamic_cast<SignalNoise*> (sp);
    /*
    mTurbulence = sn->turbulence();
    mTurbulenceSlider->setValue( mTurbulence*10 );
*/
  }

  if ( mSignalType == Signal::eIdSignalStrips ||
       mSignalType == Signal::eIdSignalLines ||
       mSignalType == Signal::eIdSignalSpheres ||
       mSignalType == Signal::eIdSignalTiledTriangles)
  {    
   if ( mSignalType == Signal::eIdSignalStrips )
   {
     SignalStrips* sstrip = dynamic_cast<SignalStrips*> (sp);
     sstrip->getNoiseParams( mComposedFreq1, mPhase, mThreshold);
   }
   if ( mSignalType == Signal::eIdSignalLines )
   {
     SignalLines* sl = dynamic_cast<SignalLines*> (sp);
     sl->getNoiseParams( mComposedFreq1, mPhase, mThreshold);
   }
   if ( mSignalType == Signal::eIdSignalSpheres )
   {
     SignalSpheres* sl = dynamic_cast<SignalSpheres*> (sp);
     sl->getNoiseParams( mComposedFreq1, mPhase, mThreshold);
   }
   if ( mSignalType == Signal::eIdSignalTiledTriangles )
   {
     SignalTiledTriangles* sl = dynamic_cast<SignalTiledTriangles*> (sp);
     sl->getNoiseParams( mComposedFreq1, mPhase, mThreshold);
   }

/*
   mFreq1.x = mComposedFreq1.x / mNoiseW.x;
   mFreq1.y = mComposedFreq1.y / mNoiseW.y;
   mFreq1.z = mComposedFreq1.z / mNoiseW.z;
   */
   mFreq1.x = mNoiseW.x / mComposedFreq1.x;
   mFreq1.y = mNoiseW.y / mComposedFreq1.y;
   mFreq1.z = mNoiseW.z / mComposedFreq1.z;

   bool  freq1IsChecked = ( mFreq1.x == mFreq1.y && mFreq1.x == mFreq1.z );
   mFreq1Label->setChecked( freq1IsChecked );
   mFreq1SliderX->setValue( mFreq1[0]*100 );
   mFreq1SliderY->setValue( mFreq1[1]*100 );
   mFreq1SliderZ->setValue( mFreq1[2]*100 );

   bool  phaseIsChecked = ( mPhase.x == mPhase.y && mPhase.x == mPhase.z );
   mPhaseLabel->setChecked( phaseIsChecked );
   mPhaseSliderX->setValue( mPhase[0]*100 );
   mPhaseSliderY->setValue( mPhase[1]*100 );
   mPhaseSliderZ->setValue( mPhase[2]*100 );

   mThresholdSpin->setValue(mThreshold);
  }




  if ( mSignalType == Signal::eIdSignalKhan1 )
  {
    SignalKhan1* sl = dynamic_cast<SignalKhan1*> (sp);
    sl->getNoiseParams( mFreq1float, mFreq2float, mThreshold);
    mFreq1floatSpin->setValue(mFreq1float);
    mFreq2floatSpin->setValue(mFreq2float);
    mThresholdSpin->setValue(mThreshold);
  }

  mInhibitSignalChangedSlot = false;
  blockSignals(false);
}


void SignalLayerWidget::loadCompositeOperationVariables()
{
  blockSignals(true);
  mInhibitSignalChangedSlot = true;

  mChooseSignal->setCurrentIndex( mIndexOfLayerSignal );
  mChooseInputParams->setCurrentIndex( mInputParams );

  if ( !mIsLastLayer )
  {
    if ( mCompositeLayerOpName == std::string ("none")  )
    {
      if ( !mNewLayerOpModeSet )
      {
        mCompositeLayerOpBlend = 0.5f;
        mNewLayerOpModeSet = true;
      }
    }
    else
    {
      LayerOp* layerOpPointer = dynamic_cast<LayerOp*> (mProcShader->getLayer( mCompositeLayerOpName ) );
      mCompositeLayerOpBlend = layerOpPointer->blendFactor();
    }

    mOpBlendFactorSlider->setValue( mCompositeLayerOpBlend*float( mOpBlendFactorSlider->maximum()));

    //  Get composite operation
    if ( mCompositeLayerOpName == std::string("none") )
      mCompositeLayerOpId = 0;
    else
    {
      LayerOp* lo = dynamic_cast<LayerOp*> ( mProcShader->getLayer( mCompositeLayerOpName ) );
      mCompositeLayerOpId = lo->getOperation();
    }
    mChooseOperation->setCurrentIndex( mCompositeLayerOpId );
  }

  mInhibitSignalChangedSlot = false;
  blockSignals(false);
}

void SignalLayerWidget::signalChanged()
{
  if (mInhibitSignalChangedSlot)
    return;

  blockSignals(true);
  mInhibitSignalChangedSlot = true;

  mSignalType = mChooseSignalType->currentIndex();

  mInputParams = mChooseInputParams->currentIndex();

  if ( !mWeightLabel->isChecked() )
  {
    mNoiseW.x = mChooseWeightX->value() / 10.0;
    mNoiseW.y = mChooseWeightY->value() / 10.0;
    mNoiseW.z = mChooseWeightZ->value() / 10.0;
  }
  else
    mNoiseW = getDifferentValue(
                mChooseWeightX->value() / 10.0,
                mChooseWeightY->value() / 10.0,
                mChooseWeightZ->value() / 10.0 );

  mNoiseNoct = mOctSpin->value();
  mNoiseMoct = mOctMultSpin->value();

  //  change signal
  mProcShader->modifySignal (mSignal, mSignalType, mInputParams, mNoiseW, mNoiseNoct, mNoiseMoct);

  // warp
  if (  mChooseSignalWarp->currentIndex() == -1 )
    mWarpCheckBox->setChecked( false );
  mProcShader->modifySignalWarp( mSignal, mWarpSignalName, mWarpLengthSpin->value(), mWarpCheckBox->isChecked() );

  // colour
  mColor.resize( m2GradientPicker->mNumColor );
  mColorId = m2GradientPicker->mColorId;
  m2GradientPicker->colours( mColor );

  Layer* l = mProcShader->getLayer( mNameLayer );
  LayerSignal* ls = dynamic_cast<LayerSignal*>( l );
  ls->setColMap( ColourMap( mColorId, mColor ) );

  //specific

  Signal* sp = mProcShader->getSignal( mSignal );
  if ( mSignalType == Signal::eIdSignalNoise )
  {
    SignalNoise* sn = dynamic_cast<SignalNoise*> (sp);
    /*
    mTurbulence = mTurbulenceSlider->value() * 0.1f;
    sn->setTurbulence( mTurbulence );
    */
  }

  if ( mSignalType == Signal::eIdSignalStrips ||
       mSignalType == Signal::eIdSignalLines ||
       mSignalType == Signal::eIdSignalSpheres ||
       mSignalType == Signal::eIdSignalTiledTriangles)
  {
    if ( !mFreq1Label->isChecked() )
    {
      mFreq1[0] = mFreq1SliderX->value()*0.01f;
      mFreq1[1] = mFreq1SliderY->value()*0.01f;
      mFreq1[2] = mFreq1SliderZ->value()*0.01f;
    }
    else
      mFreq1 = getDifferentValue( mFreq1SliderX->value()*0.01f, mFreq1SliderY->value()*0.01f,  mFreq1SliderZ->value()*0.01f );
    /*
    mComposedFreq1.x = 1/ (mFreq1.x * mNoiseW.x);
    mComposedFreq1.y = 1/ (mFreq1.y * mNoiseW.y);
    mComposedFreq1.z = 1/ (mFreq1.z * mNoiseW.z);
    */
    mComposedFreq1.x =mNoiseW.x / mFreq1.x ;
    mComposedFreq1.y =mNoiseW.y / mFreq1.y ;
    mComposedFreq1.z =mNoiseW.z / mFreq1.z ;

    if ( !mPhaseLabel->isChecked() )
    {
      mPhase[0] = mPhaseSliderX->value()*0.01f;
      mPhase[1] = mPhaseSliderY->value()*0.01f;
      mPhase[2] = mPhaseSliderZ->value()*0.01f;
    }
    else
      mPhase = getDifferentValue( mPhaseSliderX->value()*0.01f, mPhaseSliderY->value()*0.01f, mPhaseSliderZ->value()*0.01f );



    mThreshold  = mThresholdSpin->value();

    if ( mSignalType == Signal::eIdSignalStrips )
    {
      SignalStrips* sstrip = dynamic_cast<SignalStrips*> (sp);
      sstrip->setNoiseParams( mComposedFreq1, mPhase, mThreshold );
    }
    if ( mSignalType == Signal::eIdSignalLines)
    {
      SignalLines* sl = dynamic_cast<SignalLines*> (sp);
      sl->setNoiseParams( mComposedFreq1, mPhase, mThreshold );
    }
    if ( mSignalType == Signal::eIdSignalSpheres)
    {
      SignalSpheres* sl = dynamic_cast<SignalSpheres*> (sp);
      sl->setNoiseParams( mComposedFreq1, mPhase, mThreshold );
    }
    if ( mSignalType == Signal::eIdSignalTiledTriangles)
    {
      SignalTiledTriangles* sl = dynamic_cast<SignalTiledTriangles*> (sp);
      sl->setNoiseParams( mComposedFreq1, mPhase, mThreshold );
    }
  }


  if ( mSignalType == Signal::eIdSignalKhan1 )
  {
    SignalKhan1* sl = dynamic_cast<SignalKhan1*> (sp);
    mFreq1float = mFreq1floatSpin->value();
    mFreq2float = mFreq2floatSpin->value();
    mThreshold  = mThresholdSpin->value();
    sl->setNoiseParams( mFreq1float, mFreq2float, mThreshold);
  }



  mProcShader->mSelectedChannelPointer->modifiyLayer ( mNameLayer,
                                                       mSignal);

  mProcShader->mSelectedChannelPointer->setColMap( mNameLayer, ColourMap( mColorId, mColor) );

  /*
  mProcShader->modifyLayerSA ( mNameLayer,
                              mSignal,
                              D3DXVECTOR4(mColor1.r, mColor1.g, mColor1.b, mColor1.a ),
                              D3DXVECTOR4(mColor2.r, mColor2.g, mColor2.b, mColor2.a ) );
*/
  redrawSignalPreview();
  redrawFinal();

  mInhibitSignalChangedSlot = false;
  blockSignals(false);

  emit forceReloadSignals();
}

void SignalLayerWidget::redrawSignalPreview()
{
  std::string signalName = ProcShaderIO::formatSignalName( mSignal );
  std::wstring path = makeShaderFilePath( signalName );

  mCurrentSignLay->setPreviewSignalName(
        mProcShader->getNameChannelType(mChannelId),
        mSignal );
  mProcShader->previewLayerSignal ( mNameLayer, mChannelId , path.c_str ());
  if (!mCurrentSignLay->changeShaderPath (path.c_str ()))
    QMessageBox::warning (this,tr("Init error"),tr("CHADER WRRROOOONGGG"));

}

void SignalLayerWidget::redrawFinal()
{
  mProfiler->start(0);  
  std::wstring path = makeShaderFilePath( std::string("temp") );
  mProcShader->exportToHlsl  ( path.c_str ());
  mProfiler->end(0);

  mProfiler->start(1);
  if (!mFinalMaterial->changeShaderPath (path.c_str ()))
    QMessageBox::warning (this,tr("Init error"),tr("CHADER WRRROOOONGGG"));  
  mProfiler->end(1);


}

std::wstring SignalLayerWidget::makeShaderFilePath ( std::string& aActualSignal )
{

  std::wstring actualSignalWide (aActualSignal.length (), L' ');
  std::copy(aActualSignal.begin (), aActualSignal.end (), actualSignalWide.begin ());

  QString qpath = QCoreApplication::applicationDirPath();
  std::wstring path = qpath.toStdWString();
  path.append(L"/Assets/shaders/");
  path = path + actualSignalWide + L".fx";

  return path;
}

int SignalLayerWidget::getOpToCompositeId()
{
  return mChooseOperation->currentIndex();
}

std::string SignalLayerWidget::getLayerName()
{
  return mNameLayer;
}

void SignalLayerWidget::changeSignalOfLayer(const QString &aSelectedSignal)
{
  if (mSignal != aSelectedSignal.toStdString())
  {
    mProcShader->modifiyLayer( mNameLayer, aSelectedSignal.toStdString() );
    emit forceReloadLayersAndSignals();
  }
}


void SignalLayerWidget::changeWarpSignalOfLayer(const QString &aSelectedWarpSignal)
{
  if (mInhibitSignalChangedSlot)
    return;
  mInhibitSignalChangedSlot = true;
  if ( aSelectedWarpSignal.compare( QString::fromStdString(mWarpSignalName) ))
  {
    mWarpSignalName = aSelectedWarpSignal.toStdString();
    mProcShader->modifySignalWarp( mSignal, mWarpSignalName, mWarpLengthSpin->value(), mWarpCheckBox->isChecked() );
  }
  else
    mChooseSignalWarp->setCurrentIndex( -1 );

  mInhibitSignalChangedSlot = false;

  redrawSignalPreview();
  redrawFinal();
  emit forceReloadSignals();
}

void SignalLayerWidget::signalOpChanged( int aOpId )
{
  if (mInhibitSignalChangedSlot)
    return;

  mCompositeLayerOpId = aOpId;
  if (mCompositeLayerOpName != std::string("none"))
  {
    mProcShader->modifiyLayer( mCompositeLayerOpName , mCompositeLayerOpId, mCompositeLayerOpBlend );
    redrawFinal();
  }

}

void SignalLayerWidget::signalOpBlendChanged( )
{
  if (mInhibitSignalChangedSlot)
    return;

  mCompositeLayerOpBlend =  float(mOpBlendFactorSlider->value()) /
                            float(mOpBlendFactorSlider->maximum());
  if (mCompositeLayerOpName != std::string("none"))
  {
    mProcShader->modifiyLayer( mCompositeLayerOpName , mCompositeLayerOpId, mCompositeLayerOpBlend );
    redrawFinal();
  }

}

float SignalLayerWidget::compositeLayerBlendFactor()
{
  return mCompositeLayerOpBlend;
}

void SignalLayerWidget::signalTypeChanged()
{
  if (mInhibitSignalChangedSlot)
    return;

  mSignalType = mChooseSignalType->currentIndex();


  mProcShader->mSelectedChannelPointer->modifiyLayer ( mNameLayer,
                                                       mSignal );

  mProcShader->mSelectedChannelPointer->setColMap( mNameLayer,
                                                   ColourMap( mColorId, mColor ) );

  mProcShader->modifySignal (mSignal, mSignalType, mInputParams, mNoiseW, mNoiseNoct, mNoiseMoct);

  emit forceReloadLayersAndSignals();

}

void SignalLayerWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
      mDragStartPosition = event->pos();
}

void SignalLayerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - mDragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    if (!mDragging)
    {
      mDragging = true;
      emit setDragSource( mNameLayer, mIsLastLayer  );

      QDrag *dr = new QDrag(this);

      // The data to be transferred by the drag and drop operation is contained in a QMimeData object
      QMimeData *data = new QMimeData;
      /*
      data->setText("This is a test");
      // Assign ownership of the QMimeData object to the QDrag object.
      */
      dr->setMimeData(data);

      // Start the drag and drop operation
      dr->start();

    }
}

void SignalLayerWidget::mouseReleaseEvent(QMouseEvent *event)
{/*
  if (mDragging)
  {
    mDragging = false;
    emit setDragDestination( mNameLayer );
  }*/
}

void SignalLayerWidget::setDragging(bool aValue)
{
  mDragging = aValue;
}

void SignalLayerWidget::dropEvent(QDropEvent *de)
{
  if (mDragging)
  {
    mDragging = false;
    emit setDragDestination( mNameLayer, mIsLastLayer );
  }
}

void SignalLayerWidget::dragMoveEvent(QDragMoveEvent *de)
{
  de->accept();
}

void SignalLayerWidget::dragEnterEvent(QDragEnterEvent *event)
{
  event->acceptProposedAction();
}

D3DXVECTOR3 SignalLayerWidget::getDifferentValue(float v1, float v2, float v3)
{
  float different;
  if (v1 == v2)
    different = v3;
  if (v2 == v3)
    different = v1;
  else
    different = v2;
  return D3DXVECTOR3( different, different, different );
}


void SignalLayerWidget::profileRedrawFinal( std::string& tempName, ProfilerDp* prof  )
{
  std::wstring path = makeShaderFilePath( tempName );
  prof->start(0);
  if (!mFinalMaterial->changeShaderPath (path.c_str ()))
    QMessageBox::warning (this,tr("Init error"),tr("CHADER WRRROOOONGGG"));
  prof->end(0);
}

//TODO
void SignalLayerWidget::signalToBeDeleted()
{
  if ( mIsLastLayer && !mIsAlone )
  {
    emit forceDeleteListLayerAndSignal();
  }
}

void SignalLayerWidget::signalToBeReinitialized()
{
  mProcShader->modifySignaltoDefault( mSignal );
  emit forceReloadSignals();
}

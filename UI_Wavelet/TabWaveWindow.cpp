#include "TabWaveWindow.h"
#include "Noise/haarNoise.cpp"

TabWaveWindow::TabWaveWindow(QWidget *parent):
  QFrame(parent)
{
  mMainLayout = new QVBoxLayout (this);
  mMainLayout->setAlignment( Qt::AlignTop );
  setLayout(mMainLayout);
  isVisible();

  mControlLayout = new QHBoxLayout (this);
  mInputLayout = new QHBoxLayout (this);
  mOutputLayout = new QHBoxLayout (this);

  mControlLayout->setAlignment( Qt::AlignLeft );
  mInputLayout->setAlignment( Qt::AlignLeft );
  mOutputLayout->setAlignment( Qt::AlignLeft );

  mMainLayout->addLayout( mControlLayout );
  mMainLayout->addLayout( mInputLayout );
  mMainLayout->addLayout( mOutputLayout );

  mNumBits = 31;

  mInSlider.resize(mNumBits);
  mOutSlider.resize(mNumBits);

  for ( int i = 0; i < mNumBits; ++i )
  {
    mInSlider[i] = new QSlider( this );
    mInSlider[i]->setRange ( 0, 1 );
    mInSlider[i]->setSliderPosition( 0 );
    mInSlider[i]->setSingleStep( 1 );

    mOutSlider[i] = new QSlider( this );
    mOutSlider[i]->setRange ( 0, 100 );
    mOutSlider[i]->setSliderPosition( 0 );
    mOutSlider[i]->setSingleStep( 1 );

    mInputLayout->addWidget(mInSlider[i]);
    mOutputLayout->addWidget(mOutSlider[i]);

    connect( mInSlider[i], SIGNAL(valueChanged(int)), this, SLOT(inputChanged()));
  }

  mInputSpin = new QSpinBox(this);
  mOutputSpin  = new QSpinBox(this);
  mControlLayout->addWidget(mInputSpin);
  mControlLayout->addWidget(mOutputSpin);

  connect( mInputSpin, SIGNAL(valueChanged(int)), this, SLOT(inputIntChanged()));

  inputChanged();

  mInhibitInputSliderSignal = false;
}

TabWaveWindow::~TabWaveWindow()
{

}

void TabWaveWindow::inputChanged()
{
  if ( mInhibitInputSliderSignal )
    return;

  readInputSignal();
  recomputeSignal();
}

void TabWaveWindow::inputIntChanged()
{
  if ( mInhibitInputSliderSignal )
    return;

  mInhibitInputSliderSignal = true;
  mInput = mInputSpin->value();
  int value = mInput;
  for ( int i = 0; i < mNumBits; ++i )
  {
    mInSlider[i]->setValue( value & 1 ) ;
    value >>= 1;
  }
  mInhibitInputSliderSignal = false;
  recomputeSignal();
}


void TabWaveWindow::readInputSignal()
{
  if ( mInhibitInputSliderSignal )
    return;

  mInhibitInputSliderSignal = true;
  mInput = 0;
  for ( int i = mNumBits - 1; i >= 0; --i )
  {
    mInput <<= 1;
    mInput += mInSlider[i]->value();
  }

  mInputSpin->setValue(mInput);
  mInhibitInputSliderSignal = false;
}

void TabWaveWindow::recomputeSignal()
{
  int   numBits = mNumBits;
  std::vector<float> signal (numBits + 1);
  std::vector<float> signalReconstructed(numBits + 1);

  signal[31] = 0;

  int value = mInput;
  for ( int i = numBits-1 ; i >= 0 ; --i )
  {
    signal[i] = value & 1;
    value = value >> 1;
  }

  bool useHaar = false;
  if (useHaar)
    Haar::haar1Du<float> ( signal, signalReconstructed );
  else
  {
    Daubechies daub;
    daub.invDaubTrans( &signal[0], numBits + 1);
    for ( int i = 0; i < numBits; ++i )
        signalReconstructed[i] = signal[i];
  }

  float retValue = 0;
  for ( int i = numBits - 1; i >= 0; --i )
  {

    retValue += fabs(signalReconstructed[numBits - i]);

    int fvalue = fabs(signalReconstructed[numBits - i]) * 100.0f;

    mOutSlider[i]->setValue( fvalue );
  }

  mOutput = retValue;

  mOutputSpin->setValue( mOutput );
}



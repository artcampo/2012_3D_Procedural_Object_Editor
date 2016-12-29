#include "ChannelSpecWidget.hpp"

ChannelSpecWidget::ChannelSpecWidget(QWidget *parent, int aChannelId, QString& aName) :
    QWidget(parent)
{

  mChannelId = aChannelId;

  mLayout = new QHBoxLayout(this);
  mLayout->setAlignment( Qt::AlignTop );
  setLayout(mLayout);


  mNameButton = new ChannelNameButton( this, aName );
  mColorPicker = new SingleColourWidget( this, &D3DXCOLOR(1,1,1,1) );
  mPercSpin    = new QDoubleSpinBox(this);

  mLayout->addWidget( mNameButton );
  mLayout->addWidget( mColorPicker );
  mLayout->addWidget( mPercSpin );

  mInhibitSignalChangedSlot = true;

  connect( mPercSpin,     SIGNAL(valueChanged(int)), this, SLOT(signalChanged()));
  connect( mColorPicker,  SIGNAL(changed()), this, SLOT(signalChanged()));


  mInhibitSignalChangedSlot = false;
}


void ChannelSpecWidget::changeName(QString &aName)
{
  mNameButton->setText( aName );
}



void ChannelSpecWidget::signalChanged()
{
  if (mInhibitSignalChangedSlot)
    return;

  mSpecPercentage = mPercSpin->value();

  mSpecColour.r = mColorPicker->mStoredColour->r;
  mSpecColour.g = mColorPicker->mStoredColour->g;
  mSpecColour.b = mColorPicker->mStoredColour->b;

  mInhibitSignalChangedSlot = true;
  emit changed();
}


#include "ChannelWidget.hpp"

ChannelWidget::ChannelWidget(QWidget *parent, int aChannelId, QString& aName, const float aDepth) :
    QWidget(parent)
{

  mChannelId = aChannelId;

  mLayout = new QHBoxLayout(this);
  mLayout->setAlignment( Qt::AlignTop );
  setLayout(mLayout);

  mDepthLabel = new QLabel( QString("Depth: "), this);

  mDepthSpin  = new QDoubleSpinBox(this);
  mDepthSpin->setRange( 5, 200 );
  mDepthSpin->setValue( aDepth );

  mSaveButton = new QPushButton(this);
  mSaveButton->setText( QString ("Save"));
  QObject::connect(mSaveButton, SIGNAL(clicked()), this, SLOT(save()));
  mSaveButton->setFixedSize( 36, 27);

  mLoadButton = new QPushButton(this);
  mLoadButton->setText( QString ("Load"));
  QObject::connect(mLoadButton, SIGNAL(clicked()), this, SLOT(load()));
  mLoadButton->setFixedSize( 36, 27);

  mNameButton = new ChannelNameButton( this, aName );

  mLayout->addWidget( mNameButton );  
  mLayout->addWidget( mDepthLabel );
  mLayout->addWidget( mDepthSpin );
  mLayout->addWidget( mSaveButton );
  mLayout->addWidget( mLoadButton );

}

// TODO: or to the parent?
void ChannelWidget::save()
{
  emit channelSelectedForSaving( mNameButton->text() );
}

void ChannelWidget::load()
{
  emit channelAskList();

  LoadChannelDialog* d = new LoadChannelDialog ( this, mList );

  int ret = d->exec();
  if (ret == QDialog::Accepted )
  {
    QString selected = d->selected();
    emit channelSelectedForLoading( selected );
  }
}

void ChannelWidget::changeName(QString &aName)
{
  mNameButton->setText( aName );
}

void ChannelWidget::changeDepth( const float aDepth )
{
  mDepthSpin->setValue( aDepth );
}



void ChannelWidget::getListChannels(std::vector<QString> aList)
{
  mList = aList;
}




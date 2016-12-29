#include "opextrudewidget.h"

OpExtrudeWidget::OpExtrudeWidget(QWidget *parent) :
  QFrame(parent)
{
  mLayout = new QVBoxLayout( this );
  setLayout( mLayout );

  mLayout->setAlignment( Qt::AlignTop );

  mTab = new QTabWidget(this);

  mCombo = new QComboBox(this);
  mCombo->addItem( QString::fromUtf8("Normal") );
  mCombo->addItem( QString::fromUtf8("Inner") );
  mCombo->addItem( QString::fromUtf8("Matrix") );
  mCombo->setFixedWidth( mCombo->sizeHint().width() );

  mLayout->addWidget(mCombo );
  mLayout->addWidget(mTab);

  mExtTab[0] = new QWidget( mTab );
  mExtTab[1] = new QWidget( mTab );
  mExtTab[2] = new QWidget( mTab );

  mTLay[0] = new QVBoxLayout( this );
  mTLay[1] = new QVBoxLayout( this );
  mTLay[2] = new QVBoxLayout( this );

  mTLay[0]->setAlignment( Qt::AlignTop );
  mTLay[1]->setAlignment( Qt::AlignTop );
  mTLay[2]->setAlignment( Qt::AlignTop );

  mExtTab[0]->setLayout( mTLay[0] );
  mExtTab[1]->setLayout( mTLay[1] );
  mExtTab[2]->setLayout( mTLay[2] );

  mPartsOSpin = new QSpinBox();
  mPartsISpin = new QSpinBox();
  mPartsMSpin = new QSpinBox();

  mPartsOSpin->setSingleStep( 1 );
  mPartsOSpin->setRange ( 1, 255 );

  mPartsISpin->setSingleStep( 1 );
  mPartsISpin->setRange ( 1, 255 );

  mPartsMSpin->setSingleStep( 1 );
  mPartsMSpin->setRange ( 1, 255 );


  mPartsLabel[0] = new QLabel( QString("Number of parts:"));
  mPartsLabel[1] = new QLabel( QString("Number of parts:"));
  mPartsLabel[2] = new QLabel( QString("Number of parts:"));

  mTLay[0]->addWidget( mPartsLabel[0] );
  mTLay[1]->addWidget( mPartsLabel[1] );
  mTLay[2]->addWidget( mPartsLabel[2] );

  mTLay[0]->addWidget( mPartsOSpin );
  mTLay[1]->addWidget( mPartsISpin );
  mTLay[2]->addWidget( mPartsMSpin );


  connect( mCombo, SIGNAL(currentIndexChanged ( int )),
           this, SLOT(changeType( int )));
  /*
  mTab->addTab (mIn, QString::fromUtf8("Inner"));
  mTab->addTab (mOf, QString::fromUtf8("Offset"));
  mTab->addTab (mMa, QString::fromUtf8("Matrix"));  */

  mExtMode = 0;

  mTab->addTab (mExtTab[0], QString::fromUtf8("Options"));
  mTab->addTab (mExtTab[1], QString::fromUtf8("Options"));
  mTab->addTab (mExtTab[2], QString::fromUtf8("Options"));
  mTab->removeTab( 0 );
  mTab->removeTab( 0 );
  mTab->removeTab( 0 );

  mTab->addTab (mExtTab[0], QString::fromUtf8("Options"));
}

void OpExtrudeWidget::changeType(int index)
{
  removeAllTabs();
  mExtMode = index;
  mTab->addTab (mExtTab[mExtMode], QString::fromUtf8("Options"));

  emit extrudeTypeChanged(index);
}

void OpExtrudeWidget::removeAllTabs()
{
  mTab->removeTab( 0 );
}


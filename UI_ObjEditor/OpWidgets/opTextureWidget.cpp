#include "opTextureWidget.h"

OpTextureWidget ::OpTextureWidget ( ObjectEditor* aEditor, QWidget *parent) :
    QFrame(parent)
{

  mInhibitSignal = false;
  mEditor = aEditor;
  mLayout = new QVBoxLayout( this );
  setLayout( mLayout );

  mLayout->setAlignment( Qt::AlignTop );
  setFrameStyle(QFrame::Box | QFrame::Sunken);


  mEditionStarted = mEditor->hasProcTex();
  mSelectedTexture = 0;

  if (!mEditionStarted)
    initNoTex();
  else
    initTex();

}

void OpTextureWidget::initTextureEdition()
{
  mLayout->removeWidget(mInitButton);
  mInitButton->deleteLater();

  mEditor->initTexturing();
  initTex();

  while (! _selectFileName() ) ;
}

void OpTextureWidget::initNoTex()
{
  mInitButton = new QPushButton();
  mInitButton->setText( QString("Init edition"));
  connect(mInitButton, SIGNAL(clicked()),this, SLOT(initTextureEdition()));

  mLayout->addWidget(mInitButton);
}

void OpTextureWidget::initTex()
{
  std::vector<ObjEd::ProcTex> t = mEditor->procTex();
  mProcTex = t;

  mCombo = new QComboBox(this);

/*
  mInnerFrame = new QFrame( );
  mInnerFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);*/
  mInnerLayout = new QVBoxLayout (  );
  mTopLayout = new QHBoxLayout();

  mScaleSpin = new QDoubleSpinBox( );
  mScaleSpin->setSingleStep( 0.1 );
  mScaleSpin->setRange ( 0, 5 );
  mNameLabel = new QLabel( );

  mAddButton = new QPushButton();
  mAddButton->setText( QString("Add"));

  mSelectFileButton = new QPushButton();
  mSelectFileButton->setText( QString("Select Shader"));

  mInnerLayout->addWidget( mNameLabel );
  mInnerLayout->addWidget( mSelectFileButton );
  mInnerLayout->addWidget( mScaleSpin );

  mTopLayout->addWidget( mCombo );
  mTopLayout->addWidget( mAddButton );
  mLayout->addLayout( mTopLayout );
  mLayout->addLayout( mInnerLayout );

  connect( mCombo, SIGNAL(currentIndexChanged ( int )),
           this, SLOT(changeSelectedTex( int )));
  connect( mScaleSpin, SIGNAL(valueChanged(double)), this, SLOT(texChanged()));

  connect(mAddButton, SIGNAL(clicked()),this, SLOT(addTexture()));

  connect(mSelectFileButton, SIGNAL(clicked()),this, SLOT(selectFileName()));



  loadTextures( 0 );
  mSelectedTexture = 0;
  mEditor->setCurrentTex( mSelectedTexture );
}


void OpTextureWidget::changeSelectedTex(int index)
{
  if (!mInhibitSignal)
  {
    mSelectedTexture = index;
    mScaleSpin->setValue( mProcTex[mSelectedTexture].scale );
    mNameLabel->setText(  QString::fromStdString( mProcTex[mSelectedTexture].name ) );
    mEditor->setCurrentTex( mSelectedTexture );
  }
}

void OpTextureWidget::texChanged()
{  
  if (!mInhibitSignal)
  {
    mProcTex[ mSelectedTexture ].scale =  mScaleSpin->value();
    mEditor->setProcTex( mProcTex );
  }
}

void OpTextureWidget::addTexture()
{
  mEditor->newProcTex();

  std::vector<ObjEd::ProcTex> t = mEditor->procTex();
  mProcTex = t;

  loadTextures( t.size() - 1 );
}

void OpTextureWidget::loadTextures( int newIndex )
{
  mInhibitSignal = true;
  mCombo->clear();


  for (int i = 0; i < mProcTex.size(); ++i )
  {
    mCombo->addItem( QString::fromStdString( mProcTex[i].name) );
  }
  mCombo->setFixedWidth( mCombo->sizeHint().width() );

  changeSelectedTex( 0 );
  mInhibitSignal = false;
}

void OpTextureWidget::selectFileName()
{
  _selectFileName();
}

bool OpTextureWidget::_selectFileName()
{
  QString path( QFileDialog::getOpenFileName(this, tr("Open File"), "./Assets/Shaders/",
                                             tr("Dx-fx Hlsl 4 (*.fx)")) );
  if ( ! path.isNull() )
  {
    QFileInfo pathInfo( path );
    QString fileName( pathInfo.fileName() );

    mInhibitSignal = true;
    mProcTex[ mSelectedTexture ].name = fileName.toStdString();

    wcscpy_s ( mProcTex[ mSelectedTexture ].mPath, (path.toStdWString()).c_str() );

    mNameLabel->setText( fileName );
    mInhibitSignal = false;
    texChanged();
    mInhibitSignal = true;
    loadTextures( mSelectedTexture );
    mInhibitSignal = false;

  }
  return (!path.isNull());
}

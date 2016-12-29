#include "TabBezierWindow.h"


TabBezierWindow::TabBezierWindow(QWidget *parent):
  QWidget(parent)
{
  installEventFilter(this);

  mObjectTree = new ObjectTree( GlobalEditor::gEditor );

  mGridMultiplier = 1.0f;
  mBackgroundImagePresent = false;
  mBackgroundImageAlpha = 0.4f;
  mBackgroundImageSize = 1.0f;
  mSizeSliderMax  = 300.0f;

  mTargetHeight = 590;
  mTargetWidth  = 314;
  mLayout = new QGridLayout (this);
  setLayout(mLayout);
  mButtonAndTreeLayout = new QGridLayout ();
  mButtonLayout = new QGridLayout ();
  //mButtonLayout->set
  //mButtonLayout->SetFixedSize();
        //36*2 + 4, 27*2 + 4 );

  int startX = 300;
  int startY = 300;

  mViewX      = new QD3DWidgetBezier(this, startX, startY, ViewManager::ViewX());
  mViewY      = new QD3DWidgetBezier(this, startX, startY, ViewManager::ViewY());
  mViewZ      = new QD3DWidgetBezier(this, startX, startY, ViewManager::ViewZ());
  mViewFree   = new QD3DWidgetBezier(this, startX, startY, ViewManager::ViewFree());

  mViewX->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewY->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewZ->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewFree->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);

  bool success = true;
  success &= mViewX->init();
  success &= mViewY->init();
  success &= mViewZ->init();
  success &= mViewFree->init();

  if (!success)
  {
    QMessageBox::warning (
      0,
      QString("Dx init error"),
      QString("Dx init error"));

  }

  mLayout->addWidget( mViewX, 0, 0, 0);
  mLayout->addWidget( mViewY, 1, 0, 0);
  mLayout->addWidget( mViewZ, 0, 1, 0);
  mLayout->addWidget( mViewFree, 1, 1, 0);
  mLayout->addLayout( mButtonAndTreeLayout, 0, 2, 0);

  mButtonImage = new QPushButton;
  mButtonImage->setIcon(QIcon(".//Assets//img//viewportbackground.png"));
  mButtonImage->setIconSize( QSize(30, 23) );
  //mButtonImage->setFixedSize( 36, 27);

  mButtonImageMute = new QPushButton;
  mButtonImageMute->setIcon(QIcon(".//Assets//img//erasebackground.png"));
  mButtonImageMute->setIconSize( QSize(30, 23) );
  //mButtonImageMute->setFixedSize( 36, 27);


  mButtonModeSelect = new QPushButton;
  mButtonModeSelect->setIcon(QIcon(".//Assets//img//pointselection.png"));
  mButtonModeSelect->setIconSize( QSize(30, 23) );
 // mButtonModeSelect->setFixedSize( 36, 27);

  mButtonModeCreate = new QPushButton;
  mButtonModeCreate->setIcon(QIcon(".//Assets//img//addpoint.png"));
  mButtonModeCreate->setIconSize( QSize(30, 23) );
  //mButtonModeCreate->setFixedSize( 36, 27);

  mButtonModeInsert = new QPushButton;
  mButtonModeInsert->setIcon(QIcon(".//Assets//img//insertpointinspline.png"));
  mButtonModeInsert->setIconSize( QSize(30, 23) );
  //mButtonModeInsert->setFixedSize( 36, 27);

  mButtonModeMoveSpline = new QPushButton;
  mButtonModeMoveSpline->setIcon(QIcon(".//Assets//img//splineselection.png"));
  mButtonModeMoveSpline->setIconSize( QSize(30, 23) );
  //mButtonModeMoveSpline->setFixedSize( 36, 27);

  /*
  QSizePolicy::setHorizontalPolicy( QSizePolicy::Minimum );
  QSizePolicy::setVerticalPolicy( QSizePolicy::Minimum );
  */
  QObject::connect(mButtonImage, SIGNAL(clicked()), this, SLOT(loadImage()));

  QObject::connect(mButtonImageMute, SIGNAL(clicked()), this, SLOT(muteImage()));
  QObject::connect(mButtonModeSelect, SIGNAL(clicked()), this, SLOT(splineModeSelect()));
  QObject::connect(mButtonModeMoveSpline, SIGNAL(clicked()), this, SLOT(splineModeMove()));
  QObject::connect(mButtonModeCreate, SIGNAL(clicked()), this, SLOT(splineModeCreate()));
  QObject::connect(mButtonModeInsert, SIGNAL(clicked()), this, SLOT(splineModeInsert()));

  /////////////////////////////////////////////////
  //  Sliders
  mBackgroundSizeSlider  = new QSlider( Qt::Horizontal );
  mBackgroundAlphaSlider = new QSlider( Qt::Horizontal );
  mGridMultiplierSlider  = new QSlider( Qt::Horizontal );

  mBackgroundSizeSlider->setRange( 5, mSizeSliderMax );
  mBackgroundSizeSlider->setValue(100);
  mBackgroundSizeSlider->setTickInterval ( 1 );

  mBackgroundAlphaSlider->setRange( 0, 100 );
  mBackgroundAlphaSlider->setValue(40);
  mBackgroundAlphaSlider->setTickInterval ( 1 );

  mGridMultiplierSlider->setRange( 1, 200 );
  mGridMultiplierSlider->setValue(100);
  mGridMultiplierSlider->setTickInterval ( 1 );
/*
  connect(this, SIGNAL(valueChanged(int)),
                    mBackgroundSizeSlider, SLOT(setValueBackgroundSize(int)));
  connect(this, SIGNAL(valueChanged(int)),
                    mBackgroundAlphaSlider, SLOT(setValueBackgroundAlpha(int)));
*/
  connect(mBackgroundSizeSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(setValueBackgroundSize(int)));
  connect(mBackgroundAlphaSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(setValueBackgroundAlpha(int)));
  connect(mGridMultiplierSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(setValueGridMultiplier(int)));

  /////////////////////////////////////////////////
  //  Tree
  mButtonAndTreeLayout->addLayout(mButtonLayout,0,0);
  mButtonAndTreeLayout->addWidget(mBackgroundSizeSlider,1,0);
  mButtonAndTreeLayout->addWidget(mBackgroundAlphaSlider,2,0);
  mButtonAndTreeLayout->addWidget(mGridMultiplierSlider,3,0);
  mButtonAndTreeLayout->addWidget(mObjectTree,4,0);


  mButtonLayout->addWidget(mButtonModeSelect,     0, 0, 0);
  mButtonLayout->addWidget(mButtonModeMoveSpline, 0, 1, 0);
  mButtonLayout->addWidget(mButtonModeCreate,     1, 0, 0);
  mButtonLayout->addWidget(mButtonModeInsert,     1, 1, 0);
  mButtonLayout->addWidget(mButtonImage,          2, 0, 0);
  mButtonLayout->addWidget(mButtonImageMute,      2, 1, 0);



  GlobalEditor::gViewX    = mViewX;
  GlobalEditor::gViewY    = mViewY;
  GlobalEditor::gViewZ    = mViewZ;
  GlobalEditor::gViewF    = mViewFree;
  GlobalEditor::gObjTree  = mObjectTree;

  GlobalEditor::gEditor->init();
  GlobalEditor::gEditor->selectWorkView(mViewX);
}

TabBezierWindow::~TabBezierWindow()
{
  delete mViewX;
  delete mViewY;
  delete mViewZ;
  delete mViewFree;

  delete mObjectTree;
}

void TabBezierWindow::recomputeArea()
{
  mObjectTree->setFixedSize( mObjectsTreePixelWidth, frameGeometry().height() - 30 );

  mTargetHeight = frameGeometry().height() - 30;
  mTargetWidth  = frameGeometry().width() - mObjectsTreePixelWidth  - 30;

  mViewX->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewY->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewZ->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
  mViewFree->setFixedSize(mTargetWidth / 2, mTargetHeight / 2);
}


bool TabBezierWindow::eventFilter(QObject* obj, QEvent* event) {
        if (event->type() == QEvent::Resize)
          recomputeArea();
        return QWidget::eventFilter(obj, event);
}

void TabBezierWindow::loadImage()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
       tr("Bit map (*.bmp)"));

   if (fileName != "") {
     mFileName = fileName;
     mBackgroundImagePresent = true;
     GlobalEditor::gEditor->setBackgroundImageFileName(fileName, mBackgroundImageAlpha, mBackgroundImageSize);
   }
}


void TabBezierWindow::splineModeSelect()
{
  GlobalEditor::gEditor->modeSelect();
}

void TabBezierWindow::splineModeCreate()
{
  GlobalEditor::gEditor->modeCreate();
}

void TabBezierWindow::splineModeInsert()
{
  GlobalEditor::gEditor->modeInsert();
}

void TabBezierWindow::splineModeMove()
{
  GlobalEditor::gEditor->modeMove();
}

void TabBezierWindow::muteImage()
{
  GlobalEditor::gEditor->muteBackgroundImage();
}

void TabBezierWindow::setValueBackgroundSize(int v)
{
  mBackgroundImageSize = float(v)/float(mSizeSliderMax);
  if (mBackgroundImagePresent)
    GlobalEditor::gEditor->setBackgroundImageFileName(mFileName, mBackgroundImageAlpha, mBackgroundImageSize);
}


void TabBezierWindow::setValueBackgroundAlpha(int v)
{
  mBackgroundImageAlpha = float(v)/float(100);
  if (mBackgroundImagePresent)
    GlobalEditor::gEditor->setBackgroundImageFileName(mFileName, mBackgroundImageAlpha, mBackgroundImageSize);
}

void TabBezierWindow::setValueGridMultiplier(int v)
{
  mGridMultiplier = float(v)/float(100);
  GlobalEditor::gEditor->setGridMultiplier( mGridMultiplier );
}

#include "TabObjEditor.h"


TabObjEditor::TabObjEditor(QWidget *parent):
  QWidget(parent)
{
  installEventFilter(this);

  mObjectTree = new ObjectTreeWidget( this, mEditor );

  recomputeArea();

  mEditor = new ObjectEditor();  
  connect(mEditor, SIGNAL(updateViews()),this, SLOT(updateViewsSlot()));

  initConfiguration();

  mLayout               = new QVBoxLayout ();
  mButtonBarLayout      = new QHBoxLayout ();
  mCentralLayout        = new QHBoxLayout ();
  mRenderLayout         = new QGridLayout ();
  mSideLayout           = new QVBoxLayout ();
  mTreeAndOpLayout      = new QVBoxLayout ();

  setLayout(mLayout);
  mLayout->addLayout( mButtonBarLayout );
  mLayout->addLayout( mCentralLayout );
  mCentralLayout->addLayout( mRenderLayout);
  mCentralLayout->addLayout( mSideLayout );

  mSideLayout->addLayout( mTreeAndOpLayout );

  mButtonBarLayout->setAlignment( Qt::AlignLeft );

  mSideLayout->setAlignment( Qt::AlignTop );
  mTreeAndOpLayout->setAlignment( Qt::AlignTop );
  mTreeAndOpLayout->addWidget(mObjectTree);


  initRenderWidgets();
  initButtonBar();
  initOpWidgets();
  initQtSlotsForOperations();

  initOptions();
  updateOptions();

  //mSideLayout->addWidget(mOperationView);



  updateViews();
}

TabObjEditor::~TabObjEditor()
{
  delete mEditor;
  delete mSingleWindow;
  /*
  delete mMultiWindow[0];
  delete mMultiWindow[1];
  delete mMultiWindow[2];
  delete mMultiWindow[3];
  */
}

void TabObjEditor::recomputeArea()
{
  QDesktopWidget *desktop = QApplication::desktop();
  int screenWidth  = desktop->width();
  int screenHeight = desktop->height();  

  mTargetHeight = screenHeight - 200;
  mTargetWidth  = screenWidth - mObjectsTreePixelWidth  - 40 - 100;
}


bool TabObjEditor::eventFilter(QObject* obj, QEvent* event) {
        if (event->type() == QEvent::Resize)
          recomputeArea();
        return QWidget::eventFilter(obj, event);
}

void TabObjEditor::loadImage()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
       tr("Bit map (*.bmp)"));

   if (fileName != "") {
     mFileName = fileName;
     mBackgroundImagePresent = true;
     mEditor->setBackgroundImageFileName(fileName, mBackgroundImageAlpha, mBackgroundImageSize);
   }
}


void TabObjEditor::muteImage()
{
  mEditor->muteBackgroundImage();
}

void TabObjEditor::setValueBackgroundSize(int v)
{
  mBackgroundImageSize = float(v)/float(mSizeSliderMax);
  if (mBackgroundImagePresent)
    mEditor->setBackgroundImageFileName(mFileName, mBackgroundImageAlpha, mBackgroundImageSize);
}


void TabObjEditor::setValueBackgroundAlpha(int v)
{
  mBackgroundImageAlpha = float(v)/float(100);
  if (mBackgroundImagePresent)
    mEditor->setBackgroundImageFileName(mFileName, mBackgroundImageAlpha, mBackgroundImageSize);
}

void TabObjEditor::setValueGridMultiplier(int v)
{
  mGridMultiplier = float(v)/float(100);
  mEditor->setGridMultiplier( mGridMultiplier );
}




void TabObjEditor::updateViews()
{

  if (mSingleWindowMode)
  {
    if ( mEditor->hasRenderModel() )
    {
      if ( mEditor->renderMode() == ePhongMode ||
           mEditor->renderMode() == eTransparentMode )
      {

        std::vector<EditableMeshModel*> models;
        models = mEditor->convertObject( mSingleWindow->app->getD3DXdevice(),
                                         mSingleWindow->mView->getCamPos() );

        for ( unsigned int i = 0 ; i < models.size(); ++i )
          mSingleWindow->createModel( models[i], i );

      }
      if ( mEditor->renderMode() == eProcTexMode )
      {
        std::vector<EditableMeshModelProcTex*> models;
        models = mEditor->convertObjectProcTex( mSingleWindow->app->getD3DXdevice(),
                                         mSingleWindow->mView->getCamPos() );

        for ( unsigned int i = 0 ; i < models.size(); ++i )
          mSingleWindow->createModelProcTex( models[i], i );

      }
    }
    mSingleWindow->updateGizmoTModel();
    mSingleWindow->updateGizmoRModel();
    mSingleWindow->updateGizmoSModel();
  }
  else
  {
    for (int i = 0; i < 4; ++i)
    {
      if ( mEditor->hasRenderModel() )
      {
        if ( mEditor->renderMode() == ePhongMode ||
             mEditor->renderMode() == eTransparentMode )
        {
          std::vector<EditableMeshModel*> models;
          models = mEditor->convertObject( mMultiWindow[i]->app->getD3DXdevice(),
                                           mMultiWindow[i]->mView->getCamPos() );

          for ( unsigned int i = 0 ; i < models.size(); ++i )
            mMultiWindow[i]->createModel( models[i], i );
        }

        if ( mEditor->renderMode() == eProcTexMode )
        {
          std::vector<EditableMeshModelProcTex*> models;
          models = mEditor->convertObjectProcTex( mMultiWindow[i]->app->getD3DXdevice(),
                                           mMultiWindow[i]->mView->getCamPos() );

          for ( unsigned int i = 0 ; i < models.size(); ++i )
            mMultiWindow[i]->createModelProcTex( models[i], i );
        }
      }
      mMultiWindow[i]->updateGizmoTModel();
      mMultiWindow[i]->updateGizmoRModel();
      mMultiWindow[i]->updateGizmoSModel();
    }
  }

}



void TabObjEditor::updateViewsSlot()
{
  updateViews();
}



void TabObjEditor::initConfiguration()
{
  mCurrentWindow = mSingleWindow;
  mGridMultiplier = 1.0f;
  mBackgroundImagePresent = false;
  mBackgroundImageAlpha = 0.4f;
  mBackgroundImageSize = 1.0f;
  mSizeSliderMax  = 300.0f;
}


void TabObjEditor::initButtonBar()
{
  mSwapViewsButton = new QPushButton;
  mRenderChangeButton = new QPushButton;
  mModeOButton = new QPushButton;
  mModeVButton = new QPushButton;
  mModeEButton = new QPushButton;
  mModeFButton = new QPushButton;

  mCubeButton =  new QPushButton;
  mModeOpRotButton = new QPushButton;
  mModeOpScaButton = new QPushButton;
  mModeOpExtButton = new QPushButton;
  mModeOpBevButton = new QPushButton;
  mModeOpKniButton   = new QPushButton;
  mModeOpNoScButton = new QPushButton;
  mModeOpSelButton= new QPushButton;
  mModeOpTexButton= new QPushButton;

  mModeOButton->setCheckable( true );
  mModeVButton->setCheckable( true );
  mModeEButton->setCheckable( true );
  mModeFButton->setCheckable( true );

  mModeOpTexButton->setCheckable( true );
  mModeOpSelButton->setCheckable( true );
  mModeOpRotButton->setCheckable( true );
  mModeOpScaButton->setCheckable( true );
  mModeOpExtButton->setCheckable( true );
  mModeOpBevButton->setCheckable( true );
  mModeOpKniButton->setCheckable( true );
  mModeOpNoScButton->setCheckable( true );



  mSwapViewsButton->setIconSize( QSize(30, 23) );
  mRenderChangeButton->setIconSize( QSize(30, 23) );
  mModeOButton->setIconSize( QSize(30, 23) );
  mSwapViewsButton->setIconSize( QSize(30, 23) );
  mModeEButton->setIconSize( QSize(30, 23) );
  mModeFButton->setIconSize( QSize(30, 23) );
  mCubeButton->setIconSize( QSize(30, 23) );
  mModeOpRotButton->setIconSize( QSize(30, 23) );
  mModeOpScaButton->setIconSize( QSize(30, 23) );
  mModeOpExtButton->setIconSize( QSize(30, 23) );
  mModeOpBevButton->setIconSize( QSize(30, 23) );
  mModeOpKniButton->setIconSize( QSize(30, 23) );
  mModeOpNoScButton->setIconSize( QSize(30, 23) );
  mModeOpSelButton->setIconSize( QSize(30, 23) );
  mModeOpTexButton->setIconSize( QSize(30, 23) );

  mSwapViewsButton->setText( QString("Swap"));
  mRenderChangeButton->setText( QString("Render"));
  mModeOButton->setText( QString("O"));
  mModeVButton->setText( QString("V"));
  mModeEButton->setText( QString("E"));
  mModeFButton->setText( QString("F"));  
  mCubeButton->setText( QString("Cube"));
  mModeOpRotButton->setText( QString("Rot"));
  mModeOpScaButton->setText( QString("Sca"));
  mModeOpExtButton->setText( QString("Ext"));
  mModeOpBevButton->setText( QString("Bev"));
  mModeOpKniButton->setText( QString("Kni"));
  mModeOpNoScButton->setText( QString("nSc"));
  mModeOpSelButton->setText( QString("Sel"));
  mModeOpTexButton->setText( QString("Tex"));


/*
  mSwapViewsButton->setIcon(QIcon(".//Assets//img//addpoint.png"));
  */

  mSwapViewsButton->setFixedSize( 36, 27);
  mRenderChangeButton->setFixedSize( 36, 27);
  mCubeButton->setFixedSize( 36, 27);
  mModeOButton->setFixedSize( 36, 27);
  mModeVButton->setFixedSize( 36, 27);
  mModeEButton->setFixedSize( 36, 27);
  mModeFButton->setFixedSize( 36, 27);
  mModeOpSelButton->setFixedSize( 36, 27);
  mModeOpRotButton->setFixedSize( 36, 27);
  mModeOpScaButton->setFixedSize( 36, 27);
  mModeOpExtButton->setFixedSize( 36, 27);
  mModeOpBevButton->setFixedSize( 36, 27);
  mModeOpKniButton->setFixedSize( 36, 27);
  mModeOpNoScButton->setFixedSize( 36, 27);
  mModeOpTexButton->setFixedSize( 36, 27);

  mBut1Split = new QSplitter();
  mBut2Split = new QSplitter();
  mBut3Split = new QSplitter();

  mButtonBarLayout->addWidget( mSwapViewsButton );
  mButtonBarLayout->addWidget( mRenderChangeButton );
  mButtonBarLayout->addWidget( mBut1Split );
  mButtonBarLayout->addWidget( mModeOButton );
  mButtonBarLayout->addWidget( mModeVButton );
  mButtonBarLayout->addWidget( mModeEButton );
  mButtonBarLayout->addWidget( mModeFButton );  
  mButtonBarLayout->addWidget( mBut2Split );
  mButtonBarLayout->addWidget( mCubeButton );
  mButtonBarLayout->addWidget( mBut3Split );
  mButtonBarLayout->addWidget( mModeOpSelButton );
  mButtonBarLayout->addWidget( mModeOpRotButton );
  mButtonBarLayout->addWidget( mModeOpScaButton );
  mButtonBarLayout->addWidget( mModeOpExtButton );
  mButtonBarLayout->addWidget( mModeOpBevButton );
  mButtonBarLayout->addWidget( mModeOpKniButton );
  mButtonBarLayout->addWidget( mModeOpNoScButton );
  mButtonBarLayout->addWidget( mModeOpTexButton );

  connect(mSwapViewsButton, SIGNAL(clicked()),this, SLOT(swapViews()));
  connect(mRenderChangeButton, SIGNAL(clicked()),this, SLOT(changeRender()));
  connect(mCubeButton, SIGNAL(clicked()),this, SLOT(cubeButtonPressed()));
  connect(mModeOButton, SIGNAL(clicked()),this, SLOT(setModeO()));
  connect(mModeVButton, SIGNAL(clicked()),this, SLOT(setModeV()));
  connect(mModeEButton, SIGNAL(clicked()),this, SLOT(setModeE()));
  connect(mModeFButton, SIGNAL(clicked()),this, SLOT(setModeF()));
  connect(mModeOpRotButton, SIGNAL(clicked()),this, SLOT(setModeOpRot()));
  connect(mModeOpScaButton, SIGNAL(clicked()),this, SLOT(setModeOpSca()));
  connect(mModeOpExtButton, SIGNAL(clicked()),this, SLOT(setModeOpExt()));
  connect(mModeOpBevButton, SIGNAL(clicked()),this, SLOT(setModeOpBev()));
  connect(mModeOpKniButton, SIGNAL(clicked()),this, SLOT(setModeOpKni()));
  connect(mModeOpNoScButton, SIGNAL(clicked()),this, SLOT(setModeOpNoSc()));
  connect(mModeOpSelButton, SIGNAL(clicked()),this, SLOT(setModeOpSel()));
  connect(mModeOpTexButton, SIGNAL(clicked()),this, SLOT(setModeOpTex()));



}

void TabObjEditor::setModeO( bool aUpdateViews )
{
  downModeButtons();
  mModeOButton->setChecked( true );
  mEditor->setMode( ObjEd::eModeObject );
  mCurrentWindow->setMode( ObjEd::eModeObject );
  if (aUpdateViews)
    updateViews();
}

void TabObjEditor::setModeV(bool aUpdateViews )
{
  downModeButtons();
  mModeVButton->setChecked( true );
  mEditor->setMode( ObjEd::eModeVertex );
  mCurrentWindow->setMode( ObjEd::eModeVertex );
  if (aUpdateViews)
    updateViews();
}

void TabObjEditor::setModeE(bool aUpdateViews )
{
  downModeButtons();
  mModeEButton->setChecked( true );
  mEditor->setMode( ObjEd::eModeEdge );
  mCurrentWindow->setMode( ObjEd::eModeEdge );
  if (aUpdateViews)
    updateViews();
}

void TabObjEditor::setModeF(bool aUpdateViews )
{
  downModeButtons();
  mModeFButton->setChecked( true );
  mEditor->setMode( ObjEd::eModeFace );
  mCurrentWindow->setMode( ObjEd::eModeFace );
  if (aUpdateViews)
    updateViews();
}



void TabObjEditor::setModeOpExt()
{
  setModeBeforeKnife();
  hideOpWidgets();
  mOpExtrW->show();
  downModeOpButtons();
  cancelKnifeOperation();

  mModeOpExtButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeExtrude );
  updateViews();
}

void TabObjEditor::setModeOpBev()
{
  setModeBeforeKnife();
  hideOpWidgets();
  downModeOpButtons();
  cancelKnifeOperation();
  mModeOpBevButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeBevel );
  updateViews();
}

void TabObjEditor::setModeOpKni()
{
  setModeBeforeKnife();
  if (!mHasModeBeforeKnife)
  {
    mKnifeModeBefore = mEditor->mode();
    mHasModeBeforeKnife = true;
  }
  hideOpWidgets();
  mOpKnifeW->show();
  downModeOpButtons();
  mModeOpKniButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeKniveLine );
  updateViews();
}

void TabObjEditor::setModeOpNoSc()
{
  setModeBeforeKnife();
  hideOpWidgets();
  downModeOpButtons();
  cancelKnifeOperation();

  mModeOpNoScButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeNormalScale );
  updateViews();
}

void TabObjEditor::setModeOpSel()
{
  setModeBeforeKnife();
  hideOpWidgets();
  downModeOpButtons();
  cancelKnifeOperation();

  mModeOpSelButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeSelection );
  updateViews();
}

void TabObjEditor::setModeOpTex()
{
  setModeBeforeKnife();
  hideOpWidgets();
  downModeOpButtons();
  cancelKnifeOperation();
  mOpTex->show();

  mModeOpTexButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeTexture );
  updateViews();
}

void TabObjEditor::downModeButtons()
{
  mModeOButton->setChecked( false );
  mModeVButton->setChecked( false );
  mModeEButton->setChecked( false );
  mModeFButton->setChecked( false );
}

void TabObjEditor::downModeOpButtons()
{
  mModeOpSelButton->setChecked( false );
  mModeOpRotButton->setChecked( false );
  mModeOpScaButton->setChecked( false );
  mModeOpExtButton->setChecked( false );
  mModeOpBevButton->setChecked( false );
  mModeOpKniButton->setChecked( false );
  mModeOpNoScButton->setChecked( false );
  mModeOpTexButton->setChecked( false );
}

void TabObjEditor::setModeOpRot()
{
  setModeBeforeKnife();
  downModeOpButtons();
  cancelKnifeOperation();
  mModeOpRotButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeRotation );
  updateViews();
}

void TabObjEditor::setModeOpSca()
{
  setModeBeforeKnife();
  downModeOpButtons();
  cancelKnifeOperation();
  mModeOpScaButton->setChecked( true );
  mEditor->setModeOp( ObjEd::eOpModeScale );
  updateViews();
}

void TabObjEditor::initEdition()
{
  mEditor->selectWorkView( mSingleWindow );
  mEditor->newFile();
  setModeF();
  setModeOpSel();
  mHasModeBeforeKnife = false;
}

void TabObjEditor::initOpWidgets()
{
  mOpKnifeW = new OpKnifeWidget( this);
  mOpExtrW  = new OpExtrudeWidget( this ) ;
  mOpTex    = new OpTextureWidget ( mEditor, this );


  mTreeAndOpLayout->addWidget( mOpKnifeW );
  mTreeAndOpLayout->addWidget( mOpExtrW );
  mTreeAndOpLayout->addWidget( mOpTex );



  hideOpWidgets();

  connect(mOpKnifeW->mAtHalfCheckBox, SIGNAL(stateChanged(int)),
                    this, SLOT(widOpAtHalf(int)));
  connect(mOpKnifeW->mAt1CheckBox, SIGNAL(stateChanged(int)),
                    this, SLOT(widOpAtHalf12(int)));
  connect(mOpKnifeW->mAt2CheckBox, SIGNAL(stateChanged(int)),
                    this, SLOT(widOpAtHalf12(int)));

  connect( mOpExtrW, SIGNAL(extrudeTypeChanged(int)),
           this, SLOT(extrudeTypeChangedSlot(int)));

}

void TabObjEditor::hideOpWidgets()
{
  mOpKnifeW->hide();
  mOpExtrW->hide();
  mOpTex->hide();
}

void TabObjEditor::widOpAtHalf12(int state)
{
  if( mOpKnifeW->mAt1CheckBox->checkState() == Qt::Checked  )
    mEditor->optionsOpKnifeHalf( (int)1 );
  else if( mOpKnifeW->mAt2CheckBox->checkState() == Qt::Checked  )
    mEditor->optionsOpKnifeHalf( (int)2 );
  else
    mEditor->optionsOpKnifeHalf( (int)0 );
}

void TabObjEditor::widOpAtHalf(int state)
{
  if ( state == Qt::Unchecked )
    mEditor->optionsOpKnifeHalf ( false );
  if ( state == Qt::Checked )
    mEditor->optionsOpKnifeHalf ( true );

  widOpAtHalf12(state);
}

void TabObjEditor::extrudeTypeChangedSlot(int mode)
{
  mEditor->opExtOptions( mode );
}

void TabObjEditor::initQtSlotsForOperations()
{

  connect( mOpExtrW->mPartsOSpin, SIGNAL(valueChanged(int)),
           this, SLOT(npartsEOchanged(int)));
  connect( mOpExtrW->mPartsISpin, SIGNAL(valueChanged(int)),
           this, SLOT(npartsEIchanged(int)));
  connect( mOpExtrW->mPartsMSpin, SIGNAL(valueChanged(int)),
           this, SLOT(npartsEMchanged(int)));
}

void TabObjEditor::npartsEOchanged(int val)
{
  opEO.numParts = val;
  updateOptions();
}

void TabObjEditor::npartsEIchanged(int val)
{
  opEI.numParts = val;
  updateOptions();
}

void TabObjEditor::npartsEMchanged(int val)
{
  opEM.numParts = val;
  updateOptions();
}

void TabObjEditor::initOptions()
{
  opEO.numParts = 1;
  opEI.numParts = 1;
  opEM.numParts = 1;
}

void TabObjEditor::updateOptions()
{
  mEditor->updateOperationOptions( opEO, opEI, opEM );
}

void TabObjEditor::cubeButtonPressed()
{
  mEditor->addNewPrimitive();
  updateViewsSlot();
}

void TabObjEditor::initRenderWidgets()
{

  mSingleWindow = new QD3DWidgetObj( this, mTargetWidth, mTargetHeight, ObjView::ViewFree(), mEditor );
  mSingleWindow->setFixedSize(mTargetWidth, mTargetHeight);
/*
  mMultiWindow[0] = new QD3DWidgetObj( this, mTargetWidth/2, mTargetHeight/2, ObjView::ViewFree(), mEditor );
  mMultiWindow[0]->setFixedSize(mTargetWidth/2, mTargetHeight/2);

  mMultiWindow[1] = new QD3DWidgetObj( this, mTargetWidth/2, mTargetHeight/2, ObjView::ViewFree(), mEditor );
  mMultiWindow[1]->setFixedSize(mTargetWidth/2, mTargetHeight/2);

  mMultiWindow[2] = new QD3DWidgetObj( this, mTargetWidth/2, mTargetHeight/2, ObjView::ViewFree(), mEditor );
  mMultiWindow[2]->setFixedSize(mTargetWidth/2, mTargetHeight/2);

  mMultiWindow[3] = new QD3DWidgetObj( this, mTargetWidth/2, mTargetHeight/2, ObjView::ViewFree(), mEditor );
  mMultiWindow[3]->setFixedSize(mTargetWidth/2, mTargetHeight/2);

  mMultiWindow[0]->hide();
  mMultiWindow[1]->hide();
  mMultiWindow[2]->hide();
  mMultiWindow[3]->hide();
*/
  mRenderLayout->addWidget( mSingleWindow, 0, 0 );
/*
  mRenderLayout->addWidget( mMultiWindow[0], 0, 0 );
  mRenderLayout->addWidget( mMultiWindow[1], 0, 1 );
  mRenderLayout->addWidget( mMultiWindow[2], 1, 0 );
  mRenderLayout->addWidget( mMultiWindow[3], 1, 1 );
*/

  mCurrentWindow = mSingleWindow;

  mSingleWindowMode = true;
}


void TabObjEditor::swapViews()
{
  if ( mSingleWindowMode )
  {

    mSingleWindow->hide();

    mMultiWindow[0]->show();
    mMultiWindow[1]->show();
    mMultiWindow[2]->show();
    mMultiWindow[3]->show();

    mSingleWindowMode = false;
    updateViews();
  }
  else
  {

    mSingleWindow->show();
    mMultiWindow[0]->hide();
    mMultiWindow[1]->hide();
    mMultiWindow[2]->hide();
    mMultiWindow[3]->hide();

    mSingleWindowMode = true;
    updateViews();
  }
}

void TabObjEditor::cancelKnifeOperation()
{
  if (mSingleWindowMode)
    mSingleWindow->cancelKnifeOperation();
  else
  {
    mMultiWindow[0]->cancelKnifeOperation();
    mMultiWindow[1]->cancelKnifeOperation();
    mMultiWindow[2]->cancelKnifeOperation();
    mMultiWindow[3]->cancelKnifeOperation();
  }
}

void TabObjEditor::changeRender()
{
  mEditor->changeRenderMode();
  updateViews();
}

void TabObjEditor::setModeBeforeKnife()
{
  if ( mHasModeBeforeKnife)
  {
    mHasModeBeforeKnife = false;
    switch(mKnifeModeBefore)
    {
      case eModeObject:   setModeO( false ); break;
      case eModeVertex:   setModeV( false ); break;
      case eModeEdge:     setModeE( false ); break;
      case eModeFace:     setModeF( false ); break;
    };
  }
}

void TabObjEditor::reloadTexWidget()
{
  mTreeAndOpLayout->removeWidget( mOpTex );
  delete mOpTex;
  mOpTex    = new OpTextureWidget ( mEditor, this );
  mOpTex->show();
  mTreeAndOpLayout->addWidget( mOpTex );
}

void TabObjEditor::selectionMode()
{
  setModeOpSel();
}

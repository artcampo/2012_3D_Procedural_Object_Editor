#include "TabDirGEditor.h"


TabDirGEditor::TabDirGEditor(QWidget *parent):
  QWidget(parent)
{
  installEventFilter(this);

  recomputeArea();

  mEditor = new DirGraphEditor();
  mEditor->newFile();
  connect(mEditor, SIGNAL(updateViews()),this, SLOT(updateViewsSlot()));
  connect(mEditor, SIGNAL(dirGraphNameChanged(QString)),this, SLOT(graphNameChangedByEditor(QString)));

  initConfiguration();

  mLayout               = new QVBoxLayout ();
  mCentralLayout        = new QVBoxLayout ();
  mButtonLayout         = new QHBoxLayout ();
  mRenderLayout         = new QGridLayout ();

  mGraphName   = new NameButton(this, QString("Name me"));
  connect( mGraphName,  SIGNAL(nameChanged(QString)), this, SLOT(graphNameChanged(QString)));

  mAdjustYButton = new QPushButton( "Adjust y to 0", this);
  connect( mAdjustYButton,  SIGNAL(clicked()), this, SLOT(adjustYCoord()));

  mAdjustToGridButton = new QPushButton( "Adjust all points to grid", this);
  connect( mAdjustToGridButton,  SIGNAL(clicked()), this, SLOT(adjustToGridCoord()));


  setLayout(mLayout);
  mLayout->addLayout( mCentralLayout );
  mButtonLayout->addWidget( mGraphName );
  mButtonLayout->addWidget( mAdjustYButton );
  mButtonLayout->addWidget( mAdjustToGridButton );


  mCentralLayout->addLayout( mButtonLayout);
  mCentralLayout->addLayout( mRenderLayout);

  initRenderWidgets();
  mEditor->selectWorkView(mSingleWindow);
  //updateViews();
}

TabDirGEditor::~TabDirGEditor()
{
  delete mEditor;
  delete mSingleWindow;

}

void TabDirGEditor::recomputeArea()
{
  QDesktopWidget *desktop = QApplication::desktop();
  int screenWidth  = desktop->width();
  int screenHeight = desktop->height();

  mTargetHeight = screenHeight;
  mTargetWidth  = screenWidth;
}


bool TabDirGEditor::eventFilter(QObject* obj, QEvent* event) {
        if (event->type() == QEvent::Resize)
          recomputeArea();
        return QWidget::eventFilter(obj, event);
}




void TabDirGEditor::updateViews()
{
  EditableDirGraph* model;

  model = mEditor->convertModel();
  mSingleWindow->app->setModel( model );

}



void TabDirGEditor::updateViewsSlot()
{
  updateViews();
}



void TabDirGEditor::initConfiguration()
{
  mGridMultiplier = 1.0f;
}



void TabDirGEditor::initRenderWidgets()
{
  mSingleWindow = new QD3DWidgetDirG( this, mTargetWidth, mTargetHeight, ObjViewDG::ViewFree(), mEditor );
  mSingleWindow->setFixedSize(mTargetWidth, mTargetHeight);
  mSingleWindow->init();

  mRenderLayout->addWidget( mSingleWindow, 0, 0 );
}


void TabDirGEditor::graphNameChanged( QString aName )
{
  mEditor->setGraphExportName( aName );
}

void TabDirGEditor::graphNameChangedByEditor( QString aName )
{
  mGraphName->setName( aName );
}

void TabDirGEditor::adjustYCoord()
{
  mEditor->adjustYCoord();
}

void TabDirGEditor::adjustToGridCoord()
{
  mEditor->adjustAllPointsToGrid();
}

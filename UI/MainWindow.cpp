#include "MainWindow.hpp"
//#include "App/dx_misc.hpp"

#include <QColorDialog>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent)
{
  GlobalEditor::gMainWindow = this;
  initPathsForShaders();
  initMenu();

  QIcon icon;
  icon.addFile(
    QString::fromUtf8(":/img/icon"), QSize(), QIcon::Normal, QIcon::Off );
  this->setWindowIcon(icon);

  statusBar()->showMessage(tr("Not gordian is in your pc"));

}

void MainWindow::initAllStuff()
{
  ///////////////////////////////////////////////////////
  //  Data layer elements
  // TODO: this should be moved to its tab

  mEditor = new EditorBezier( );
  GlobalEditor::gEditor = mEditor;

  ///////////////////////////////////////////////////////
  //  Gui elements
  mMainTab = new QTabWidget(this);

  mMeshTab = new TabObjEditor( mMainTab );
  mMainTab->addTab (mMeshTab, QString::fromUtf8("Objects"));
  mMeshTab->initEdition();

  mTextureTab = new TabTextureWindow( mMainTab );
  mMainTab->addTab (mTextureTab, QString::fromUtf8("Material"));

  mSplineslTab = new TabBezierWindow( mMainTab );
  mMainTab->addTab (mSplineslTab, QString::fromUtf8("Splines"));

  mPalTab = new TabPalWindow( mMainTab );
  mMainTab->addTab (mPalTab, QString::fromUtf8("Palette"));

  mWaveletTab = new TabWaveWindow( mMainTab );
  mMainTab->addTab (mWaveletTab, QString::fromUtf8("Haar"));

  mDirGraphTab = new TabDirGEditor( mMainTab );
  mMainTab->addTab (mDirGraphTab, QString::fromUtf8("DirGraph"));






  mMeshTabId = 0;
  mMaterialTabId = 1;
  mSplinesTabId = 2;
  mGraphTabId = 5;

  setCentralWidget( mMainTab );

}

MainWindow::~MainWindow()
{
  delete mMeshTab;

  //TODO: had to comment this out to avoid crash at unload time. why?
  delete mEditor;
  delete mSplineslTab;
  delete mMainTab;

  //delete mDirGraphTab;
}

/**** Init functions ****/

void MainWindow::initMenu()
{
  mFileMenu   = menuBar()->addMenu(tr("&File"));
  mEditMenu   = menuBar()->addMenu(tr("&Edit"));
  mTexMenu    = menuBar()->addMenu(tr("&Textures"));
  mSplineMenu = menuBar()->addMenu(tr("&Splines"));
  mPalMenu    = menuBar()->addMenu(tr("&Palette"));  
  mHelpMenu   = menuBar()->addMenu(tr("&Help"));
  mSplineModeMenu   = new QMenu(tr("&Change mode"), this);
  mSplineBackMenu   = new QMenu(tr("&Background Image"), this);

  ////////////////////////////
  //  Edit
  mFileNewAction = new QAction(tr("&New"), this);
  mFileOpenAction = new QAction(tr("&Open"), this);
  mFileSaveAction = new QAction(tr("&Save"), this);
  mFileExportAction = new QAction(tr("&Export"), this);
  mFileExitAction = new QAction(tr("E&xit"), this);

  connect(mFileNewAction, SIGNAL(triggered()), this, SLOT(newF()));
  connect(mFileOpenAction, SIGNAL(triggered()), this, SLOT(open()));
  connect(mFileSaveAction, SIGNAL(triggered()), this, SLOT(save()));
  connect(mFileExportAction, SIGNAL(triggered()), this, SLOT(fileexport()));
  connect(mFileExitAction, SIGNAL(triggered()), qApp, SLOT(quit()));


  mFileMenu->addAction(mFileNewAction);
  mFileMenu->addSeparator();
  mFileMenu->addAction(mFileOpenAction);
  mFileMenu->addAction(mFileSaveAction);
  mFileMenu->addAction(mFileExportAction);
  mFileMenu->addSeparator();
  mFileMenu->addAction(mFileExitAction);

  ////////////////////////////
  //  Edit
  mEditUndoAction   = new QAction(tr("&Undo"), this);
  mEditRedoAction   = new QAction(tr("&Redo"), this);
  mEditCutAction    = new QAction(tr("Cut"), this);
  mEditCopyAction   = new QAction(tr("&Copy"), this);
  mEditPasteAction  = new QAction(tr("&Paste"), this);

  connect(mEditUndoAction,  SIGNAL(triggered()), this, SLOT(undo()));
  connect(mEditRedoAction,  SIGNAL(triggered()), this, SLOT(redo()));
  connect(mEditCutAction,   SIGNAL(triggered()), this, SLOT(cut()));
  connect(mEditCopyAction,  SIGNAL(triggered()), this, SLOT(copy()));
  connect(mEditPasteAction, SIGNAL(triggered()), this, SLOT(paste()));


  mEditMenu->addAction(mEditUndoAction);
  mEditMenu->addAction(mEditRedoAction);
  mEditMenu->addSeparator();
  mEditMenu->addAction(mEditCutAction);
  mEditMenu->addAction(mEditCopyAction);
  mEditMenu->addAction(mEditPasteAction);

  ////////////////////////////
  //  Splines

  mSplineNew    = new QAction(tr("&New"), this);
  mSplineCopyCurrent  = new QAction(tr("&Copy spline from current"), this);
  mSplineExport = new QAction(tr("&Export to c"), this);
  mSplineExportLast = new QAction(tr("Export &last point to c"), this);
  mSplinetoggleAdjustGrid = new QAction(tr("&Toggle Adjust to grid"), this);
  mSplineCloseAll = new QAction(tr("Close &all splines"), this);


  mSplineModeSelect   = new QAction(tr("&Select/delete points"), this);
  mSplineModeCreate   = new QAction(tr("&Create new points"), this);
  mSplineModeInsert   = new QAction(tr("&Insert new points"), this);
  mSplineModeMove     = new QAction(tr("&Move spline"), this);

  mSplineModeMenu->addAction(mSplineModeSelect);
  mSplineModeMenu->addAction(mSplineModeCreate);
  mSplineModeMenu->addAction(mSplineModeInsert);
  mSplineModeMenu->addAction(mSplineModeMove);


  mSplineBackChoose  = new QAction(tr("&Choose image"), this);
  mSplineBackMute    = new QAction(tr("&Mute"), this);

  mSplineBackMenu->addAction(mSplineBackChoose);
  mSplineBackMenu->addAction(mSplineBackMute);

  mSplineMenu->addAction(mSplineNew);
  mSplineMenu->addAction(mSplineCopyCurrent);
  mSplineMenu->addAction(mSplineCloseAll);
  mSplineMenu->addMenu(mSplineModeMenu);
  mSplineMenu->addSeparator();
  mSplineMenu->addAction(mSplineExport);
  mSplineMenu->addAction(mSplineExportLast);
  mSplineMenu->addSeparator();
  mSplineMenu->addMenu(mSplineBackMenu);
  mSplineMenu->addAction(mSplinetoggleAdjustGrid);

  connect(mSplineNew,  SIGNAL(triggered()), this, SLOT(splineNew()));
  connect(mSplineCopyCurrent,  SIGNAL(triggered()), this, SLOT(splineCopyCurrent()));
  connect(mSplineModeSelect,  SIGNAL(triggered()), this, SLOT(splineModeSelect()));
  connect(mSplineModeCreate,  SIGNAL(triggered()), this, SLOT(splineModeCreate()));
  connect(mSplineModeInsert,  SIGNAL(triggered()), this, SLOT(splineModeInsert()));
  connect(mSplineModeMove,  SIGNAL(triggered()), this, SLOT(splineModeMove()));  
  connect(mSplineExport,  SIGNAL(triggered()), this, SLOT(splineExport()));
  connect(mSplineExportLast,  SIGNAL(triggered()), this, SLOT(splineExportLast()));
  connect(mSplinetoggleAdjustGrid,  SIGNAL(triggered()), this, SLOT(splinetoggleAdjustGrid()));
  connect(mSplineCloseAll,  SIGNAL(triggered()), this, SLOT(splineCloseAll()));


  mHelpAbout = new QAction(tr("&About"), this);
  mHelpMenu->addAction(mHelpAbout);
  connect(mHelpAbout,  SIGNAL(triggered()), this, SLOT(showAbout()));


  ////////////////////////////
  //  Pal
  mPalNewColour   = new QAction(tr("&New colour"), this);
  mPalExportNpal  = new QAction(tr("&Export to .npal"), this);
  mPalMenu->addAction(mPalNewColour);
  mPalMenu->addAction(mPalExportNpal);
  connect(mPalNewColour,  SIGNAL(triggered()), this, SLOT(palNewColour()));
  connect(mPalExportNpal,  SIGNAL(triggered()), this, SLOT(palExportNpal()));


  ////////////////////////////
  //  Textures

  mTexExport            = new QAction(tr("&Export"), this);
  mTexExportDeferred    = new QAction(tr("&Export to Deferred - PNTUV"), this);
  mTexExportDeferredPNT = new QAction(tr("&Export to Deferred - PNT"), this);

  mTexMenu->addAction( mTexExport );
  mTexMenu->addAction( mTexExportDeferred );
  mTexMenu->addAction( mTexExportDeferredPNT );
  connect(mTexExport,  SIGNAL(triggered()), this, SLOT(texExport()));
  connect(mTexExportDeferred,  SIGNAL(triggered()), this, SLOT( texExportDeferred() ));
  connect(mTexExportDeferredPNT,  SIGNAL(triggered()), this, SLOT( texExportDeferredPNT() ));

  ////////////////////////////
  //  Tabs

}



/**** SLOTS ****/

void MainWindow::newF()
{
  int indexTab = mMainTab->currentIndex();
  if ( indexTab == mMeshTabId )
  {
    //QString fileName = QMessageBox()
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setWindowTitle("Dis-moi chérie");
    msgBox.setText("You want to lose you work?");
    msgBox.setIcon(QMessageBox::Question);
    if(msgBox.exec() == QMessageBox::Yes)
    {
      mMeshTab->mEditor->newFile();
      mMeshTab->reloadTexWidget();
    }
  }

  if ( indexTab == mGraphTabId )
  {
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setWindowTitle("Dis-moi chérie");
    msgBox.setText("You want to lose you work?");
    msgBox.setIcon(QMessageBox::Question);
    if(msgBox.exec() == QMessageBox::Yes)
    {
      mDirGraphTab->mEditor->newFile();
    }
  }
}

void MainWindow::open()
{
  int indexTab = mMainTab->currentIndex();
  if ( indexTab == mSplinesTabId )
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
         tr("Not Gordian Editor (*.ng)"));

     if (fileName != "") {
         QFile file(fileName);
         if (!file.open(QIODevice::ReadOnly)) {
             QMessageBox::critical(this, tr("Error"),
                 tr("Could not open file"));
             return;
         }
         GlobalEditor::gEditor->loadFromFile(file);
         file.close();
     }
  }

  if ( indexTab == mMeshTabId )
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Not Gordian Mesh (*.ngm)"));

    if (fileName != "")
    {
      mMeshTab->mEditor->loadFromFile( fileName, mMeshTab->mEditor->mode() );
      mMeshTab->reloadTexWidget();
    }
  }


  if ( indexTab == mMaterialTabId )
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Not Gordian Mesh (*.ngt)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"),
                tr("Could not open file"));
            return;
        }
        mTextureTab->loadFromFile( file );
        file.close();
    }

  }

  if ( indexTab == mGraphTabId )
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
        tr("Not Gordian Directed Acylic Graph (*.ndg)"));

    if (fileName != "")
    {
      mDirGraphTab->mEditor->loadFromFile( fileName );
    }
  }

}



void MainWindow::save()
{  
  int indexTab = mMainTab->currentIndex();
  if ( indexTab == mSplinesTabId )
  {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Not Gordian Editor (*.ng)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
          QMessageBox::critical(this, tr("Error"),
              tr("Could not open file for save"));
          return;
        } else {
            GlobalEditor::gEditor->saveToFile(file);
            file.close();

        }
    }
  }
  if ( indexTab == mMeshTabId )
  {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Not Gordian Mesh (*.ngm)"));

    if (fileName != "")
      mMeshTab->mEditor->saveToFile( fileName );

  }

  if ( indexTab == mMaterialTabId )
  {
    //texExport();
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
          tr("Not Gordian Texture (*.ngt)"));

      if (fileName != "") {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error"),
                tr("Could not open file for save"));
            return;
          } else {
              mTextureTab->saveToFile(file);
              file.close();

          }
      }
  }

  if ( indexTab == mGraphTabId )
  {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Not Gordian Editor Graph (*.ndg)"));

    if (fileName != "") {
            mDirGraphTab->mEditor->saveToFile(fileName);
        }
  }

}

void MainWindow::fileexport()
{
  int indexTab = mMainTab->currentIndex();

  if ( indexTab == mGraphTabId )
  {
    /*
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Text file (*.txt)"));

    if (fileName != "") {
            mDirGraphTab->mEditor->exportToFile(fileName);
        }
        */
    mDirGraphTab->mEditor->exportToFile();
  }

  if ( indexTab == mMaterialTabId )
  {
    texExport();
  }

}

void MainWindow::splineNew()
{
  GlobalEditor::gEditor->createNewSpline();
}

void MainWindow::quit()
{
}

void MainWindow::paste()
{
}

void MainWindow::copy()
{
}

void MainWindow::cut()
{
}

void MainWindow::redo()
{  
  int indexTab = mMainTab->currentIndex();
  if ( indexTab == mSplinesTabId )
    GlobalEditor::gEditor->redoEditorState();
  if ( indexTab == mMeshTabId )
  {
    mMeshTab->mEditor->redoEditorState();
    mMeshTab->setModeOpSel();
  }
}

void MainWindow::undo()
{
  int indexTab = mMainTab->currentIndex();
  if ( indexTab == mSplinesTabId )
    GlobalEditor::gEditor->undoEditorState();
  if ( indexTab == mMeshTabId )
  {
    mMeshTab->mEditor->undoEditorState();
    mMeshTab->setModeOpSel();
  }
}

void MainWindow::initPathsForShaders()
{
  QString path;
  std::wstring w;

  //  GENERAL
  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdTriPUV.fx");
  wcscpy_s ( mPathShad3Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdTrianglesPUV]->mFileName = (LPCWSTR)&mPathShad3Buffer[0];

  //  TRANSLUCENT
  path=  QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdGeomP.fx");
  wcscpy_s ( mPathShad1Buffer,  ((path.toStdWString()).c_str()) );  
  gStoredShaders[shadFwdGeomP]->mFileName = (LPCWSTR)&mPathShad1Buffer[0];

  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdPointPSC.fx");
  wcscpy_s ( mPathShad2Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdPointPSC]->mFileName = (LPCWSTR)&mPathShad2Buffer[0];

  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdGeomThickLinesPC.fx");
  wcscpy_s ( mPathShad4Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdGeomPC]->mFileName = (LPCWSTR)&mPathShad4Buffer[0];

  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdGeomFacePC.fx");
  wcscpy_s ( mPathShad8Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdGeomFacePC]->mFileName = (LPCWSTR)&mPathShad8Buffer[0];




  //  PHONG
  path=  QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdPhongGeomP.fx");
  wcscpy_s ( mPathShad5Buffer,  ((path.toStdWString()).c_str()) );
  gStoredShaders[shadFwdPhongGeomP]->mFileName = (LPCWSTR)&mPathShad5Buffer[0];

  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdPhongPointPSC.fx");
  wcscpy_s ( mPathShad6Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdPhongPointPSC]->mFileName = (LPCWSTR)&mPathShad6Buffer[0];

  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdPhongGeomPNC.fx");
  wcscpy_s ( mPathShad7Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdPhongGeomPNC]->mFileName = (LPCWSTR)&mPathShad7Buffer[0];

  //  Graph
  path=QCoreApplication::applicationDirPath();
  path.append("/Assets/shaders/editor/fwdLineGraphPC.fx");
  wcscpy_s ( mPathShad9Buffer, (path.toStdWString()).c_str() );
  gStoredShaders[shadFwdLineGraphPC]->mFileName = (LPCWSTR)&mPathShad9Buffer[0];

}



void MainWindow::keyPressEvent( QKeyEvent *event )
{
  //  TODO: send messages to right TAB
  int indexTab = mMainTab->currentIndex();

    if ( event->isAutoRepeat() ) {
        event->ignore();
    }
    else {

      ////////////////////////////////////////////
      //  All keys to splines
      if ( indexTab == mSplinesTabId )
      {
        if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier)
          GlobalEditor::gEditor->undoEditorState();

        if (event->key() == Qt::Key_Y && event->modifiers() == Qt::ControlModifier)
          GlobalEditor::gEditor->redoEditorState();

        if (event->key() == Qt::Key_Delete )
          GlobalEditor::gEditor->deleteObject();

        if (event->key() == Qt::Key_G )
          GlobalEditor::gEditor->toggleAdjustToGrid();
      }

      ////////////////////////////////////////////
      //  All keys to objEd
      if ( indexTab == mMeshTabId )
      {
        if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier)
          mMeshTab->mEditor->undoEditorState();

        if (event->key() == Qt::Key_Y && event->modifiers() == Qt::ControlModifier)
          mMeshTab->mEditor->redoEditorState();

        if (event->key() == Qt::Key_Escape )
          mMeshTab->setModeOpSel();

      }

      ////////////////////////////////////////////
      //  All keys to graphEd
      if ( indexTab == mGraphTabId )
      {
        if (event->key() == Qt::Key_Delete )
          mDirGraphTab->mEditor->deleteHighlighted();
      }
    }
}

void MainWindow::splineModeSelect()
{
  GlobalEditor::gEditor->modeSelect();
}

void MainWindow::splineModeCreate()
{
  GlobalEditor::gEditor->modeCreate();
}

void MainWindow::splineModeInsert()
{
  GlobalEditor::gEditor->modeInsert();
}

void MainWindow::splineModeMove()
{
  GlobalEditor::gEditor->modeMove();
}


void MainWindow::splinetoggleAdjustGrid()
{
  GlobalEditor::gEditor->toggleAdjustToGrid();
}

void MainWindow::splineExport()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "",
      tr("Plain text (*.txt);;C (*.c);;C++ (*.cpp);;C++ header (*.hpp)"));

  if (fileName != "") {
      QFile file(fileName);
      if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"),
            tr("Could not open file for save"));
        return;
      } else {
          GlobalEditor::gEditor->exportToFile(file);
          file.close();
      }
  }
}

void MainWindow::splineCopyCurrent()
{
  GlobalEditor::gEditor->copyCurrentSpline();
}

void MainWindow::showAbout()
{
  NotEditorAbout aboutBox;
  aboutBox.exec();
}

void MainWindow::splineCloseAll()
{
  GlobalEditor::gEditor->closeAllSplines();
}

void MainWindow::splineExportLast()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File (Last Point)"), "",
      tr("Plain text (*.txt);;C (*.c);;C++ (*.cpp);;C++ header (*.hpp)"));

  if (fileName != "") {
      QFile file(fileName);
      if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"),
            tr("Could not open file for save"));
        return;
      } else {
          GlobalEditor::gEditor->exportToFileLastPoint(file);
          file.close();
      }
  }

}

void MainWindow::palNewColour()
{
  mPalTab->newColour();
}

void MainWindow::palExportNpal()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export to Npal"), "",
      tr("Not Palette ( *.npal )"));

  if (fileName != "") {
      QFile file(fileName);
      if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"),
            tr("Could not open file for save"));
        return;
      } else {

          mPalTab->exportToNpal(file);
          file.close();
      }
  }


}

void MainWindow::updateStatusBarByTab(QString aText)
{
  statusBar()->showMessage( aText );
}

void MainWindow::texExport()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "./Assets/Shaders/",
      tr("Dx-fx Hlsl 4 (*.fx)"));

  if (fileName != "")
  {
    mTextureTab->shaderExport( fileName );
  }

}

void MainWindow::texExportDeferred()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File [Deferred]"), "./Assets/Shaders/",
      tr("Dx-fx Hlsl 4 (*.fx)"));

  if (fileName != "")
  {
    mTextureTab->shaderExportDeferred( fileName, 0 );
  }

}

void MainWindow::texExportDeferredPNT()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File [Deferred]"), "./Assets/Shaders/",
      tr("Dx-fx Hlsl 4 (*.fx)"));

  if (fileName != "")
  {
    mTextureTab->shaderExportDeferred( fileName, 1 );
  }

}


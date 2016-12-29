#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP



#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGraphicsView>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QInputDialog>
#include <QRadioButton>
#include <QApplication>
#include <QtGui>
#include <QMessageBox>


#include "UI_Bezier/QD3DWidgetBezier.hpp"
#include "UI_Bezier/TabBezierWindow.h"
#include "UI_Bezier/ViewManager.h"
#include "UI_Pal/TabPalWindow.h"
#include "UI_Textures/TabTextureWindow.hpp"
#include "UI_Wavelet/TabWaveWindow.h"
#include "UI_ObjEditor/TabObjEditor.h"
#include "UI_DirGraph/TabDirGEditor.h"

#include "noteditorabout.h"

//class editorbezier;
#include "Editor_Bezier/editorbezier.h"
#include "Editor_Bezier/EditorGlobal.hpp"



class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
    void initAllStuff();
    void keyPressEvent( QKeyEvent *event );
    void updateStatusBarByTab ( QString aText );

  private slots:
    void newF();
    void open();
    void save();
    void fileexport();
    void quit();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    void splineNew();
    void splineCopyCurrent();
    void splineModeSelect();
    void splineModeCreate();
    void splineModeInsert();
    void splineModeMove();
    void splineExport();
    void splineExportLast();
    void splinetoggleAdjustGrid();
    void splineCloseAll();

    void palNewColour();
    void palExportNpal();

    void  texExport();
    void  texExportDeferred();
    void  texExportDeferredPNT();

    void showAbout();

    ///////////////////////////////////////////////////////
    //  Gui elements
  private:
    QTabWidget          *mMainTab;
    TabBezierWindow     *mSplineslTab;
    TabPalWindow        *mPalTab;
    TabTextureWindow    *mTextureTab;
    TabWaveWindow       *mWaveletTab;
    TabObjEditor        *mMeshTab;
    TabDirGEditor       *mDirGraphTab;

    QAction *mFileNewAction;
    QAction *mFileOpenAction;
    QAction *mFileSaveAction;
    QAction *mFileExportAction;
    QAction *mFileExitAction;

    QAction *mEditUndoAction;
    QAction *mEditRedoAction;
    QAction *mEditCutAction;
    QAction *mEditCopyAction;
    QAction *mEditPasteAction;

    QAction *mSplineNew;
    QAction *mSplineCopyCurrent;
    QAction *mSplineExport;
    QAction *mSplineExportLast;
    QAction *mSplinetoggleAdjustGrid;
    QAction *mSplineCloseAll;

    QAction *mSplineModeSelect;
    QAction *mSplineModeCreate;
    QAction *mSplineModeInsert;
    QAction *mSplineModeMove;
    QAction *mSplineBackChoose;
    QAction *mSplineBackMute;

    QAction *mPalNewColour;
    QAction *mPalExportNpal;

    QAction *mTexExport;
    QAction *mTexExportDeferred;
    QAction *mTexExportDeferredPNT;

    QAction *mHelpAbout;

    QMenu *mFileMenu;
    QMenu *mEditMenu;
    QMenu *mSplineMenu;
    QMenu *mHelpMenu;
    QMenu *mPalMenu;
    QMenu *mTexMenu;

    QMenu *mSplineModeMenu;
    QMenu *mSplineBackMenu;

    int mMeshTabId;
    int mMaterialTabId;
    int mSplinesTabId;
    int mGraphTabId;

    ///////////////////////////////////////////////////////
    //  Data layer elements
public:
    EditorBezier*   mEditor;

private:
     wchar_t mPathShad1Buffer[2048];
     wchar_t mPathShad2Buffer[2048];
     wchar_t mPathShad3Buffer[2048];
     wchar_t mPathShad4Buffer[2048];
     wchar_t mPathShad5Buffer[2048];
     wchar_t mPathShad6Buffer[2048];
     wchar_t mPathShad7Buffer[2048];
     wchar_t mPathShad8Buffer[2048];
     wchar_t mPathShad9Buffer[2048];



private:
    void    initMenu();
    void    initPathsForShaders();

};






#endif // MAINWINDOW_HPP

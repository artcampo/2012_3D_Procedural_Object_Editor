#ifndef _TabObjEditor_H
#define _TabObjEditor_H


#include <QMessageBox>
#include <QString>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QDesktopWidget>
#include <QSplitter>

#include "ObjEditor/objectEditor.h"

#include "UI_ObjEditor/QD3DWidgetObj.hpp"
#include "UI_ObjEditor/ObjView.h"
#include "UI_ObjEditor/ObjectTreeWidget.h"
#include "ObjEditor/ObjEditorSharedData.hpp"

#include "UI_ObjEditor/OpWidgets/opknifewidget.h"
#include "UI_ObjEditor/OpWidgets/opextrudewidget.h"
#include "UI_ObjEditor/OpWidgets/opTextureWidget.h"

class TabObjEditor : public QWidget
{
    Q_OBJECT
public:
    TabObjEditor(QWidget *parent);
    ~TabObjEditor();
    void recomputeArea();
    bool eventFilter(QObject* obj, QEvent* event);


    void  initEdition();
    void  reloadTexWidget();

    void  selectionMode();

    //  important
private:
    void  updateViews();

    //second clss
private:
    void  setBackgroundImageDialog();
    void  initConfiguration();
    void  initButtonBar();
    void  initOpWidgets();

    void initOptions();
    void initRenderWidgets();
    void initQtSlotsForOperations();
    void updateOptions();

    void cancelKnifeOperation();

private:
    ObjectEditor*       mEditor;
    ObjectTreeWidget*   mObjectTree;

    QD3DWidgetObj*      mSingleWindow;    

    QD3DWidgetObj*      mMultiWindow[4];


    bool                mSingleWindowMode;

    QD3DWidgetObj*    mCurrentWindow;

    //  Layout
private:
    QVBoxLayout*  mLayout;
    QHBoxLayout*  mCentralLayout;
    QGridLayout*  mRenderLayout;
    QVBoxLayout*  mSideLayout;
    QHBoxLayout*  mButtonBarLayout;
    QVBoxLayout*  mTreeAndOpLayout;

    //  Buttons
private:
    QPushButton*  mSwapViewsButton;    
    QPushButton*  mRenderChangeButton;

    QSplitter*    mBut1Split;

    QPushButton*  mModeOButton;
    QPushButton*  mModeVButton;
    QPushButton*  mModeEButton;
    QPushButton*  mModeFButton;

    QSplitter*    mBut2Split;

    QPushButton*  mCubeButton;

    QSplitter*    mBut3Split;

    QPushButton*  mModeOpRotButton;
    QPushButton*  mModeOpScaButton;
    QPushButton*  mModeOpExtButton;
    QPushButton*  mModeOpBevButton;
    QPushButton*  mModeOpKniButton;
    QPushButton*  mModeOpNoScButton;
    QPushButton*  mModeOpSelButton;
    QPushButton*  mModeOpTexButton;

    // op widgets
private:
    OpKnifeWidget*    mOpKnifeW;
    OpExtrudeWidget*  mOpExtrW;
    OpTextureWidget*  mOpTex;

    //  Image background and shit
private:
    QPushButton*  mButtonImage;
    QPushButton*  mButtonImageMute;

    QSlider*      mBackgroundSizeSlider;
    QSlider*      mBackgroundAlphaSlider;
    QSlider*      mGridMultiplierSlider;

    int   mTargetHeight;
    int   mTargetWidth;

    int   mRenderHeight;
    int   mRenderWidth;

    float mGridMultiplier;
    float mBackgroundImageAlpha;
    float mBackgroundImageSize;
    float mSizeSliderMax;
    bool    mBackgroundImagePresent;
    QString mFileName;

    bool  mDelayInit;

    static const int   mObjectsTreePixelWidth = 150;

private:
    Options::ExtrudeOptions   opEO;
    Options::ExtInnerOptions  opEI;
    Options::ExtMatrixOptions opEM;

    bool    mHasModeBeforeKnife;
    int     mKnifeModeBefore;

private:
    void downModeButtons();
    void downModeOpButtons();
    void hideOpWidgets();
    //  Image
private slots:
    void updateViewsSlot();
    void loadImage();    
    void muteImage();

    void setValueBackgroundSize(int v);
    void setValueBackgroundAlpha(int v);
    void setValueGridMultiplier(int v);    

    //  render
private slots:

    friend class MainWindow;

    //  logic / modes
private slots:
    void  swapViews();
    void  changeRender();
    void  setModeO (bool aUpdateViews = true );
    void  setModeV (bool aUpdateViews = true );
    void  setModeE (bool aUpdateViews = true );
    void  setModeF (bool aUpdateViews = true );
    void  setModeBeforeKnife();

    void  cubeButtonPressed();

    void  setModeOpSel ();
    void  setModeOpRot ();
    void  setModeOpSca ();
    void  setModeOpExt ();
    void  setModeOpBev ();
    void  setModeOpKni ();
    void  setModeOpNoSc ();
    void  setModeOpTex ();

public slots:
    //  for op widgets
    void  widOpAtHalf(int state);
    void  widOpAtHalf12(int state);

    void  extrudeTypeChangedSlot(int mode);
    void  npartsEOchanged( int val );
    void  npartsEIchanged( int val );
    void  npartsEMchanged( int val );

};

#endif // TabObjEditor_H

#ifndef _TabDigGEditor_H
#define _TabDigGEditor_H


#include <QMessageBox>
#include <QString>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QDesktopWidget>
#include <QSplitter>
#include <Qstring>

#include "QD3DWidgetDirG.hpp"
#include "UI_DirGraph/MinorWidgets/NameButton.hpp"

class TabDirGEditor : public QWidget
{
    Q_OBJECT
public:
    TabDirGEditor(QWidget *parent);
    ~TabDirGEditor();
    void recomputeArea();
    bool eventFilter(QObject* obj, QEvent* event);


    void  initConfiguration();
    void  initRenderWidgets();

    //  important
private:
    void  updateViews();



public:
    DirGraphEditor*      mEditor;
private:
    QD3DWidgetDirG*       mSingleWindow;
    NameButton*           mGraphName;
    QPushButton*          mAdjustYButton;
    QPushButton*          mAdjustToGridButton;

    //  Layout
private:
    QVBoxLayout*  mLayout;
    QVBoxLayout*  mCentralLayout;
    QGridLayout*  mRenderLayout;
    QHBoxLayout*  mButtonLayout;

    int   mTargetHeight;
    int   mTargetWidth;

    int   mRenderHeight;
    int   mRenderWidth;

    float mGridMultiplier;

    QString mFileName;

    bool  mDelayInit;


    friend class MainWindow;


private slots:
    void updateViewsSlot();
    void graphNameChanged( QString aName );
    void graphNameChangedByEditor( QString aName );
    void adjustYCoord();
    void adjustToGridCoord();

public slots:


};

#endif // _TabDigGEditor_H

#ifndef _TabBezierWindow_H
#define _TabBezierWindow_H


#include <QMessageBox>
#include <QString>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include "QD3DWidgetBezier.hpp"

#include "UI_Bezier/ViewManager.h"
#include "UI_Bezier/objecttree.h"

class TabBezierWindow : public QWidget
{
    Q_OBJECT
public:
    TabBezierWindow(QWidget *parent);
    ~TabBezierWindow();
    void recomputeArea();
    bool eventFilter(QObject* obj, QEvent* event);
private:
    QGridLayout*  mLayout;
    QGridLayout*  mButtonLayout;
    QGridLayout*  mButtonAndTreeLayout;

    QPushButton*  mButtonImage;
    QPushButton*  mButtonImageMute;
    QPushButton*  mButtonModeSelect;
    QPushButton*  mButtonModeCreate;
    QPushButton*  mButtonModeInsert;
    QPushButton*  mButtonModeMoveSpline;

    QD3DWidgetBezier*   mViewX;
    QD3DWidgetBezier*   mViewY;
    QD3DWidgetBezier*   mViewZ;
    QD3DWidgetBezier*   mViewFree;

    ObjectTree*   mObjectTree;

    QSlider*      mBackgroundSizeSlider;
    QSlider*      mBackgroundAlphaSlider;
    QSlider*      mGridMultiplierSlider;

    int   mTargetHeight;
    int   mTargetWidth;

    float mGridMultiplier;
    float mBackgroundImageAlpha;
    float mBackgroundImageSize;
    float mSizeSliderMax;
    bool    mBackgroundImagePresent;
    QString mFileName;

    bool  mDelayInit;

    static const int   mObjectsTreePixelWidth = 150;
private slots:
    void loadImage();    
    void muteImage();
    void splineModeSelect();
    void splineModeCreate();
    void splineModeInsert();
    void splineModeMove();

    void setValueBackgroundSize(int v);
    void setValueBackgroundAlpha(int v);
    void setValueGridMultiplier(int v);
};

#endif // TabBezierWindow_H

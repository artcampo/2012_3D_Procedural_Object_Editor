#ifndef _Gradient2Widget_COLOURWIDGET_H
#define _Gradient2Widget_COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>
#include "UI_Textures\MinorWidgets\GradientRenderer.h"

#include "App/dx_misc.hpp"

class Gradient2Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Gradient2Widget(QWidget *parent, D3DXCOLOR* aColor1, D3DXCOLOR* aColor2 );
public:
  QHBoxLayout*  mLayout;
  QPushButton*  mButtonPick1;
  QPushButton*  mButtonPick2;
  GradientRenderer* renderArea;

  QColor        mColour1;
  QColor        mColour2;

  D3DXCOLOR*    mStoredColour1;
  D3DXCOLOR*    mStoredColour2;

  void colours( D3DXCOLOR* aColor1, D3DXCOLOR* aColor2 );

signals:
  void changed();

public slots:
  void  pickColour1();
  void  pickColour2();
  void changePen();

private:


};

#endif // COLOURWIDGET_H

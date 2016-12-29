#ifndef _SINGLE_COLOURWIDGET_H
#define _SINGLE_COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>

#include "App/dx_misc.hpp"

class SingleColourWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SingleColourWidget(QWidget *parent, D3DXCOLOR* aColor);
public:
  QHBoxLayout*  mLayout;
  QPushButton*  mButtonPick;
  QTextEdit*    mArea;
  QColor        mColour;
  D3DXCOLOR*    mStoredColour;

signals:
  void changed();

public slots:
  void  pickColour();

};

#endif // COLOURWIDGET_H

#ifndef _SplineNWidget_COLOURWIDGET_H
#define _SplineNWidget_COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QComboBox>
#include "UI_Textures\MinorWidgets\GradientRenderer.h"

#include "App/dx_misc.hpp"
#include <vector>

class ProcShader;

class SplineNWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SplineNWidget(QWidget *parent,
                           std::vector<D3DXCOLOR>& aColours,
                           int aColorId, ProcShader* aProcShader );
public:
  QHBoxLayout*                    mLayout;
  QVBoxLayout*                    mSelectionLayout;
  QHBoxLayout*                    mSelectionLayout1;
  QHBoxLayout*                    mSelectionLayout2;
  std::vector<QPushButton*>       mButtonPick;
  std::vector<GradientRenderer*>  renderArea;
  std::vector<QColor>             mColour;
  QComboBox*                      mTypeCombo;


  int                         mColorId;
  std::vector<D3DXCOLOR*>     mStoredColour;
  int                         mNumColor;

  ProcShader* mProcShader;

  void colours( std::vector<D3DXCOLOR>& aColours );
  void change( int aColorId );
  void numColours();
private:
  void pickColour( int aIndex );

signals:
  void changed();

public slots:

  void  pickColour0();
  void  pickColour1();
  void  pickColour2();
  void  pickColour3();
  void  pickColour4();
  void  pickColour5();
  void  changePen();
  void  colMapTypeChanged();


public:
  static const int mMaxColours  = 6;
  static const int mMaxAreas    = 3;

  private:
  void removePreviousWidgets( const int aPreviousNumColours );
  void addWidgets( const int aCurrentNumColours );

};

#endif // COLOURWIDGET_H

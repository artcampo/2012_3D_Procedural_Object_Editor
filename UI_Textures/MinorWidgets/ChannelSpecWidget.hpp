#ifndef _ChannelSpecWidget_COLOURWIDGET_H
#define _ChannelSpecWidget_COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

#include "App/dx_misc.hpp"
#include "ChannelNameButton.hpp"
#include "UI_Textures/MinorWidgets/LoadChannelDialog.hpp"
#include "UI_Textures/MinorWidgets/SingleColourWidget.h"

class ChannelSpecWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelSpecWidget(QWidget *parent, int aChannelId, QString& aName );
public:  
  QHBoxLayout*  mLayout;

  ChannelNameButton*    mNameButton;
  SingleColourWidget*   mColorPicker;
  QDoubleSpinBox*       mPercSpin;

  void  changeName (QString& aName); 

private:
  int mChannelId;  
  bool mInhibitSignalChangedSlot;

public:
  float       mSpecPercentage;
  D3DXCOLOR   mSpecColour;


public slots:
  void signalChanged();

signals:
  void channelSelectedForLoading ( QString aName );
  void channelSelectedForSaving ( QString aName );
  void channelAskList();

  void changed();


};

#endif //ChannelSpecWidget_H

#ifndef _ChannelWidget_COLOURWIDGET_H
#define _ChannelWidget_COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QLabel>

#include "App/dx_misc.hpp"
#include "ChannelNameButton.hpp"
#include "UI_Textures/MinorWidgets/LoadChannelDialog.hpp"

class ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget *parent, int aChannelId, QString& aName, const float aDepth );
public:  
  QHBoxLayout*  mLayout;

  ChannelNameButton*  mNameButton;
  QLabel*           mDepthLabel;
  QDoubleSpinBox*   mDepthSpin;
  QPushButton*  mSaveButton;
  QPushButton*  mLoadButton;

  void  changeName (QString& aName);
  void  changeDepth( const float aDepth );
  void  getListChannels ( std::vector<QString> aList );

private:
  int mChannelId;
  std::vector<QString> mList;

public slots:
  void  save();
  void  load();

signals:
  void channelSelectedForLoading ( QString aName );
  void channelSelectedForSaving ( QString aName );
  void channelAskList();

};

#endif //ChannelWidget_H

#ifndef _LoadChannelDialog_H
#define _LoadChannelDialog_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDialog>
#include <QListWidget>

#include "App/dx_misc.hpp"

class LoadChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoadChannelDialog(QWidget *parent, std::vector<QString> aNames );
public:  
  QVBoxLayout*  mLayout;
  QHBoxLayout*  mNestedLayout;

  QListWidget*  mNamesList;

  QPushButton*  mAcceptButton;
  QPushButton*  mCancelButton;

  QString       selected();

private:

};

#endif //ChannelWidget_H

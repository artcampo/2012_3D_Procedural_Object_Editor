#ifndef COLOURWIDGET_H
#define COLOURWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QTextBrowser>
#include <QColor>
#include <QScrollArea>

class ColourWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColourWidget(QWidget *parent = 0);
public:
  QPushButton*  mButtonPick;
  QPushButton*  mButtonDel;
  QTextEdit*    mArea;
  QColor        mColour;
  bool          mDelete;
signals:

public slots:
  void  pickColour();
  void  deleteMe();
};

#endif // COLOURWIDGET_H

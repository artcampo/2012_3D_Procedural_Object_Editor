#ifndef OPKNIFEWIDGET_H
#define OPKNIFEWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QCheckBox>
#include <QVBoxLayout>


class OpKnifeWidget : public QFrame
{
    Q_OBJECT
public:
    explicit OpKnifeWidget(QWidget *parent = 0);

public:
  QCheckBox*   mAtHalfCheckBox;
  QCheckBox*   mAt1CheckBox;
  QCheckBox*   mAt2CheckBox;

  QVBoxLayout*  mLayout;

signals:  

public slots:


};

#endif // OPKNIFEWIDGET_H

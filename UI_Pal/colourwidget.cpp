#include "colourwidget.h"

ColourWidget::ColourWidget(QWidget *parent) :
    QWidget(parent)
{
  mColour =  Qt::white;
  mDelete = false;

  mButtonPick = new QPushButton;
  mButtonPick->setText( QString ("Set"));
  QObject::connect(mButtonPick, SIGNAL(clicked()), this, SLOT(pickColour()));  
  mButtonPick->setFixedSize( 36, 27);

  mButtonDel = new QPushButton;
  mButtonDel->setText( QString ("Delete"));
  QObject::connect(mButtonDel, SIGNAL(clicked()), this, SLOT(pickColour()));
  mButtonDel->setFixedSize( 36, 27);

  mArea = new QTextEdit ();
  mArea->setFixedSize( 100, 27);
  //mArea->setText(QString("try"));
  QPalette p=palette();
  p.setColor(QPalette::Base, mColour);
  mArea->setPalette(p);
  mArea->setAutoFillBackground(true);
  mArea->setEnabled(false);

  //        p.setColor(QPalette::Base, QColor(240, 240, 255));


  /*
  mArea->setTextBackgroundColor ( mColour );
  mArea->setBackgroundRole( );
  mArea->setAutoFillBackground(true);*/
  //mArea->setEnabled(false);
}

void ColourWidget::pickColour()
{
  //QColor
  QColor defaultCol(Qt::white);
  QString title ("Choose colour");
  mColour =
      QColorDialog::getColor
      ( defaultCol, this, title, QColorDialog::ShowAlphaChannel );

  //pick.
  QPalette p=palette();
  p.setColor(QPalette::Base, mColour);
  mArea->setPalette(p);

  //mCo//lour = mArea->textBackgroundColor();
}

void ColourWidget::deleteMe()
{
  mDelete = true;
}

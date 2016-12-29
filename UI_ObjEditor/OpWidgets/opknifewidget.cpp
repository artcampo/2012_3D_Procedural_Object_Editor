#include "opknifewidget.h"

OpKnifeWidget::OpKnifeWidget(QWidget *parent) :
    QFrame(parent)
{

  mLayout = new QVBoxLayout( this );
  setLayout( mLayout );

  mAtHalfCheckBox = new QCheckBox( QString("Cut edge at half"),       this);
  mAt1CheckBox    = new QCheckBox( QString("Cut edge at one third"),  this);
  mAt2CheckBox    = new QCheckBox( QString("Cut edge at two thirds"), this);

  mLayout->setAlignment( Qt::AlignTop );

  mLayout->addWidget(mAtHalfCheckBox);
  mLayout->addWidget(mAt1CheckBox);
  mLayout->addWidget(mAt2CheckBox);

  //setFrameStyle(QFrame::Box | QFrame::Sunken);
}

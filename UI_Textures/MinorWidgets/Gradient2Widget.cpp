#include "Gradient2Widget.hpp"
#include "UI_Textures\MinorWidgets\GradientRenderer.h"

Gradient2Widget::Gradient2Widget(QWidget *parent, D3DXCOLOR* aColor1, D3DXCOLOR* aColor2) :
    QWidget(parent)
{
  mStoredColour1 = aColor1;
  mStoredColour2 = aColor2;


  isVisible();

  mLayout = new QHBoxLayout(this);
  setLayout(mLayout);

  mButtonPick1 = new QPushButton(this);
  mButtonPick1->setText( QString ("Set 1"));
  QObject::connect(mButtonPick1, SIGNAL(clicked()), this, SLOT(pickColour1()));
  mButtonPick1->setFixedSize( 36, 27);

  mButtonPick2 = new QPushButton(this);
  mButtonPick2->setText( QString ("Set 2"));
  QObject::connect(mButtonPick2, SIGNAL(clicked()), this, SLOT(pickColour2()));
  mButtonPick2->setFixedSize( 36, 27);


  renderArea = new GradientRenderer(this);

  QColor c1 = QColor::fromRgbF(mStoredColour1->r, mStoredColour1->g, mStoredColour1->b, 1);
  QColor c2 = QColor::fromRgbF(mStoredColour2->r, mStoredColour2->g, mStoredColour2->b, 1);
  renderArea->setColors( c1, c2 );


  mLayout->addWidget( mButtonPick1 );
  mLayout->addWidget( renderArea );
  mLayout->addWidget( mButtonPick2 );

}

void Gradient2Widget::changePen()
{
  QColor c1 = QColor::fromRgbF(mStoredColour1->r, mStoredColour1->g, mStoredColour1->b, 1);
  QColor c2 = QColor::fromRgbF(mStoredColour2->r, mStoredColour2->g, mStoredColour2->b, 1);
  renderArea->setColors( c1, c2 );
}

void Gradient2Widget::pickColour1()
{
  //QColor
  mColour1 = QColor( mStoredColour1->r*255, mStoredColour1->g*255, mStoredColour1->b*255, mStoredColour1->a*255 );
  QString title ("Choose colour");
  mColour1 =
      QColorDialog::getColor
      ( mColour1, this, title, QColorDialog::ShowAlphaChannel );


  qreal r,g,b,a;
  mColour1.getRgbF( &r,&g,&b,&a);
  mStoredColour1->r = r;
  mStoredColour1->g = g;
  mStoredColour1->b = b;
  mStoredColour1->a = a;

  changePen();
  emit changed();
}


void Gradient2Widget::pickColour2()
{
  //QColor
  mColour2 = QColor( mStoredColour2->r*255, mStoredColour2->g*255, mStoredColour2->b*255, mStoredColour2->a*255 );
  QString title ("Choose colour");
  mColour2 =
      QColorDialog::getColor
      ( mColour2, this, title, QColorDialog::ShowAlphaChannel );


  qreal r,g,b,a;
  mColour2.getRgbF( &r,&g,&b,&a);
  mStoredColour2->r = r;
  mStoredColour2->g = g;
  mStoredColour2->b = b;
  mStoredColour2->a = a;

  changePen();
  emit changed();
}

void Gradient2Widget::colours( D3DXCOLOR* aColor1, D3DXCOLOR* aColor2)
{
  mStoredColour1 = aColor1;
  mStoredColour2 = aColor2;
}

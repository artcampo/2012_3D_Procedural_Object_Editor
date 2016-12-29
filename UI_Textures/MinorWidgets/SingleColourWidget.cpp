#include "SingleColourWidget.h"
#include "UI_Textures\MinorWidgets\GradientRenderer.h"

SingleColourWidget::SingleColourWidget(QWidget *parent, D3DXCOLOR* aColor) :
    QWidget(parent)
{
  mStoredColour = aColor;
  isVisible();

  mLayout = new QHBoxLayout(this);
  setLayout(mLayout);

  mButtonPick = new QPushButton(this);
  mButtonPick->setText( QString ("Set"));
  QObject::connect(mButtonPick, SIGNAL(clicked()), this, SLOT(pickColour()));  
  mButtonPick->setFixedSize( 36, 27);


  mArea = new QTextEdit (this);
  mArea->setFixedSize( 100, 27);
  //mArea->setText(QString("try"));
  QPalette p=palette();
  p.setColor(QPalette::Base, mColour);
  mArea->setPalette(p);
  mArea->setAutoFillBackground(true);
  mArea->setEnabled(false);

  mLayout->addWidget( mButtonPick );
  mLayout->addWidget( mArea );


  GradientRenderer* renderArea = new GradientRenderer(this);

  QLinearGradient linearGradient(0, 0, 100, 100);
   linearGradient.setColorAt(0.0, Qt::white);
   linearGradient.setColorAt(0.2, Qt::green);
   linearGradient.setColorAt(1.0, Qt::black);
   renderArea->setBrush(linearGradient);

   Qt::PenStyle style = Qt::PenStyle(Qt::SolidLine);
   Qt::PenCapStyle cap = Qt::PenCapStyle(Qt::SquareCap);
   Qt::PenJoinStyle join = Qt::PenJoinStyle(Qt::BevelJoin);

   renderArea->setPen(QPen(Qt::blue, 100, style, cap, join));

   QHBoxLayout* newLayout = new QHBoxLayout(this);
   newLayout->addWidget( renderArea );
   newLayout->setGeometry( QRect(0,0,10,10));
   mLayout->addLayout(newLayout);

  //mLayout->addWidget( renderArea );

  //        p.setColor(QPalette::Base, QColor(240, 240, 255));


  /*
  mArea->setTextBackgroundColor ( mColour );
  mArea->setBackgroundRole( );
  mArea->setAutoFillBackground(true);*/
  //mArea->setEnabled(false);
}

void SingleColourWidget::pickColour()
{
  //QColor
  mColour = QColor( mStoredColour->r*255, mStoredColour->g*255, mStoredColour->b*255, mStoredColour->a*255 );
  QString title ("Choose colour");
  mColour =
      QColorDialog::getColor
      ( mColour, this, title, QColorDialog::ShowAlphaChannel );


  qreal r,g,b,a;
  mColour.getRgbF( &r,&g,&b,&a);
  mStoredColour->r = r;
  mStoredColour->g = g;
  mStoredColour->b = b;
  mStoredColour->a = a;

  //pick.
  QPalette p=palette();
  p.setColor(QPalette::Base, mColour);
  mArea->setPalette(p);

  emit changed();
}

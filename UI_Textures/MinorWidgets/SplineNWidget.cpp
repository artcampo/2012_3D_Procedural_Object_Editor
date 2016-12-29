#include "SplineNWidget.hpp"
#include "UI_Textures\MinorWidgets\GradientRenderer.h"
#include "Editor_Texture/DataLayer/Layer.hpp"
#include "Editor_Texture/DataLayer/ProcShader.hpp"
#include <vector>
using namespace std;

SplineNWidget::SplineNWidget(QWidget *parent,
                             std::vector<D3DXCOLOR>& aColours,
                             int aColorId,
                             ProcShader* aProcShader  ) :
    QWidget(parent),
    mProcShader(aProcShader)
{

  mColorId = aColorId;

  numColours();

  isVisible();

  mLayout = new QHBoxLayout(this);
  setLayout(mLayout);
  mSelectionLayout = new QVBoxLayout(this);
  mSelectionLayout1 = new QHBoxLayout(this);
  mSelectionLayout2 = new QHBoxLayout(this);

  mStoredColour.resize(mMaxColours);
  mColour.resize(mMaxColours);
  mButtonPick.resize(mMaxColours);

  for ( int i = 0; i < mNumColor; ++i )
    mStoredColour[i] = &aColours[i];

  for ( int i = 0; i < mMaxColours; ++i )
  {
    mButtonPick[i] = new QPushButton(this);
    mButtonPick[i]->setText( QString ("Set"));    
    mButtonPick[i]->setFixedSize( 36, 27);
    mButtonPick[i]->hide();
    switch(i)
    {
      case 0: QObject::connect(mButtonPick[0], SIGNAL(clicked()), this, SLOT(pickColour0())); break;
      case 1: QObject::connect(mButtonPick[1], SIGNAL(clicked()), this, SLOT(pickColour1())); break;
      case 2: QObject::connect(mButtonPick[2], SIGNAL(clicked()), this, SLOT(pickColour2())); break;
      case 3: QObject::connect(mButtonPick[3], SIGNAL(clicked()), this, SLOT(pickColour3())); break;
      case 4: QObject::connect(mButtonPick[4], SIGNAL(clicked()), this, SLOT(pickColour4())); break;
      case 5: QObject::connect(mButtonPick[5], SIGNAL(clicked()), this, SLOT(pickColour5())); break;
    }
  }


  //for 1 color
  renderArea.resize( 1 + 2 + 3);
  renderArea[0] = new GradientRenderer(this, 100);
  renderArea[0]->hide();
  //for 2 color
  renderArea[1] = new GradientRenderer(this, 50);
  renderArea[1]->hide();
  renderArea[2] = new GradientRenderer(this, 50);
  renderArea[2]->hide();
  //for 3 color
  renderArea[3] = new GradientRenderer(this, 33);
  renderArea[3]->hide();
  renderArea[4] = new GradientRenderer(this, 33);
  renderArea[4]->hide();
  renderArea[5] = new GradientRenderer(this, 33);
  renderArea[5]->hide();

  //  combo
  mTypeCombo = new QComboBox(this);
  int numColMaps = mProcShader->getNumLayerColourMapping();
  for (unsigned int i = 0; i < numColMaps ; ++i)
    mTypeCombo->addItem( QString::fromStdString(mProcShader->getNameLayerColourMapping(i)) );

  mTypeCombo->setFixedWidth( mTypeCombo->sizeHint().width() );
  connect( mTypeCombo, SIGNAL( currentIndexChanged(int) ), this, SLOT(colMapTypeChanged() ));

  mLayout->addWidget( mTypeCombo );
  mLayout->addLayout( mSelectionLayout );
  mSelectionLayout->addLayout( mSelectionLayout1 );
  mSelectionLayout->addLayout( mSelectionLayout2 );
  addWidgets(mNumColor);
  //  renew contents
  changePen();
}

void SplineNWidget::colMapTypeChanged()
{
  change(mTypeCombo->currentIndex());
}

void SplineNWidget::changePen()
{
  switch ( mNumColor )
  {
    case 2:
            mColour[0] = QColor::fromRgbF(mStoredColour[0]->r, mStoredColour[0]->g, mStoredColour[0]->b, 1);
            mColour[1] = QColor::fromRgbF(mStoredColour[1]->r, mStoredColour[1]->g, mStoredColour[1]->b, 1);
            renderArea[0]->setColors( mColour[0], mColour[1] );
            break;
    case 4:
            mColour[0] = QColor::fromRgbF(mStoredColour[1]->r, mStoredColour[1]->g, mStoredColour[1]->b, 1);
            mColour[1] = QColor::fromRgbF(mStoredColour[2]->r, mStoredColour[2]->g, mStoredColour[2]->b, 1);
            renderArea[0]->setColors( mColour[0], mColour[1] );
            break;
    case 5:
            mColour[0] = QColor::fromRgbF(mStoredColour[1]->r, mStoredColour[1]->g, mStoredColour[1]->b, 1);
            mColour[1] = QColor::fromRgbF(mStoredColour[2]->r, mStoredColour[2]->g, mStoredColour[2]->b, 1);
            mColour[2] = QColor::fromRgbF(mStoredColour[3]->r, mStoredColour[3]->g, mStoredColour[3]->b, 1);
            renderArea[1]->setColors( mColour[0], mColour[1] );
            renderArea[2]->setColors( mColour[1], mColour[2] );
            break;
    case 6:
            mColour[0] = QColor::fromRgbF(mStoredColour[1]->r, mStoredColour[1]->g, mStoredColour[1]->b, 1);
            mColour[1] = QColor::fromRgbF(mStoredColour[2]->r, mStoredColour[2]->g, mStoredColour[2]->b, 1);
            mColour[2] = QColor::fromRgbF(mStoredColour[3]->r, mStoredColour[3]->g, mStoredColour[3]->b, 1);
            mColour[3] = QColor::fromRgbF(mStoredColour[4]->r, mStoredColour[4]->g, mStoredColour[4]->b, 1);
            renderArea[3]->setColors( mColour[0], mColour[1] );
            renderArea[4]->setColors( mColour[1], mColour[2] );
            renderArea[5]->setColors( mColour[2], mColour[3] );
            break;
  };
}

void SplineNWidget::pickColour( int aIndex )
{
  int i = aIndex;
  //QColor
  mColour[i] = QColor( mStoredColour[i]->r*255, mStoredColour[i]->g*255, mStoredColour[i]->b*255, mStoredColour[i]->a*255 );
  QString title ("Choose colour");
  mColour[i] =
      QColorDialog::getColor
      ( mColour[i], this, title, QColorDialog::ShowAlphaChannel );


  qreal r,g,b,a;
  mColour[i].getRgbF( &r,&g,&b,&a);
  mStoredColour[i]->r = r;
  mStoredColour[i]->g = g;
  mStoredColour[i]->b = b;
  mStoredColour[i]->a = a;

  changePen();
  emit changed();
}

void  SplineNWidget::pickColour0() {  pickColour(0); }
void  SplineNWidget::pickColour1() {  pickColour(1); }
void  SplineNWidget::pickColour2() {  pickColour(2); }
void  SplineNWidget::pickColour3() {  pickColour(3); }
void  SplineNWidget::pickColour4() {  pickColour(4); }
void  SplineNWidget::pickColour5() {  pickColour(5); }

void SplineNWidget::removePreviousWidgets( const int aPreviousNumColours )
{
  for ( int i = 0; i < aPreviousNumColours; ++i)
  {
    mSelectionLayout2->removeWidget( mButtonPick[i] );
    mButtonPick[i]->hide();
  }

  switch ( aPreviousNumColours )
  {
    case 2:
    case 4: mSelectionLayout1->removeWidget( renderArea[0] );
            renderArea[0]->hide();
            break;
    case 5: mSelectionLayout1->removeWidget( renderArea[1] );
            mSelectionLayout1->removeWidget( renderArea[2] );
            renderArea[1]->hide();
            renderArea[2]->hide();
            break;
    case 6: mSelectionLayout1->removeWidget( renderArea[3] );
            mSelectionLayout1->removeWidget( renderArea[4] );
            mSelectionLayout1->removeWidget( renderArea[5] );
            renderArea[3]->hide();
            renderArea[4]->hide();
            renderArea[5]->hide();
            break;
  };
}

void SplineNWidget::addWidgets( const int aCurrentNumColours )
{
  for ( int i = 0; i < aCurrentNumColours; ++i)
  {
    mSelectionLayout2->addWidget( mButtonPick[i] );
    mButtonPick[i]->show();    
  }

  switch ( aCurrentNumColours )
  {
    case 2:
    case 4: mSelectionLayout1->addWidget( renderArea[0] );
            renderArea[0]->show();
            break;
    case 5: mSelectionLayout1->addWidget( renderArea[1] );
            mSelectionLayout1->addWidget( renderArea[2] );
            renderArea[1]->show();
            renderArea[2]->show();
            break;
    case 6: mSelectionLayout1->addWidget( renderArea[3] );
            mSelectionLayout1->addWidget( renderArea[4] );
            mSelectionLayout1->addWidget( renderArea[5] );
            renderArea[3]->show();
            renderArea[4]->show();
            renderArea[5]->show();
            break;
  };


}

void SplineNWidget::change( int aColorId )
{
  if ( aColorId != mColorId )
  {
    removePreviousWidgets( mNumColor );
    mColorId = aColorId;
    numColours();
    addWidgets( mNumColor );
    emit changed();
    //changePen();
  }
}

void SplineNWidget::numColours()
{
  switch( mColorId )
  {
    case LayerSignal::eColMap_lerp2:   mNumColor = 2; break;
    case LayerSignal::eColMap_spline4: mNumColor = 4; break;
    case LayerSignal::eColMap_spline5: mNumColor = 5; break;
    case LayerSignal::eColMap_spline6: mNumColor = 6; break;
  }
}

void SplineNWidget::colours( std::vector<D3DXCOLOR>& aColours )
{
  for ( int i = 0; i < mNumColor; ++i )
    mStoredColour[i] = &aColours[i];
  changePen();
}

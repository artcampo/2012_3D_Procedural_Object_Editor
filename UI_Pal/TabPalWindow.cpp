#include "TabPalWindow.h"


TabPalWindow::TabPalWindow(QWidget *parent):
  QFrame(parent)
{
  mLayout = new QVBoxLayout (this);
  mLayout->setAlignment( Qt::AlignTop );
  setLayout(mLayout);
  isVisible();
}

TabPalWindow::~TabPalWindow()
{

}




void TabPalWindow::newColour()
{

  int indexLayout = mColLayout.size();
  mColLayout.resize( indexLayout + 1);
  mColLayout[indexLayout] = new QHBoxLayout(this);
  mColLayout[indexLayout]->setSizeConstraint( QLayout::SetMinimumSize );
  mColLayout[indexLayout]->setAlignment (Qt::AlignLeft);
  mLayout->addLayout( mColLayout[indexLayout] );


  int index = mColours.size();
  mColours.resize( index + 1);
  mColours[index] = new ColourWidget(this);
  mColLayout[indexLayout]->addWidget( mColours[index]->mButtonPick );
  mColLayout[indexLayout]->addWidget( mColours[index]->mArea );
}

void TabPalWindow::exportToNpal( QFile& aFile )
{
  QTextStream stream(&aFile);

  //  Export Splines
  stream << mColours.size() << endl;
  for ( unsigned int i = 0; i < mColours.size() ; ++i)
  {
    qreal r,g,b,a;
    mColours[i]->mColour.getRgbF (&r,&g,&b,&a);
    stream << r << endl;
    stream << g << endl;
    stream << b << endl;
    stream << a << endl;
  }

  stream << endl;


  stream.flush();
}

#include "ChannelButton.hpp"

ChannelButton::ChannelButton( QWidget *parent, QString& aName, int aIndex ):
  QPushButton(parent)
{

  mIndex = aIndex;
  setFixedSize( 100,100 );
  setText ( aName );
  connect( this, SIGNAL(clicked()), this, SLOT(clickedSlut()));
}

void ChannelButton::clickedSlut()
{
  emit selectChannel( mIndex );
}


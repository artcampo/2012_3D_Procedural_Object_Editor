#include "ChannelNameButton.hpp"

ChannelNameButton::ChannelNameButton( QWidget *parent, QString& aName ):
  QPushButton(parent)
{
  mName = aName;
  setText ( aName );
  //setFixedWidth( 150 );
  connect( this, SIGNAL(clicked()), this, SLOT(clickedSlut()));
}

void ChannelNameButton::clickedSlut()
{
  bool ok;
  QString item = QInputDialog::getText(this, tr("Changing Channel name"),
                                       tr("Introduce new name:"),
                                       QLineEdit::Normal,
                                       this->text (), &ok);
  if (ok &&
      !item.isEmpty() &&
      this->text() != item
      )
  {
    mName = item;
    setText ( mName );
    emit nameChanged();
  }

}


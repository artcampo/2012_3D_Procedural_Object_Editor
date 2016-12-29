#include "SignalNameButton.hpp"

SignalNameButton::SignalNameButton( QWidget *parent, QString& aName, ProcShader* aProcShader ):
  QPushButton(parent)
{
  mProcShader = aProcShader;
  setText ( aName );
  connect( this, SIGNAL(clicked()), this, SLOT(clickedSlut()));
}

void SignalNameButton::clickedSlut()
{
  bool ok;
  QString item = QInputDialog::getText(this, tr("Changing Signal name"),
                                       tr("Introduce new name:"),
                                       QLineEdit::Normal,
                                       this->text (), &ok);
  if (ok &&
      !item.isEmpty() &&
      this->text() != item &&
      mProcShader->signalNameIsUnique( item.toStdString() )
      )
    this->setText (item);
}


#ifndef SignalNameButton_HPP
#define SignalNameButton_HPP

#include <QPushButton>
#include <QInputDialog>
#include <QString>

#include "Editor_Texture/DataLayer/ProcShader.hpp"

class SignalNameButton : public QPushButton
{

Q_OBJECT

public slots:
  void clickedSlut();

public:
    SignalNameButton(QWidget *parent,
                     QString& aName,
                     ProcShader* aProcShader);
private:
    ProcShader* mProcShader;

};

#endif // SignalNameButton

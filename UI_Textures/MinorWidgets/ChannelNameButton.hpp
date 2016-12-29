#ifndef ChannelNameButton_HPP
#define ChannelNameButton_HPP

#include <QPushButton>
#include <QInputDialog>
#include <QString>

#include "Editor_Texture/DataLayer/ProcShader.hpp"

class ChannelNameButton : public QPushButton
{

Q_OBJECT

public slots:
  void clickedSlut();
signals:
    void nameChanged();

public:
    ChannelNameButton(QWidget *parent,
                     QString& aName);
private:
  QString     mName;


};

#endif // SignalNameButton

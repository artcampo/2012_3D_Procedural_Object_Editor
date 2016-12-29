#ifndef NameButton_HPP
#define NameButton_HPP

#include <QPushButton>
#include <QInputDialog>
#include <QString>

class NameButton : public QPushButton
{

Q_OBJECT

public slots:
  void clickedSlut();


public:
    NameButton(QWidget *parent, QString& aName );
    void setName( QString aName);
private:
  QString     mName;

signals:
    void nameChanged ( QString aName);

};

#endif // NameButton_HPP

#ifndef ChannelButton_HPP
#define ChannelButton_HPP

#include <QPushButton>
#include <QInputDialog>
#include <QString>

class ChannelButton : public QPushButton
{

Q_OBJECT

public slots:
  void clickedSlut();

private:
  int mIndex;

public:
    ChannelButton(QWidget *parent, QString& aName, int aIndex );

signals:
    void selectChannel ( int aChannelId );

};

#endif // ChannelButton_HPP

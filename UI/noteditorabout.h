#ifndef NOTEDITORABOUT_H
#define NOTEDITORABOUT_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtGui>
#include <QPixmap>

class NotEditorAbout : public QDialog
{
    Q_OBJECT
public:
    explicit NotEditorAbout(QWidget *parent = 0);
private:
  QPushButton*  quitButton;
  QImage *       logo;
  QGraphicsPixmapItem* item;
  QGraphicsScene* scene;
  QGraphicsView*  view;
  QGraphicsProxyWidget *proxy;
signals:

public slots:

};

#endif // NOTEDITORABOUT_H

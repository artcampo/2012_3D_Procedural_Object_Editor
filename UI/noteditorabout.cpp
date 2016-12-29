#include "noteditorabout.h"

NotEditorAbout::NotEditorAbout(QWidget *parent) :
    QDialog(parent)
{

   quitButton = new QPushButton ("Quit");

   connect(quitButton, SIGNAL(clicked()), this, SLOT(reject()));
   QVBoxLayout layout;

   setLayout(&layout);

   QString path;
   std::wstring w;
   path=  QCoreApplication::applicationDirPath();
   path.append("/Assets/img/about.png");
   logo = new QImage(path);

   item = new QGraphicsPixmapItem ( QPixmap::fromImage(*logo));
   scene = new QGraphicsScene();
   scene->addItem(item);

   view = new QGraphicsView(scene);

   setFixedSize(200,200);

   layout.addWidget(view);
   //layout.addWidget(quitButton);


   //this->layout()->setSizeConstraint( QLayout::SetFixedSize );

   //view->show();

}


#include <QtGui>

#include "UI/MainWindow.hpp"


#if defined(WIN32) && defined(_DEBUG)
   #define _CRTDBG_MAP_ALLOC
   #include <stdlib.h>
   #include <crtdbg.h>
   #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
   #define new DEBUG_NEW
#endif

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Object Editor");

    w.showMaximized();
    //w.resize(200,200);
    w.initAllStuff();

    a.setQuitOnLastWindowClosed(true);
    int code = a.exec();

   // _CrtDumpMemoryLeaks();

    return code;
}

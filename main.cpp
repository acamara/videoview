//Implementació del main

#include <QtGui/QApplication>
#include "mainwindow.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>  // for XInitThreads() call
#endif

int main(int _argc, char* _argv[])
{

#ifdef Q_WS_X11
    //Això ha de ser el primer a l'aplicació per fer crides al fil Xlib necessaris per guardar els fils OpenGL de renderitzat
    XInitThreads();
#endif
    // Inicialització de l'aplicació Qt
    QApplication app(_argc, _argv);

    // Sortir si el sistema no suporta OpenGL
    if (!QGLFormat::hasOpenGL())
        qFatal("Aquest sistema no suporta OpenGL");

    MainWindow w;
    w.show();

    //Iniciem el main event loop
    return app.exec();
}

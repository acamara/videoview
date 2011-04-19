//Implementació del main

#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int _argc, char* _argv[])
{

    // Inicialització de l'aplicació Qt
    QApplication app(_argc, _argv);

    MainWindow w;
    w.show();

    //Iniciem el main event loop
    return app.exec();
}

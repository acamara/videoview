//Implementaci� del main

#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int _argc, char* _argv[])
{

    // Inicialitzaci� de l'aplicaci� Qt
    QApplication app(_argc, _argv);

    MainWindow w;
    w.show();

    //Iniciem el main event loop
    return app.exec();
}

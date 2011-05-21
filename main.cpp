//Implementació del main

#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"


int main(int _argc, char* _argv[])
{
    //Per solucionar els problemes de accents, forcem la codificació.
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);

    // Inicialització de l'aplicació Qt
    QApplication app(_argc, _argv);

    MainWindow w;
    w.show();

    //Iniciem el main event loop
    return app.exec();
}

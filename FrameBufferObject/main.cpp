#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w(0);
    w.resize(640,480);
    w.show();
    return a.exec();
}

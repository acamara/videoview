//Implementaci� de la classe PGMWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "pgmwidget.h"
#include "GravarThread.h"

//Constructor de la classe PGMWidget
PGMWidget::PGMWidget(QWidget *parent)
    : QGLWidget(parent),
    glt(this)

{
    setFormat(QGLFormat(QGL::DoubleBuffer));

    //L'intercanvi de mem�ria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    // Inici del fil de gravaci�
    initRendering();
}

//M�tode de la classe PGMWidget que inicia el renderitzat
void PGMWidget::initRendering()
{
    // Inici del fil de gravaci�
    glt.start();
}

//M�tode de la classe PGMWidget que inicia l'adquisici�
void PGMWidget::initgravar()
{
    // Inici del fil de renderitzat
    glt.setgravar(true);
}

//M�tode de la classe PGMWidget que finalitza el renderitzat
void PGMWidget::finishRendering( )
{
    // Petici� de parar el fil de renderitzat
    glt.stop();
    // wait till the thread has exited
    glt.wait();
}

//M�tode de la classe PGMWidget que controla els events de sortida
void PGMWidget::closeEvent( QCloseEvent * event )
{
    // request stopping
    finishRendering();
    // close the widget (base class)
    QGLWidget::closeEvent(event);
}

//M�tode de la classe PGMWidget que controla el event de redimensionat
void PGMWidget::resizeEvent( QResizeEvent * event )
{
    // signal the gravaci� thread that a resize is needed
    glt.resizeViewport(event->size());
}

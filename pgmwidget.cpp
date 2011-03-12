//Implementació de la classe PGMWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "pgmwidget.h"
#include "GravarThread.h"

//Constructor de la classe PGMWidget
PGMWidget::PGMWidget(QWidget *parent)
    : QGLWidget(parent),pglt(0)

{
    setFormat(QGLFormat(QGL::DoubleBuffer));
}

//Mètode de la classe PGMWidget que inicia el renderitzat
void PGMWidget::initRendering()
{
    // Inici del fil de renderitzat
    pglt=new GravarThread(this);
    setAutoBufferSwap(false);
    pglt->start();
}

//Mètode de la classe PGMWidget que inicia la gravació
void PGMWidget::initPGM(QSize resolucio, double fps)
{
    if(pglt){
        finishRendering();
    }
    initRendering();
    pglt->setconfig(resolucio,fps);
}

//Mètode de la classe PGMWidget que finalitza el renderitzat
void PGMWidget::finishRendering( )
{
    if(pglt){
    // Petició de parar el fil de renderitzat
    pglt->stop();
    // wait till the thread has exited
    pglt->wait();
    setAutoBufferSwap(true);
    delete pglt;
    pglt=0;
    }
}

//Mètode de la classe PGMWidget que finalitza la gravació
void PGMWidget::finishGravar( )
{
    pglt->tancavideo();
    pglt->setgravar(false);
}

//Mètode de la classe PGMWidget que controla els events de sortida
void PGMWidget::closeEvent( QCloseEvent * event )
{
    // request stopping
    finishRendering();
    // close the widget (base class)
    QGLWidget::closeEvent(event);
}

//Mètode de la classe PGMWidget que controla el event de redimensionat
void PGMWidget::resizeEvent( QResizeEvent * event )
{
    // signal the gravació thread that a resize is needed
    if(pglt){
        pglt->resizeViewport(event->size());
    }
}

void PGMWidget::paintEvent(QPaintEvent *) {
    if (pglt == 0) {
      //qDebug() << "Repaint " << objectName() << "!";
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      updateGL();
    }
}

//Implementació de la classe PGMWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "pgmwidget.h"
#include "GravarThread.h"

//Constructor de la classe PGMWidget
PGMWidget::PGMWidget(QWidget *parent)
    : QGLWidget(parent),pthreadgravar(0)
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
}

//Mètode de la classe PGMWidget que inicia el renderitzat
void PGMWidget::initRendering()
{
    // Inici del fil de renderitzat
    pthreadgravar=new GravarThread(this);
    setAutoBufferSwap(false);
    pthreadgravar->start();
}

//Mètode de la classe PGMWidget que inicia la gravació
void PGMWidget::initPGM(QSize resolucio, double fps,int duradetransicio)
{
    if(pthreadgravar){
        finishRendering();
    }
    initRendering();
    pthreadgravar->setconfig(resolucio,fps,duradetransicio);
}

//Mètode de la classe PGMWidget que finalitza el renderitzat
void PGMWidget::finishRendering( )
{
    if(pthreadgravar){
        // Petició de parar el fil de renderitzat
        pthreadgravar->stop();
        // wait till the thread has exited
        pthreadgravar->wait();
        setAutoBufferSwap(true);
        delete pthreadgravar;
        pthreadgravar=0;
    }
}

//Mètode de la classe PGMWidget que finalitza la gravació
void PGMWidget::finishGravar( )
{
     if(pthreadgravar){
        pthreadgravar->setgravar(false);
    }
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
    if(pthreadgravar){
        pthreadgravar->resizeViewport(event->size());
    }
}

void PGMWidget::paintEvent(QPaintEvent *) {
    if(pthreadgravar==0){
        updateGL();
    }
}

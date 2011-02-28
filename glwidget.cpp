//Implementació de la classe GLWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "glwidget.h"
#include "renderthread.h"

//Constructor de la classe GLWidget
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
    glt(this)

{
    setFormat(QGLFormat(QGL::DoubleBuffer));

    //L'intercanvi de memòria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    // Inici del fil de renderitzat
    initRendering();
}

//Mètode de la classe GLWidget que inicia el renderitzat
void GLWidget::initRendering()
{
    // Inici del fil de renderitzat
    glt.start();
}

//Mètode de la classe GLWidget que inicia l'adquisició
void GLWidget::initadquirir(CvCapture *capture, QString cam)
{
    // Inici del fil de renderitzat
    glt.selecfontvideo(capture);
    glt.seleccam(cam);
    glt.setadquirir(true);
}

//Mètode de la classe GLWidget que finalitza el renderitzat
void GLWidget::finishRendering( )
{
    // Petició de parar el fil de renderitzat
    glt.stop();
    // wait till the thread has exited
    glt.wait();
}

//Mètode de la classe GLWidget que controla els events de sortida
void GLWidget::closeEvent( QCloseEvent * event )
{
    // request stopping
    finishRendering();
    // close the widget (base class)
    QGLWidget::closeEvent(event);
}

//Mètode de la classe GLWidget que controla el event de redimensionat
void GLWidget::resizeEvent( QResizeEvent * event )
{
    // signal the rendering thread that a resize is needed
    glt.resizeViewport(event->size());
}

//Mètode que controla si s'ha clicat el widget opengl, per fer el canvi de càmera
void GLWidget::mousePressEvent(QMouseEvent *event)
{
 emit widgetClicked();
 QWidget::mousePressEvent(event);
}

void GLWidget::canviacamactiva(QString cam)
{
    glt.camaraactiva=cam;
    //qDebug()<<glt.camaraactiva;
}

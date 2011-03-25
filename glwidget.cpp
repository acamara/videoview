//Implementació de la classe GLWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "glwidget.h"
#include "renderthread.h"

//Constructor de la classe GLWidget
GLWidget::GLWidget(PGMWidget* pgm, QWidget *parent)
    : QGLWidget(parent), pthreadrender(0), pWPGM(pgm)
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
}

//Mètode de la classe GLWidget que inicia el renderitzat
void GLWidget::initRendering()
{
    // Inici del fil de renderitzat
    pthreadrender = new RenderThread(this);
    pthreadrender->resizeViewport(size());

    //L'intercanvi de memòria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    pthreadrender->start();
}

//Mètode de la classe GLWidget que inicia l'adquisició
void GLWidget::initadquirir(CvCapture *capture, QString cam)
{
    if (pthreadrender) {
       finishRendering();
    }
    initRendering();
    // Inici del fil de renderitzat
    pthreadrender->selecfontvideo(capture);
    pthreadrender->seleccam(cam);
    pthreadrender->setadquirir(true);
}

//Mètode de la classe GLWidget que finalitza el renderitzat
void GLWidget::finishRendering()
{
  if (pthreadrender) {
    // Petició de parar el fil de renderitzat
    pthreadrender->stop();
    // wait till the thread has exited
    pthreadrender->wait();
    setAutoBufferSwap(true);
    delete pthreadrender;
    pthreadrender = 0;
  }
}

void GLWidget::paintEvent(QPaintEvent *) {
  if (pthreadrender == 0) {
    updateGL();
  }
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
    if (pthreadrender) {
     pthreadrender->resizeViewport(event->size());
    }
}

//Mètode que controla si s'ha clicat el widget opengl, per fer el canvi de càmera
void GLWidget::mousePressEvent(QMouseEvent *event)
{
 emit widgetClicked();
 QWidget::mousePressEvent(event);
}

void GLWidget::canviacamactiva(QString cam)
{
  if (pthreadrender) {
    pthreadrender->camaraactiva=cam;
    }
}

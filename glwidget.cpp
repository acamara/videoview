//Implementació de la classe GLWidget (widget OpenGL)

#include <qevent.h>
#include <qdebug.h>

#include "glwidget.h"
#include "renderthread.h"

//Constructor de la classe GLWidget
GLWidget::GLWidget(PGMWidget* pgm, QWidget *parent)
    : QGLWidget(parent), pglt(0), pPGM(pgm)

{
    setFormat(QGLFormat(QGL::DoubleBuffer));
}

//Mètode de la classe GLWidget que inicia el renderitzat
void GLWidget::initRendering()
{
    // Inici del fil de renderitzat
    pglt = new RenderThread(this);

    //L'intercanvi de memòria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    pglt->start();
}

//Mètode de la classe GLWidget que inicia l'adquisició
void GLWidget::initadquirir(CvCapture *capture, QString cam)
{
    if (pglt) {
       finishRendering();
    }
    initRendering();
    // Inici del fil de renderitzat
    pglt->selecfontvideo(capture);
    pglt->seleccam(cam);
    pglt->setadquirir(true);
}

//Mètode de la classe GLWidget que finalitza el renderitzat
void GLWidget::finishRendering()
{
  if (pglt) {
    // Petició de parar el fil de renderitzat
    pglt->stop();
    // wait till the thread has exited
    pglt->wait();
    setAutoBufferSwap(true);
    delete pglt;
    pglt = 0;
  }
}

void GLWidget::paintEvent(QPaintEvent *) {
  if (pglt == 0) {
    qDebug() << "Repaint " << objectName() << "!";
    // makeCurrent();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
    if (pglt) {
       pglt->resizeViewport(event->size());
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
  if (pglt) {
    pglt->camaraactiva=cam;
  }
  //qDebug()<<pglt->camaraactiva;
}

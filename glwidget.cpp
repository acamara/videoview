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
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    //L'intercanvi de memòria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    // Inici del fil de renderitzat
    //initRendering();
}

//Mètode de la classe GLWidget que inicia el renderitzat
void GLWidget::initRendering(CvCapture *capture, QString cam)
{
    // Inici del fil de renderitzat
    glt.selecfontvideo(capture);
    glt.seleccam(cam);
    glt.start();
    QObject::connect(&glt, SIGNAL(finished()),this, SLOT(finalitzat()));

    // wake the waiting render thread
    renderCondition().wakeAll();
}

//Mètode de la classe GLWidget que finalitza el renderitzat
void GLWidget::finishRendering( )
{
    // Petició de parar el fil de renderitzat
    glt.stop();
    // wake up render thread to actually perform stopping
    renderCondition().wakeAll();
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

//Mètode de la classe GLWidget que controla el event de Pintat
void GLWidget::paintEvent( QPaintEvent * )
{
    render();
}

//Mètode de la classe GLWidget que controla el event de redimensionat
void GLWidget::resizeEvent( QResizeEvent * event )
{
    // signal the rendering thread that a resize is needed
    glt.resizeViewport(event->size());

    render();
}

//Mètode de la classe GLWidget que bloqueja el context OpenGL
void GLWidget::lockGLContext( )
{
    // lock the render mutex for the calling thread
    renderMutex().lock();
    // make the render context current for the calling thread
    makeCurrent();
}

//Mètode de la classe GLWidget que desbloqueja el context OpenGL
void GLWidget::unlockGLContext( )
{
    // release the render context for the calling thread
    // to make it available for other threads
    doneCurrent();
    // unlock the render mutex for the calling thread
    renderMutex().unlock();
}

//Mètode de la classe GLWidget que controla el renderitzat
void GLWidget::render( )
{
    // let the wait condition wake up the waiting thread
    renderCondition().wakeAll();
}

//Mètode que controla si s'ha clicat el widget opengl, per fer el canvi de càmera
void GLWidget::mousePressEvent(QMouseEvent *event)
{
 emit widgetClicked();
 QWidget::mousePressEvent(event);
}

//Métode de test "S'HA D'ELIMINAR" només serveix per conèixer si s'ha acabat el loop de renderitat.
void GLWidget::finalitzat( )
{
    qDebug()<<"S'ha acabat el loop run";
}

//Mètode de la classe GLWidget que controla la condició de renderitzat
QWaitCondition & GLWidget::renderCondition( )
{
    return(render_condition);
}

//Mètode de la classe GLWidget que controla el render mutex
QMutex & GLWidget::renderMutex( )
{
    return(render_mutex);
}

//Mètode que controla si s'ha clicat el widget opengl, i realitza el canvi de càmera
void GLWidget::canviacamactiva()
{
    glt.camaraactiva=sender()->objectName();
    //qDebug()<<glt.camaraactiva;
}

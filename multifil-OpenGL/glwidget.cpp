//Implementaci� de la classe GLWidget (widget OpenGL)

#include <qevent.h>

#include "glwidget.h"
#include "renderthread.h"

//Constructor de la classe GLWidget
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
    glt(*this)

{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    //L'intercanvi de mem�ria es controla en el fil de renderitzat
    setAutoBufferSwap(false);

    // Inici del fil de renderitzat
    initRendering();
}

//M�tode de la classe GLWidget que inicia el renderitzat
void GLWidget::initRendering( )
{
    // Inici del fil de renderitzat
    glt.start();
    // wake the waiting render thread
    renderCondition().wakeAll();
}

//M�tode de la classe GLWidget que finalitza el renderitzat
void GLWidget::finishRendering( )
{
    // Petici� de parar el fil de renderitzat
    glt.stop();
    // wake up render thread to actually perform stopping
    renderCondition().wakeAll();
    // wait till the thread has exited
    glt.wait();
}

//M�tode de la classe GLWidget que controla els events de sortida
void GLWidget::closeEvent( QCloseEvent * _e )
{
    // request stopping
    finishRendering();
    // close the widget (base class)
    QGLWidget::closeEvent(_e);
}

//M�tode de la classe GLWidget que controla el event de Pintat
void GLWidget::paintEvent( QPaintEvent * )
{
    render();
}

//M�tode de la classe GLWidget que controla el event de redimensionat
void GLWidget::resizeEvent( QResizeEvent * _e )
{
    // signal the rendering thread that a resize is needed
    glt.resizeViewport(_e->size());

    render();
}

//M�tode de la classe GLWidget que bloqueja el context OpenGL
void GLWidget::lockGLContext( )
{
    // lock the render mutex for the calling thread
    renderMutex().lock();
    // make the render context current for the calling thread
    makeCurrent();
}

//M�tode de la classe GLWidget que desbloqueja el context OpenGL
void GLWidget::unlockGLContext( )
{
    // release the render context for the calling thread
    // to make it available for other threads
    doneCurrent();
    // unlock the render mutex for the calling thread
    renderMutex().unlock();
}

//M�tode de la classe GLWidget que controla el renderitzat
void GLWidget::render( )
{
    // let the wait condition wake up the waiting thread
    renderCondition().wakeAll();
}

//M�tode de la classe GLWidget que controla la condici� de renderitzat
QWaitCondition & GLWidget::renderCondition( )
{
    return(render_condition);
}

//M�tode de la classe GLWidget que controla el render mutex
QMutex & GLWidget::renderMutex( )
{
    return(render_mutex);
}




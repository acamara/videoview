//Implementació de la classe RenderThread

#include "renderthread.h"
#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

//Constructor de la classe RenderThread
RenderThread::RenderThread( GLWidget & _glw )
        : QThread(),
        glw(_glw),
        render_flag(true),
        resize_flag(false),
        viewport_size(_glw.size())
{

}

//Mètode de la classe RenderThread que activa el flag de redimensionat de la finestra
void RenderThread::resizeViewport( const QSize& _size )
{
    // Selecciona la mida del visor i activa el canvi de mida
    viewport_size = _size;
    resize_flag = true;
}

//Mètode de la classe RenderThread que activa el flag per parar el renderitzat
void RenderThread::stop( )
{
    // S'activa el flag per indicar que es vol sortir del thread
    // RECORDAR: El fil ha de ser cridat un cop, després de cridar a aquest mètode per sortir realment!
    render_flag = false;
}

//Mètode de la classe RenderThread que
void RenderThread::run( )
{
    // Bloqueja el render mutex del GlWidget i crea el context de renderitzat del GLWidget actual en aquest fil
    glw.lockGLContext();

    // Inicialització OpenGL.
    initializeGL();

    // Realitza aquestb procés mentre el flag de renderitzat estigui actiu
    while( render_flag )
    {
        // Comprovació de si es necesita redimensionar el GLWidget
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }

        // El codi de renderitzat va aquí
        paintGL();

        // Intercanvi dels buffers del GLWidget
        glw.swapBuffers();

        glw.doneCurrent(); // Alliberar el context de renderitzat OpenGL per seleccionar el treball!

        // wait until the gl widget says that there is something to render
        // glwidget.lockGlContext() had to be called before (see top of the function)!
        // this will release the render mutex until the wait condition is met
        // and will lock the render mutex again before exiting
        // waiting this way instead of insane looping will not waste any CPU ressources

        glw.renderCondition().wait(&glw.renderMutex());

        glw.makeCurrent(); // get the GL render context back

        // DEACTIVATED -- alternatively render a frame after a certain amount of time
        //prevent to much continous rendering activity
        // msleep(16); //sleep for 16 ms
    }
    // Desbloqejar el render mutex abans de sortir
    glw.unlockGLContext();
}

//Mètode de la classe RenderThread que inicialitza el GLWidget
void RenderThread::initializeGL()
{
    // Inicialització típica d'OpenGL
    glw.qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//Mètode de la classe RenderThread que redimensiona el GLWidget
void RenderThread::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat)width / height;
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

//Mètode de la classe RenderThread que pinta en el GLWidget
void RenderThread::paintGL()
{ 
    // Dibuixa el frame de "CAM"
    glClear (GL_COLOR_BUFFER_BIT);      //Netegem el buffer de color
    glClearColor (0.0,0.0,0.0,1.0);     //Activa el color de fons actual,seguint el model de representació RGB
    glMatrixMode(GL_PROJECTION);        //Seleccionem la matriu de projecció
    glLoadIdentity();                   //La inicialtzem com a matriu identitat
    gluOrtho2D(0,1,0,1);                //Creem una matriu per projectar coordenades en dues dimensions en la pantalla i la multipliquem per la matriu actual.

    IplImage *img=cvLoadImage("cam1.jpg");

    qframe = QImage((const unsigned char*)(img->imageData), img->width, img->height, img->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe = QGLWidget::convertToGLFormat(qframe);

    if (!qframe.isNull()) {
        qframe = qframe.scaled(viewport_size, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        glDrawPixels(qframe.width(),qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits());
    }
}


//Implementació de la classe RenderThread

#include <QDebug>
#include "renderthread.h"
#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

//Constructor de la classe RenderThread
RenderThread::RenderThread( GLWidget &_glw )
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

    // Realitza aquest procés mentre el flag de renderitzat estigui actiu
    while(render_flag )
    {
        // Comprovació de si es necesita redimensionar el GLWidget
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }

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
        msleep(40); //sleep for 40 ms
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
        glClear (GL_COLOR_BUFFER_BIT);
        glClearColor (0.0,0.0,0.0,1.0);

        if (!qframe.isNull()) {
            qframe = qframe.scaled(viewport_size, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,1,0,1);

            glEnable(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );

            glBegin(GL_QUADS);
                glTexCoord2f(0,0); glVertex2f(0,0);
                glTexCoord2f(1,0); glVertex2f(1,0);
                glTexCoord2f(1,1); glVertex2f(1,1);
                glTexCoord2f(0,1); glVertex2f(0,1);
            glEnd();

            glDisable(GL_TEXTURE_2D);

            glFlush();
        }
        //qDebug() << "Drawing...";
}

void RenderThread::sendImage(IplImage *img) {
    //La imatge es guarda utilitzant 24-bit RGB(8-8-8).
    qframe = QImage((const unsigned char*)(img->imageData), img->width, img->height, img->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe = QGLWidget::convertToGLFormat(qframe);

    glw.render();
}

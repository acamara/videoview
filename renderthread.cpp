//Implementació de la classe RenderThread

#include <QDebug>
#include <QFileDialog>
#include "renderthread.h"
#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

//Constructor de la classe RenderThread
RenderThread::RenderThread( GLWidget *_glw )
        : QThread(),
        glw(_glw)
{
    render_flag=true;
    adquirir=false;
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

}

//Mètode de la classe RenderThread que
void RenderThread::run( )
{
    glw->makeCurrent();

    // Realitza aquest procés mentre el flag de renderitzat estigui actiu
    while(render_flag )
    {
        // Comprovació de si es necesita redimensionar el GLWidget
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }

        if (adquirir)
        {
            processCam();
            paintGL();
        }

        // Intercanvi dels buffers del GLWidget
        glw->swapBuffers();

        //-----------------------------------------------------------------
        //No hi hauria de ser aquest makeCurrent però si l'actives funciona.
        glw->makeCurrent();
        //-----------------------------------------------------------------

        msleep(40); //sleep for 40 ms
    }

}

//Mètode de la classe RenderThread que redimensiona el GLWidget
void RenderThread::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void RenderThread::processCam() {
    frame = cvQueryFrame(capture);
    cvCvtColor(frame,frame,CV_BGR2RGB);

    if(camaraactiva==camera){
        enviaragravar(frame);
    }
}

//Métode de la classe RenderThread que pinta en el GLWidget
void RenderThread::paintGL()
{
        glClear (GL_COLOR_BUFFER_BIT);
        glClearColor (0.0,0.0,0.0,1.0);

        if (frame->imageData) {
            
            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,1,0,1);

            glEnable(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                          frame->width, frame->height,
                          0, GL_RGB, GL_UNSIGNED_BYTE, frame->imageData);

            glBegin(GL_QUADS);
                glTexCoord2f(0,1); glVertex2f(0,0);
                glTexCoord2f(1,1); glVertex2f(1,0);
                glTexCoord2f(1,0); glVertex2f(1,1);
                glTexCoord2f(0,0); glVertex2f(0,1);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
}


void RenderThread::selecfontvideo(CvCapture *_capture)
{
    capture=_capture;
}

void RenderThread::seleccam(QString cam)
{
    camera=cam;
}

void RenderThread::setadquirir(bool _adquirir){
    adquirir=_adquirir;
}

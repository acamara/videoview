//Implementaci� de la classe GravarThread.

#include <QtGui>
#include "GravarThread.h"
#include "pgmwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


//Constructor de la classe GravarThread
GravarThread::GravarThread(PGMWidget *_glw )
        : QThread(),
        glw(_glw)
{
    render_flag=true;
    resize_flag=false;
    gravar=false;
    mostrar=false;
}

//M�tode de la classe GravarThread que activa el flag de redimensionat de la finestra
void GravarThread::resizeViewport( const QSize& _size )
{
    // Selecciona la mida del visor i activa el canvi de mida
    viewport_size = _size;
    resize_flag = true;
}

//M�tode que realitza la gravaci� i el processat dels frames de sortida PGM
void GravarThread::run()
{
    video=cvCreateVideoWriter("sortida.avi",0,fps,cvSize(resolucio.width(),resolucio.height()),1);
    glw->makeCurrent();

    // Realitza aquest proc�s mentre el flag de renderitzat estigui actiu
    while(render_flag )
    {
        // Comprovaci� de si es necesita redimensionar el GLWidget
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }

        if (mostrar)
        {
            paintGL();
        }

        if (gravar)
        {
            //Aqu� ha d'anar el codi per gravar a fitxer.
            //Escalat del v�deo de sortida
            if(frame->width!=resolucio.width()|| frame->height!=resolucio.height()) {
               IplImage *sal= cvCreateImage(cvSize(resolucio.width(),resolucio.height()),IPL_DEPTH_8U,3);
               cvResize(frame, sal);
               cvWriteFrame(video, sal);
               cvReleaseImage(&sal);
            } else {
              cvWriteFrame(video,frame);
            }
        }

        // Intercanvi dels buffers del GLWidget
        glw->swapBuffers();

        msleep(1000.0/fps); //sleep for 40 ms
    }
    tancavideo();
}

//M�tode de la classe GravarThread que pinta en el GLWidget
void GravarThread::paintGL()
{
        glClear (GL_COLOR_BUFFER_BIT);
        glClearColor (0.0,0.0,0.0,1.0);

        if (frame) {

            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,1,0,1);

            glEnable(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                          frame->width, frame->height,
                          0, GL_BGR, GL_UNSIGNED_BYTE, frame->imageData);

            glBegin(GL_QUADS);
                glTexCoord2f(0,1); glVertex2f(0,0);
                glTexCoord2f(1,1); glVertex2f(1,0);
                glTexCoord2f(1,0); glVertex2f(1,1);
                glTexCoord2f(0,0); glVertex2f(0,1);
            glEnd();
            glDisable(GL_TEXTURE_2D);

        }
}

//M�tode de la classe GravarThread que activa el flag per parar el renderitzat
void GravarThread::stop( )
{
    render_flag=false;
}

//M�tode de la classe GravarThread que redimensiona el GLWidget
void GravarThread::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

//M�tode que estableix el tipus de transici�
void GravarThread::selectransicio(int tipusdetransicio)
{
    tipustransicio=tipusdetransicio;
    //qDebug()<<tipustransicio;
}

//M�tode que estableix la durada de la transici�
void GravarThread::selecduratransicio(int duradetransicio)
{
    duradatransicio=duradetransicio;
}

void GravarThread::rebregravar(IplImage *_frame)
{
    frame=_frame;
    mostrar=true;
    //cvNamedWindow("pvw",0);
    //cvShowImage("pvw",frame);
    //qDebug()<<"Rebut per gravar";
}

void GravarThread::setgravar(bool _gravar){
    gravar=_gravar;
}

void GravarThread::tancavideo(){
    if(video){
        cvReleaseVideoWriter (&video);
    }
}

void GravarThread::setconfig(QSize _resolucio,double _fps ){
    resolucio=_resolucio;
    fps=_fps;
}

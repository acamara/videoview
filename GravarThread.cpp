//Implementació de la classe GravarThread.

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

//Mètode de la classe GravarThread que activa el flag de redimensionat de la finestra
void GravarThread::resizeViewport( const QSize& _size )
{
    // Selecciona la mida del visor i activa el canvi de mida
    viewport_size = _size;
    resize_flag = true;
}

//Mètode que realitza la gravació i el processat dels frames de sortida PGM
void GravarThread::run()
{
    video=cvCreateVideoWriter("sortida.avi",0,fps,cvSize(resolucio.width(),resolucio.height()),1);
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

        if (mostrar)
        {
            paintGL();
        }

        if (gravar)
        {
            //Aquí ha d'anar el codi per gravar a fitxer.
            //Escalat del vídeo de sortida
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

//Métode de la classe GravarThread que pinta en el GLWidget
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

//Mètode de la classe GravarThread que activa el flag per parar el renderitzat
void GravarThread::stop( )
{
    render_flag=false;
}

//Mètode de la classe GravarThread que redimensiona el GLWidget
void GravarThread::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

//Mètode que estableix el tipus de transició
void GravarThread::selectransicio(int tipusdetransicio)
{
    tipustransicio=tipusdetransicio;
    //qDebug()<<tipustransicio;
}

//Mètode que estableix la durada de la transició
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

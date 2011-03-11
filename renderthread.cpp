//Implementació de la classe RenderThread

#include <QDebug>
#include <QFileDialog>
#include "renderthread.h"
#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

//-----------------------------------------------------------
//Instruccions de comprovació s'han de treure en el programa final
//-----------------------------------------------------------
#include <ctime>

class Cronometre {
  clock_t t1, t2;
public:
  void begin() { t1 = clock(); }
  void end()   { t2 = clock(); }
  double elapsed() const {
    return 1000.0 * double(t2 - t1) / CLOCKS_PER_SEC;
  }
};

//-----------------------------------------------------------

//Constructor de la classe RenderThread
RenderThread::RenderThread( GLWidget *_glw )
        : QThread(),
        glw(_glw)
{
    render_flag=true;
    adquirir=false;
    primer_frame=true;
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
    render_flag=false;
}

//Mètode de la classe RenderThread que
void RenderThread::run( )
{
    glw->makeCurrent();
    
    // Realitza aquest procés mentre el flag de renderitzat estigui actiu
    while(render_flag )
    {
        Cronometre C;
        //-----------------------------------------------------------------
        C.begin();
        //-----------------------------------------------------------------

        // Comprovació de si es necesita redimensionar el GLWidget
        if (resize_flag)
        {
            resizeGL(viewport_size.width(), viewport_size.height());
            resize_flag = false;
        }

        if (adquirir && capture)
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
        C.end();
        double elapsed = C.elapsed();

        static int N = 0;
        if (++N <= 10) {
          qDebug() << C.elapsed();
          N = 0;
        }

        if (elapsed < frame_time) {
          // Dormim fins que toqui posar el seguent frame
          msleep(frame_time - elapsed);
        } else {
          qDebug() << "Vaig just!";
        }

        //-----------------------------------------------------------------
        MostraTempsQT(camera[4].digitValue(), C.elapsed());
        //-----------------------------------------------------------------
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
    Cronometre C;
    //-----------------------------------------------------------------
    C.begin();
    //-----------------------------------------------------------------

    frame = cvQueryFrame(capture);

    //-----------------------------------------------------------------
    C.end();
    MostraTempscvQuery(camera[4].digitValue(), C.elapsed());
    //-----------------------------------------------------------------

    if(camaraactiva==camera){
        enviaragravar(frame);
    }
}

//Métode de la classe RenderThread que pinta en el GLWidget
void RenderThread::paintGL()
{
        glClear (GL_COLOR_BUFFER_BIT);

        if (frame) {
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,1,0,1);

            glEnable(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            if(primer_frame){
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                          frame->width, frame->height,
                          0, GL_BGR, GL_UNSIGNED_BYTE, frame->imageData);
                primer_frame=false;
            }
            else{
                glTexSubImage2D(GL_TEXTURE_2D,0,0,0,
                                frame->width,frame->height,
                                GL_BGR,GL_UNSIGNED_BYTE,frame->imageData);
            }

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
    frame_time = 1000.0 / cvGetCaptureProperty(_capture, CV_CAP_PROP_FPS);
}

void RenderThread::seleccam(QString cam)
{
    camera=cam;
}

void RenderThread::setadquirir(bool _adquirir){
    adquirir=_adquirir;
}

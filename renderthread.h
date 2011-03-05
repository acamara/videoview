//Declaració de la classe RenderThread

#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <qgl.h>
#include <qthread.h>
#include <qsize.h>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

class GLWidget;

class RenderThread : public QThread
{
    Q_OBJECT
public:
    /** Init an OpenGl render thread for the _glwidget QGL */
    RenderThread(GLWidget *_glw);

    /** main() function of the thread. */
    void run();

    /** Signal the thread to exit the next time it is woken up. */
    void stop();

    /** Request a resize of the GL viewport. This is usually called from the QGLWidgets resizeEvent() method.*/
    void resizeViewport(const QSize& _size);

    void selecfontvideo(CvCapture *_capture);
    void seleccam(QString cam);
    void setadquirir(bool _adquirir);
    QString camaraactiva;

signals:
    void enviaragravar(IplImage *frame);
    //-----------------------------------------------------------
    //Mètodes de comprovació s'han de treure en el programa final
    //-----------------------------------------------------------
    void MostraTempsQT(int cam,double time);
    void MostraTempscvQuery(int cam,double time);
    //-----------------------------------------------------------

protected:
    /** Handles resizes of the GL viewport. */
    void resizeGL(int width, int height);
    void processCam();
    void paintGL();

private:
    GLWidget *glw;
    bool render_flag;
    bool resize_flag;
    bool adquirir;
    QSize viewport_size;
    QString camera;
    IplImage *frame;
    CvCapture *capture;
};


#endif // RENDERTHREAD_H

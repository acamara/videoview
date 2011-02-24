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

    QString camaraactiva;

signals:
    void enviaragravar(IplImage *frame);

protected:
    /** Init the GL environment. */
    void initializeGL();

    /** Handles resizes of the GL viewport. */
    void resizeGL(int width, int height);

    /** Does all the painting. */
    void paintGL();

    void processCam();
    void startCam();


private:
     /** The QGLWidget of the render thread. This widget provides the GL rendering context.*/
    GLWidget *glw;

    /** Keep the thread running as long this flag is true. */
    volatile bool render_flag;

    /** Perform a resize when this flag is true. */
    volatile bool resize_flag;

    /** Current size of the viewport. */
    QSize viewport_size;

    QImage qframe;
    IplImage *frame;
    CvCapture *capture;

    QString camera;
};


#endif // RENDERTHREAD_H

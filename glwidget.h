//Declaració de la classe GLWidget (widget OpenGL)

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "renderthread.h"
#include <qgl.h>
#include <qmutex.h>
#include <qwaitcondition.h>

/** Subclasse QGLWidget amb subprocés de representació per separat i capacitats de selecció GL.
 *
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    /** Starts the rendering thread.
     * This method is called in the contructor.
     */
    void initRendering(CvCapture *capture,QString cam);
    /** Stops the rendering thread of the widget. */
    void finishRendering();

     /** Call this method before doing any OpenGL rendering from a thread.
     * This method will aquire the GL rendering context for the calling thread.
     * Rendering will only be possible from this thread until unlockGLContext()
     * is called from the same thread.
     */
    void lockGLContext();
    /** Call this method to release the rendering context again after calling lockGLContext().
     */
    void unlockGLContext();
    /** Returns a reference to the render wait condition.
     * This is only for internal purpose (render thread communication)
     */
    QWaitCondition& renderCondition();
    /** Returns a reference to the render context mutex.
     * This is only for internal purpose (render thread communication)
     */
    QMutex& renderMutex();
    /** The rendering thread. */
    RenderThread glt;

signals:
    void widgetClicked();

public slots:
    /** Cause the rendering thread to render one frame of the OpenGL scene.
     * This method is thread save.
     * \warning If the rendering thread is currently rendering (not idle) when this method is called
     * NO additional new frame will be rendered afterwards!
     */
    void render();
    void finalitzat();
    void canviacamactiva();

protected:
    /** Performs a save shutdown if the widget recieves a close event. */
    void closeEvent(QCloseEvent* _e);
    /** Calls render() if the widget recieves a paint event. */
    void paintEvent(QPaintEvent*);
    /** Requests a GL viewport resize from the rendering thread. */
    void resizeEvent(QResizeEvent* _e);

    void mousePressEvent(QMouseEvent *event);

private:
    /** Mutex for protecting the GL rendering context for multithreading. */
    QMutex render_mutex;
    /** The rendering thread uses this wait condition to save CPU ressources. */
    QWaitCondition render_condition;
};

#endif // GLWIDGET_H

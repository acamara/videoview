//Declaració de la classe GLWidget (widget OpenGL)

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "renderthread.h"
#include <qgl.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    /** Starts the rendering thread. This method is called in the contructor.     */
    void initRendering();
    /** Stops the rendering thread of the widget. */
    void finishRendering();
    void initadquirir(CvCapture *capture,QString cam);

    /** The rendering thread. */
    RenderThread glt;

signals:
    void widgetClicked();

public slots:
    void canviacamactiva(QString cam);

protected:
    /** Performs a save shutdown if the widget recieves a close event. */
    void closeEvent(QCloseEvent* _e);
    /** Requests a GL viewport resize from the rendering thread. */
    void resizeEvent(QResizeEvent* _e);
    void mousePressEvent(QMouseEvent *event);

private:

};

#endif // GLWIDGET_H

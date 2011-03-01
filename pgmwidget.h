//Declaració de la classe PGMWidget (widget OpenGL)

#ifndef PGMWIDGET_H
#define PGMWIDGET_H

#include "GravarThread.h"
#include <qgl.h>

class PGMWidget : public QGLWidget
{
    Q_OBJECT
public:
    PGMWidget(QWidget *parent = 0);
    /** Starts the rendering thread. This method is called in the contructor.     */
    void initRendering();
    /** Stops the rendering thread of the widget. */
    void finishRendering();
    void initgravar();

    /** The rendering thread. */
    GravarThread glt;

signals:
    void widgetClicked();

protected:
    /** Performs a save shutdown if the widget recieves a close event. */
    void closeEvent(QCloseEvent* _e);
    /** Requests a GL viewport resize from the rendering thread. */
    void resizeEvent(QResizeEvent* _e);
};

#endif // PGMWIDGET_H

//Declaració de la classe PGMWidget (widget OpenGL)

#ifndef PGMWIDGET_H
#define PGMWIDGET_H

#include "GravarThread.h"
#include <qgl.h>
#include <GL/glext.h>

class PGMWidget : public QGLWidget
{
    Q_OBJECT
public:
    PGMWidget(QWidget *parent = 0);
    /** Starts the rendering thread. This method is called in the contructor.     */
    void initRendering();
    void initPGM(QSize resolucio, double fps);
    /** Stops the rendering thread of the widget. */
    void finishRendering();
    void finishGravar();

    /** The rendering thread. */
    GravarThread *pglt;

protected:
    /** Performs a save shutdown if the widget recieves a close event. */
    void closeEvent(QCloseEvent* _e);
    /** Requests a GL viewport resize from the rendering thread. */
    void resizeEvent(QResizeEvent* _e);
    void paintEvent(QPaintEvent *);
};

#endif // PGMWIDGET_H

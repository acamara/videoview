#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(int numcam);
    void sendImage(IplImage *img);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QImage qframe1;
    QImage qframe2;
    QImage qframe3;
    QImage qframe4;
    int numcam;
};

#endif // GLWIDGET_H

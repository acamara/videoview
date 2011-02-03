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
    GLWidget();

signals:
    void mousePressEvent(QMouseEvent *event);

public slots:
    void sendImage(IplImage *img);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QImage qframe;
};

#endif // GLWIDGET_H

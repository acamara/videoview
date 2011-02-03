//Declaració de la clase GLWidget

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

public slots:
    void sendImage(IplImage *img);

signals:
    void widgetClicked();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);

private:
    QImage qframe;
};

#endif // GLWIDGET_H

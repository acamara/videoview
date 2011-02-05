#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget();

public slots:
    void drawframe();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QImage qframe;
};

#endif // GLWIDGET_H

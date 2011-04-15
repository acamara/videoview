#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGL>
#include <QGLWidget>

class Widget : public QGLWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QGLFramebufferObject *fbo;      // Frame Buffer Object
    GLuint fboTexture;              // Textura per FBO
    GLuint fboDepth;                // Depth buffer per a el FBO
};

#endif // WIDGET_H

#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

GLWidget::GLWidget() : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
    setMinimumSize(320,240);
}

void GLWidget::initializeGL() {
       glClearColor(0.0f,0.0f,0.0f,1.0f);   //Activa el color de fons actual,seguint el model de representació RGB
       glEnable(GL_TEXTURE_2D);             //Habilitem el mapejat de textures
}

void GLWidget::paintGL() {
    glClear (GL_COLOR_BUFFER_BIT);      //Netegem el buffer de color
    glClearColor (0.0,0.0,0.0,1.0);     //Activa el color de fons actual,seguint el model de representació RGB

    glMatrixMode(GL_PROJECTION);        //Seleccionem la matriu de projecció
    glLoadIdentity();                   //La inicialtzem com a matriu identitat
    gluOrtho2D(0,1,0,1);                //Creem una matriu per projectar coordenades en dues dimensions en la pantalla i la multipliquem per la matriu actual.

    //cam1                              //Definim uns rectangles  que representan cada càmera
    glColor3f (1.0, 1.0, 1.0);
    glRectf(0.0, 0.5,0.5, 1.0);

    //cam 2
    glColor3f (0.0, 0.0, 1.0);
    glRectf(0.5, 0.5,1.0, 1.0);

    //cam3
    glColor3f (1.0, 0.0, 0.0);
    glRectf(0.0, 0.0,0.5, 0.5);

    //cam4
    glColor3f (0.0, 1.0, 0.0);
    glRectf(0.5, 0.0,1.0, 0.5);



}

void GLWidget::resizeGL(int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
    // qDebug() << "Resizing...";
}

void GLWidget::drawframe()
{
    //LLegim una imatge fent ús de la llibreria openCV.
    IplImage *img= cvLoadImage("imagen.jpg");
    //cvNamedWindow("Imagen", 0);
    //cvShowImage("Imagen", img);

    //La imatge es guarda utilitzant 24-bit RGB(8-8-8).
    qframe = QImage((const unsigned char*)(img->imageData), img->width, img->height, img->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe = QGLWidget::convertToGLFormat(qframe);


    if (!qframe.isNull()) {
            qframe = qframe.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0,qframe.width(),qframe.height(),0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0,qframe.height()/2);
                glTexCoord2f(0,1);
                glVertex2f(0,0);
                glTexCoord2f(1,1);
                glVertex2f(qframe.width()/2,0);
                glTexCoord2f(1,0);
                glVertex2f(qframe.width()/2,qframe.height()/2);
            //glEnd();
        }
    this->updateGL();

    cvReleaseImage(&img);
    qDebug()<<"pintant";
}

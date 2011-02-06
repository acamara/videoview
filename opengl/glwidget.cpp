#include "glwidget.h"

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

    //LLegim una imatge fent ús de la llibreria openCV.
    IplImage *cam1= cvLoadImage("cam1.jpg");
    IplImage *cam2= cvLoadImage("cam2.jpg");
    IplImage *cam3= cvLoadImage("cam3.jpg");
    IplImage *cam4= cvLoadImage("cam4.jpg");

    //La imatge es guarda utilitzant 24-bit RGB(8-8-8).
    qframe1 = QImage((const unsigned char*)(cam1->imageData), cam1->width, cam1->height, cam1->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe1 = QGLWidget::convertToGLFormat(qframe1);
    qframe2 = QImage((const unsigned char*)(cam2->imageData), cam2->width, cam2->height, cam2->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe2 = QGLWidget::convertToGLFormat(qframe2);
    qframe3 = QImage((const unsigned char*)(cam3->imageData), cam3->width, cam3->height, cam3->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe3 = QGLWidget::convertToGLFormat(qframe3);
    qframe4 = QImage((const unsigned char*)(cam4->imageData), cam4->width, cam4->height, cam4->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe4 = QGLWidget::convertToGLFormat(qframe4);


    if (!qframe1.isNull()||!qframe2.isNull()||qframe3.isNull()||qframe4.isNull()) {
            qframe1 = qframe1.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe2 = qframe2.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe3 = qframe3.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe4 = qframe4.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            glMatrixMode(GL_PROJECTION);        //Seleccionem la matriu de projecció
            glLoadIdentity();                   //La inicialtzem com a matriu identitat
            gluOrtho2D(0,1,0,1);                //Creem una matriu per projectar coordenades en dues dimensions en la pantalla i la multipliquem per la matriu actual.

            //Habilitem la textura y la carregem
            glEnable(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe1.width(), qframe1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe1.bits() );

            //Assignem les coordenades a la textura
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0,0.5);
                glTexCoord2f(1,0);
                glVertex2f(0.5,0.5);
                glTexCoord2f(1,1);
                glVertex2f(.5,1.0);
                glTexCoord2f(0,1);
                glVertex2f(0,1.0);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe2.width(), qframe2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe2.bits() );

            //Assignem les coordenades a la textura
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.5,0.5);
                glTexCoord2f(1,0);
                glVertex2f(1.0,0.5);
                glTexCoord2f(1,1);
                glVertex2f(1.0,1.0);
                glTexCoord2f(0,1);
                glVertex2f(0.5,1.0);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe3.width(), qframe3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe3.bits() );

            //Assignem les coordenades a la textura
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0,0);
                glTexCoord2f(1,0);
                glVertex2f(0.5,0);
                glTexCoord2f(1,1);
                glVertex2f(.5,.5);
                glTexCoord2f(0,1);
                glVertex2f(0,.5);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe4.width(), qframe4.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe4.bits() );

            //Assignem les coordenades a la textura
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.5,0);
                glTexCoord2f(1,0);
                glVertex2f(1.0,0);
                glTexCoord2f(1,1);
                glVertex2f(1.0,.5);
                glTexCoord2f(0,1);
                glVertex2f(0.5,0.5);
            glEnd();
        }

}

void GLWidget::resizeGL(int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
    // qDebug() << "Resizing...";
}

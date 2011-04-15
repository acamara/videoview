#include "glwidget.h"

GLWidget::GLWidget(int numcams) : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
    setMinimumSize(320,240);
    numcam=numcams;
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

    //Habilitem la textura y la carregem
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    switch(numcam)

    {
    case 1:

        if (!qframe1.isNull()) {
            qframe1 = qframe1.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe1.width(), qframe1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe1.bits() );

            //Assignem les coordenades a la textura "CAM-1"
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.05,0.05);
                glTexCoord2f(1,0);
                glVertex2f(0.95,0.05);
                glTexCoord2f(1,1);
                glVertex2f(0.95,0.95);
                glTexCoord2f(0,1);
                glVertex2f(0.05,0.95);
            glEnd();
            }

    break;

    case 2:

       if (!qframe1.isNull()||!qframe2.isNull()) {
           qframe1 = qframe1.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
           qframe2 = qframe2.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

           glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe1.width(), qframe1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe1.bits() );

           //Assignem les coordenades a la textura "CAM-1"
           glBegin(GL_POLYGON);
               glTexCoord2f(0,0);
               glVertex2f(0.05,0.55);
               glTexCoord2f(1,0);
               glVertex2f(0.45,0.55);
               glTexCoord2f(1,1);
               glVertex2f(.45,0.95);
               glTexCoord2f(0,1);
               glVertex2f(0.05,0.95);
           glEnd();

           glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe2.width(), qframe2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe2.bits() );

           //Assignem les coordenades a la textura "CAM-2"
           glBegin(GL_POLYGON);
               glTexCoord2f(0,0);
               glVertex2f(0.05,0.05);
               glTexCoord2f(1,0);
               glVertex2f(0.45,0.05);
               glTexCoord2f(1,1);
               glVertex2f(.45,0.45);
               glTexCoord2f(0,1);
               glVertex2f(0.05,0.45);
           glEnd();
           }

    break;

    case 3:

        if (!qframe1.isNull()||!qframe2.isNull()||!qframe3.isNull()) {
           qframe1 = qframe1.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
           qframe2 = qframe2.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
           qframe3 = qframe3.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

           glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe1.width(), qframe1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe1.bits() );

           //Assignem les coordenades a la textura "CAM-1"
           glBegin(GL_POLYGON);
               glTexCoord2f(0,0);
               glVertex2f(0.05,0.55);
               glTexCoord2f(1,0);
               glVertex2f(0.45,0.55);
               glTexCoord2f(1,1);
               glVertex2f(.45,0.95);
               glTexCoord2f(0,1);
               glVertex2f(0.05,0.95);
           glEnd();

           glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe2.width(), qframe2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe2.bits() );

           //Assignem les coordenades a la textura "CAM-2"
           glBegin(GL_POLYGON);
               glTexCoord2f(0,0);
               glVertex2f(0.55,0.55);
               glTexCoord2f(1,0);
               glVertex2f(0.95,0.55);
               glTexCoord2f(1,1);
               glVertex2f(0.95,0.95);
               glTexCoord2f(0,1);
               glVertex2f(0.55,0.95);
           glEnd();

           glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe3.width(), qframe3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe3.bits() );

           //Assignem les coordenades a la textura "CAM-3"
           glBegin(GL_POLYGON);
               glTexCoord2f(0,0);
               glVertex2f(0.05,0.05);
               glTexCoord2f(1,0);
               glVertex2f(0.45,0.05);
               glTexCoord2f(1,1);
               glVertex2f(.45,0.45);
               glTexCoord2f(0,1);
               glVertex2f(0.05,0.45);
           glEnd();
           }
    break;

    default:

     if (!qframe1.isNull()||!qframe2.isNull()||!qframe3.isNull()||!qframe4.isNull()) {
            qframe1 = qframe1.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe2 = qframe2.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe3 = qframe3.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            qframe4 = qframe4.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe1.width(), qframe1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe1.bits() );

            //Assignem les coordenades a la textura "CAM-1"
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.05,0.55);
                glTexCoord2f(1,0);
                glVertex2f(0.45,0.55);
                glTexCoord2f(1,1);
                glVertex2f(.45,0.95);
                glTexCoord2f(0,1);
                glVertex2f(0.05,0.95);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe2.width(), qframe2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe2.bits() );

            //Assignem les coordenades a la textura "CAM-2"
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.55,0.55);
                glTexCoord2f(1,0);
                glVertex2f(0.95,0.55);
                glTexCoord2f(1,1);
                glVertex2f(0.95,0.95);
                glTexCoord2f(0,1);
                glVertex2f(0.55,0.95);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe3.width(), qframe3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe3.bits() );

            //Assignem les coordenades a la textura "CAM-3"
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.05,0.05);
                glTexCoord2f(1,0);
                glVertex2f(0.45,0.05);
                glTexCoord2f(1,1);
                glVertex2f(.45,0.45);
                glTexCoord2f(0,1);
                glVertex2f(0.05,0.45);
            glEnd();

            glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe4.width(), qframe4.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe4.bits() );

            //Assignem les coordenades a la textura "CAM-4"
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex2f(0.55,0.05);
                glTexCoord2f(1,0);
                glVertex2f(0.95,0.05);
                glTexCoord2f(1,1);
                glVertex2f(0.95,.45);
                glTexCoord2f(0,1);
                glVertex2f(0.55,0.45);
            glEnd();

        }

    break;
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

void GLWidget::sendImage(IplImage *img) {
    //La imatge es guarda utilitzant 24-bit RGB(8-8-8).
    qframe1 = QImage((const unsigned char*)(img->imageData), img->width, img->height, img->widthStep, QImage::Format_RGB888).rgbSwapped();
    qframe1 = QGLWidget::convertToGLFormat(qframe1);
    qframe2=qframe1;
    qframe3=qframe1;
    qframe4=qframe1;

    this->updateGL();
}

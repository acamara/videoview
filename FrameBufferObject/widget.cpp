#include "widget.h"
#include <QGLFramebufferObject>

Widget::Widget(QWidget *parent) :
    QGLWidget(parent)
{
    // create the framebuffer object - make sure to have a current
    // context before creating it
    makeCurrent();
    fbo = new QGLFramebufferObject(512, 512,QGLFramebufferObject::Depth,GL_TEXTURE_2D,GL_RGBA8);
    setWindowTitle(tr("OpenGL framebuffer objects"));
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    // Buffer de textura
    glGenTextures(1, &fboTexture);              // Generem una textura
    glBindTexture(GL_TEXTURE_2D, fboTexture);   // Asignamem la textura a fboTexture

    // Creem una textura de la mida de la finestra
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(), this->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Paràmetres básics per a la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Deshabilitem la textura
    glBindTexture(GL_TEXTURE_2D, 0);

    // Asignem (attachment) la textura fboTexture al buffer de color del nostre frame buffer
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fboTexture, 0);

    // Asignem (attachment) el depth buffer fboDepth al nostre frame buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fboDepth);

    // Revisem l'estat del frame buffer
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)      // Si no està complet
    {
        // No s'ha creat el FBO correctament
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);    // Deshabilitem el frame buffer

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);  // Preparem el FBO per al renderizat
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);  // Guardem els estats de glEnable y glViewport
    glViewport(0, 0, 1, 1);                         // Establim la mida del viewport del FBO

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Netegem la pantalla i el buffer de profunditat

    // Renderizado de escena

    glPopAttrib();                                  //Reestablimt els estats de glEnable y glViewport
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);    // Deshabilitem el FBO

}

void Widget::resizeGL(int w, int h)
{

}

void Widget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Netega la pantalla y el buffer de profundidad
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);                               // Desactiva la prova de profunditat
    glMatrixMode(GL_PROJECTION);                            // Selecciona la matriu de projecció
    glPushMatrix();                                         // Guarda la matriu de projecció
    glLoadIdentity();                                       // Carrega la matriu de projecció
    gluOrtho2D(0, 1, 0, 1);                                 // Crea una projecció ortogonal
    glMatrixMode(GL_MODELVIEW);                             // Selecciona la matriu del model de vista
    glPushMatrix();                                         // Guarda la matriu del model de vista

    glDisable(GL_LIGHTING);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);

    // Habilitem la textura assignada al FBO
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1, 1);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 1);
    glEnd();
}

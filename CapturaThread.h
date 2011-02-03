//Declaració de la clase CapturaThread

#ifndef CAPTURATHREAD_H
#define CAPTURATHREAD_H

#include <QtCore/QThread>
#include <QTime>

#include <glwidget.h>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

class CapturaThread : public QThread
{
 Q_OBJECT

public:
     CapturaThread(QObject *parent,char *camara, GLWidget *glWidgetcam);
     ~CapturaThread();
     void selecfontvideo(QString fontdevideo);
signals:
     void mostrarframe(IplImage *frame, char *camera,GLWidget *glWidgetcam);

public slots:
     void estatdelatransicio();

protected:
     void run();

private:
     char *camara;
     GLWidget *glwidgetcam;
     QString fontvideo;
     CvCapture *cap;
     bool parar;
     bool estattransicio;

 };



#endif // CAPTURATHREAD_H

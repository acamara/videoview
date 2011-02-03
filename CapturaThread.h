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
     CapturaThread(QObject *parent,QString camara, GLWidget *glWidgetcam,GLWidget *glWidgetpgm);
     ~CapturaThread();
     void selecfontvideo(QString fontdevideo);

public slots:
     void selecfontPGM();

protected:
     void run();

private:
     GLWidget *glwidgetcam;
     GLWidget *glwidgetpgm;
     QString camara,camarapgm;
     QString fontvideo;
     CvCapture *cap;
     IplImage *frame;
     IplImage *pgm;
     bool parar;
     bool estattransicio;

};



#endif // CAPTURATHREAD_H

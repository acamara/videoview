//Implementaci� de la clase CapturaThread.

#include <QtGui>
#include "CapturaThread.h"

//Constructor de la clase CapturaThread
CapturaThread::CapturaThread(QObject *parent,char *camaraacapturar,GLWidget *glWidgetcam)
        : QThread(parent)
{
     camara=camaraacapturar;
     glwidgetcam=glWidgetcam;
}

//Destructor de la clase CapturaThread
CapturaThread::~CapturaThread()
{

}

//Funci� menbre que selecciona la font de video
void CapturaThread::selecfontvideo(QString fontdevideo)
{
     fontvideo=fontdevideo;
}

//Funci� menbre que controla l'estat de la transici�
void CapturaThread::estatdelatransicio()
{
   estattransicio=false;
}


//Funci� menbre que comen�a la captura
void CapturaThread::run()
 {
    CvCapture *cap;

    try
    {
        cap= cvCaptureFromFile(fontvideo.toAscii());
        //cap= cvCaptureFromCAM(0);
    }

    catch (...)
    {
        cap= NULL;
    }

    if (!cap)
    {
     qDebug()<<"Error, No se puede abrir c�mara.";
    }  


  //cvNamedWindow(camara,1);

   IplImage *frame = cvQueryFrame(cap);

   int tecla= cvWaitKey(10);

   while (frame && tecla==-1) {
       //cvShowImage(camara, frame);
       frame= cvQueryFrame(cap);
       mostrarframe(frame,camara,glwidgetcam);
       tecla= cvWaitKey(10);
   }
   cvReleaseCapture(&cap);
 }

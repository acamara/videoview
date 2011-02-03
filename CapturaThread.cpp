//Implementaci� de la clase CapturaThread.

#include <QtGui>
#include "CapturaThread.h"

//Constructor de la clase CapturaThread
CapturaThread::CapturaThread(QObject *parent,QString camaraacapturar,GLWidget *glWidgetcam,GLWidget *glWidgetpgm)
        : QThread(parent)
{
     camara=camaraacapturar;
     glwidgetcam=glWidgetcam;
     glwidgetpgm=glWidgetpgm;
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

//Funci� menbre que selecciona la font de video de sortida PGM
void CapturaThread::selecfontPGM()
{
     camarapgm=sender()->objectName();
     qDebug()<<camarapgm<<"   "<<camara;

}

//Funci� menbre que realitza l'adquisici� dels frames
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

   frame = cvQueryFrame(cap);

   while (frame) {
       //cvShowImage(camara, frame);
       frame= cvQueryFrame(cap);
       glwidgetcam->sendImage(frame);
       if(camara==camarapgm){
           pgm=cvCloneImage(frame);
           glwidgetpgm->sendImage(pgm);
       }
       cvWaitKey(25);
   }
   cvReleaseCapture(&cap);
 }

//Implementaci� de la clase GravarThread.

#include <QtGui>
#include "GravarThread.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


int peso= 0;
CvVideoWriter *video;
CvSize size = cvSize(720,576); //Tama�o de grabaci�n
double fps = 25; // Frames por Segundo

GravarThread::GravarThread(QObject *parent,QString nomdeprojecte,int tipusdetransicio,int duradadetransicio)
    : QThread(parent)
{
    tipustransicio = tipusdetransicio;
    duradatransicio = duradadetransicio;
}

GravarThread::~GravarThread()
{

}

void GravarThread::run()
{
    video= cvCreateVideoWriter("salida.avi",CV_FOURCC('M','J','P','G'),fps,size, 1);
}


//Funci� menbre que estableix el tipus de transici�
void GravarThread::selectransicio(int tipusdetransicio)
{
    tipustransicio=tipusdetransicio;
    qDebug()<<tipustransicio;
}

//Funci� menbre que estableix la durada de la transici�
void GravarThread::selecduratransicio(int duradetransicio)
{
    duradatransicio=duradetransicio;
    qDebug()<<duradatransicio;
}

//Funci� menbre que manipula la transici�

void GravarThread::activarlatransicio(IplImage* Imatgeactual,IplImage *Imatgeanterior,bool estattransicio)
{

    switch(tipustransicio)
    {
    case 1: //Fos

        if(estattransicio){
        IplImage *ent1, *ent2, *sal;

        ent1= cvCloneImage(Imatgeactual);
        ent2= cvCloneImage(Imatgeanterior);

        sal= cvCloneImage(ent1);
        cvAddWeighted(ent1, peso/100.0, ent2, 1.0-peso/100.0, 0, sal);

        cvShowImage("PGM", sal);
        cvReleaseImage(&ent1);
        cvReleaseImage(&ent2);
        cvReleaseImage(&sal);
        peso++;

        if(peso==100)
        {
          pararlatransicio();
          peso=0;
        }
        break;
        }

    default:    //Transici� per defecte Tall
        cvShowImage("PGM", Imatgeactual);
        pararlatransicio();
       // qDebug()<<"Transici� activada,default";
    }

}

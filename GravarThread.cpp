//Implementació de la classe GravarThread.

#include <QtGui>
#include "GravarThread.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


int peso= 0;
CvVideoWriter *video;
CvSize size = cvSize(720,576); //Resolució de gravació
double fps = 25; // Frames per Segon

//Constructor de la classe GravarThread
GravarThread::GravarThread(QObject *parent,QString nomdeprojecte,int tipusdetransicio,int duradadetransicio)
    : QThread(parent)
{
    tipustransicio = tipusdetransicio;
    duradatransicio = duradadetransicio;
}

//Destructor de la classe GravarThread
GravarThread::~GravarThread()
{

}

//Mètode que realitza la gravació i el processat dels frames de sortida PGM
void GravarThread::run()
{
    video= cvCreateVideoWriter("sortida.avi",CV_FOURCC('M','J','P','G'),fps,size, 1);
}


//Mètode que estableix el tipus de transició
void GravarThread::selectransicio(int tipusdetransicio)
{
    tipustransicio=tipusdetransicio;
    qDebug()<<tipustransicio;
}

//Mètode que estableix la durada de la transició
void GravarThread::selecduratransicio(int duradetransicio)
{
    duradatransicio=duradetransicio;
    qDebug()<<duradatransicio;
}

//Mètode que manipula la transició
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

        //cvShowImage("PGM", sal);
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

    default:    //Transició per defecte Tall
        //cvShowImage("PGM", Imatgeactual);
        pararlatransicio();
       // qDebug()<<"Transició activada,default";
    }

}

//Mètode que rep les imatges que s'han de gravar
void GravarThread::rebregravar(IplImage *frame){
    //qDebug()<<"S'ha rebut per gravar";
    //cvNamedWindow("v",0);
    //cvShowImage("v",frame);
}

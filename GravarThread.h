//Declaració de la clase GravarThread

#ifndef GRAVARTHREAD_H
#define GRAVARTHREAD_H

#include <QtCore/QThread>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


class GravarThread : public QThread
{
    Q_OBJECT

    public:
        GravarThread(QObject *parent,QString nomdeprojecte,int tipusdetransicio,int duradetransicio);
        ~GravarThread();

    public slots:
         void selectransicio(int tipusdetransicio);
         void selecduratransicio(int duradetransicio);
         void activarlatransicio(IplImage*,IplImage *,bool estattransicio);

    signals:
         void pararlatransicio();

    private:
        int duradatransicio;
        int tipustransicio;

    protected:
        void run();
 };


#endif // GRAVARTHREAD_H

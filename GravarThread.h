//Declaració de la classe GravarThread

#ifndef GRAVARTHREAD_H
#define GRAVARTHREAD_H

#include <QtCore/QThread>
#include <qgl.h>

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

class PGMWidget;

class GravarThread : public QThread
{
    Q_OBJECT

    public:
        GravarThread(PGMWidget *_glw);
        void run();
        void stop();
        void resizeViewport(const QSize& _size);
        void rebregravar(IplImage*);

    public slots:
        void activatransicio(bool _transicio);
        void afegeixmosca();
        void paratransicio();
        void setconfig(QSize _resolucio,double _fps,int _duradetransicio);
        void setgravar(bool _gravar);
        void selectransicio(int tipusdetransicio);
        void selecduratransicio(int duradetransicio);
        void tancavideo();

    protected:
        void paintGL();
        void resizeGL(int width, int height);
        void realitzatransicio(IplImage* Imatgeactual);

   private:
        PGMWidget *glw;
        bool render_flag;
        bool resize_flag;
        bool gravar;
        bool mostrar;
        bool transicio;
        QSize viewport_size;
        QSize resolucio;
        IplImage *frame;
        double fps;
        int duradatransicio;
        int tipustransicio;
        CvVideoWriter *video;
 };

#endif // GRAVARTHREAD_H

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

    public slots:
         void selectransicio(int tipusdetransicio);
         void selecduratransicio(int duradetransicio);
         void rebregravar(IplImage*);
         void setgravar(bool _gravar);

    protected:
        void resizeGL(int width, int height);
        void paintGL();

   private:
        PGMWidget *glw;
        bool render_flag;
        bool resize_flag;
        bool gravar;
        bool mostrar;

        QSize viewport_size;
        IplImage *frame;
        IplImage *auxframe;
        int duradatransicio;
        int tipustransicio;
 };

#endif // GRAVARTHREAD_H

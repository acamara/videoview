#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "glwidget.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void startCam();
    void stopCam();
    void processCam();

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;

    CvCapture *capture;
    QTime timer;
    bool parar;

};

#endif // MAINWINDOW_H

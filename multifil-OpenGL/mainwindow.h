//Declaració de la classe MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTime>
#include <QMainWindow>

#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startCam();
    void stopCam();
    void processCam();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;


    CvCapture *capture;
    QTime timer;
    bool parar;
};

#endif // MAINWINDOW_H

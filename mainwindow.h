//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "CapturaThread.h"
#include "GravarThread.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots: 
    void cambiarcamara();

protected:
    void changeEvent(QEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_stopButton_clicked();
    void on_capturaButton_clicked();
    void newFile();
    void about();
    void aboutQt();

private:
    Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    void creainterficie();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QLabel *cam1Label;
    QLabel *cam2Label;
    QLabel *cam3Label;
    QLabel *cam4Label;
    QLabel *pgmLabel;

    GLWidget *glWidget_cam1;
    GLWidget *glWidget_cam2;
    GLWidget *glWidget_cam3;
    GLWidget *glWidget_cam4;
    GLWidget *glWidget_pgm;

    CapturaThread *Capturathread1;
    CapturaThread *Capturathread2;
    CapturaThread *Capturathread3;
    CapturaThread *Capturathread4;

    GravarThread  *Gravarthread;

};

#endif // MAINWINDOW_H

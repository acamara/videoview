//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "glwidget.h"
#include "pgmwidget.h"


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
    void canviacamara();

protected:
    void changeEvent(QEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_stopButton_2_clicked();
    void on_gravarButton_clicked();
    void on_stopButton_clicked();
    void on_adquirirButton_clicked();
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

    int numcam;
    QSize resolucio;

    enum { numwidgets = 4 };

    QLabel *Label_cam[numwidgets];
    QLabel *Label_pgm;
    GLWidget *glWidget_cam[numwidgets];
    PGMWidget *glWidget_pgm;

    CvCapture *capture[numwidgets];
};

#endif // MAINWINDOW_H

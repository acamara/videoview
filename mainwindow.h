//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

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

protected:
    void changeEvent(QEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *);

private slots:
    void on_templatesButton_clicked();
    void on_moscaButton_clicked();
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
    void finishCameras();

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

    QWidget *widget_pgm;
    QWidget *widget_cam[numwidgets];

    //Elements d'entrada
    GstElement  *bin_font[numwidgets];
    GstElement  *source_[numwidgets];
    GstElement  *tee_[numwidgets];
    GstElement  *queue_[numwidgets];
    GstElement  *sink_[numwidgets];
    GstElement  *queue_mix[numwidgets];

    //Elements de sortida
    GstElement  *bin_pgm;
    GstElement  *tee_pgm;
    GstElement  *queue_pgm;
    GstElement  *sink_pgm;
    GstElement  *queue_fitxer;
    GstElement  *conv_video_pgm;
    GstElement  *encoder_pgm;
    GstElement  *mux_pgm;
    GstElement  *sink_fitxer;

    GstElement  *videomixer;
    GstElement  *pipeline;
};

#endif // MAINWINDOW_H

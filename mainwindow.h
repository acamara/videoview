//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgetvideo.h"

#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QComboBox;
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
    void closeEvent(QCloseEvent *);

private slots:
    void on_audioSlider_valueChanged(int value);
    void on_templatesButton_clicked();
    void on_moscaButton_clicked();
    void on_stopButton_2_clicked();
    void on_gravarButton_clicked();
    void on_stopButton_clicked();
    void on_adquirirButton_clicked();
    void newFile();
    void about();
    void aboutQt();
    void Entrada_test(int k);
    void Entrada_camera(int k);
    void Entrada_fitxer(int k);
    void Entrada_audio(int k);


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
    QComboBox *combobox_cam[numwidgets];

    QWidget *widget_pgm;
    Widgetvideo *widget_cam[numwidgets];

    //Elements d'entrada
    GstElement  *bin_font[numwidgets];
    GstElement  *source_[numwidgets];
    GstElement  *tee_[numwidgets];
    GstElement  *queue_[numwidgets];
    GstElement  *sink_[numwidgets];
    GstElement  *queue_mix[numwidgets];

    GstElement  *bin_audio_font[numwidgets];
    GstElement  *audio_source[numwidgets];
    GstElement  *volume[numwidgets];
    GstElement  *tee_audio[numwidgets];
    GstElement  *queue_audio[numwidgets];
    GstElement  *queue_audio_mix[numwidgets];

    //Elements d'entrada de fitxer
    GstElement  *dec_[numwidgets];
    GstElement  *conv_audio_[numwidgets];
    GstElement  *sink_audio_[numwidgets];
    GstElement  *conv_video_[numwidgets];
    GstPad  *audiopad_[numwidgets];
    GstPad  *videopad_[numwidgets];


    //Elements de sortida
    GstElement  *bin_video_pgm;
    GstElement  *tee_video_pgm;
    GstElement  *queue_video_pgm;
    GstElement  *sink_video_pgm;
    GstElement  *queue_video_fitxer;
    GstElement  *conv_video_pgm;
    GstElement  *encoder_video_pgm;
    GstElement  *videomixer;

    GstElement  *bin_audio_pgm;
    GstElement  *tee_audio_pgm;
    GstElement  *queue_audio_pgm;
    GstElement  *sink_audio_pgm;
    GstElement  *queue_audio_fitxer;
    GstElement  *conv_audio_pgm;
    GstElement  *encoder_audio_pgm;
    GstElement  *volume_audio_pgm;
    GstElement  *audiomixer;

    GstElement  *bin_fitxer_pgm;
    GstElement  *mux_pgm;
    GstElement  *sink_fitxer;

    GstElement  *pipeline;
};

#endif // MAINWINDOW_H

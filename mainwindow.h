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

struct EntradaVideo {
  GstElement *bin_font, *source, *tee, *queue, *sink, *queue_mix;
  void crea(int k, GstElement *, const char *type);
};

struct EntradaAudio {
  GstElement *bin_font, *source, *tee, *queue, *sink, *queue_mix, *volume;
  void crea(int k, GstElement *);
};

struct EntradaFitxer {
  EntradaVideo v;
  EntradaAudio a;

  GstElement *bin_font, *source, *dec, *conv_audio, *conv_video;
  GstPad     *audiopad, *videopad;
  GstElement *audiobin, *videobin;
  void crea(int k, GstElement *);
};


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum { maxcam = 4 };

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

    QLabel *Label_cam[maxcam];
    QLabel *Label_pgm;
    QComboBox *combobox_cam[maxcam];

    QWidget *widget_pgm;
    Widgetvideo *widget_cam[maxcam];

    // Grups d'Elements
    EntradaVideo  ventrades[maxcam];
    EntradaAudio  aentrades[maxcam];
    EntradaFitxer fentrades[maxcam];

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

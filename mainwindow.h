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
  GstElement *bin_font, *source, *videorate, *tee, *queue, *sink, *queue_mix;
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

struct SortidaPGM {
  GstElement  *bin_pgm,*tee, *queue, *sink, *mixer, *volum;
  void crea(GstElement *,const char *type,const char *typesink, bool audio);
};

struct SortidaFitxer {
    GstElement  *bin_fitxer, *queue, *conv, *encoder;
    void crea(GstElement *);
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

    // Grups d'elements d'entrada
    EntradaVideo  ventrades[maxcam];
    EntradaAudio  aentrades[maxcam];
    EntradaFitxer fentrades[maxcam];

    //Grup d'elements de sortida
    SortidaPGM  vpgm;
    SortidaPGM  apgm;
    SortidaFitxer vfitxer;
    SortidaFitxer afitxer;

    GstElement  *mux_pgm;
    GstElement  *sink_fitxer;
    GstElement  *pipeline;
};

#endif // MAINWINDOW_H

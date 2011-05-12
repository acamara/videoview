//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgetvideo.h"

#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QSlider;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

struct ElementsComuns {
  GstElement *bin, *tee, *queue, *queue_mix;
  void creacomuns(int k, QString ref);
};

struct EntradaVideo: public ElementsComuns {
  GstElement *source, *color_conv, *scale, *scale_mix, *sink;
  void crea(int k, GstElement *, const char *type,QSize resolucio, int framerate);
  void creatransformadors(int k);
};

struct EntradaAudio: public ElementsComuns {
  GstElement *source, *volume, *volume_mix, *sink;
  void crea(int k, GstElement *);
};

struct EntradaFitxer {
  EntradaVideo v;
  EntradaAudio a;

  GstElement *bin_font, *source, *dec, *conv_audio, *conv_video;
  GstPad     *audiopad, *videopad;
  void crea(int k, GstElement *,QString nomfitxer);
};

struct EntradaLogo {
  GstElement *bin_logo, *source, *dec, *imagefreeze, *conv_logo;
  void crea(GstElement *,QString nomfitxer);
};

struct VideoPGM: public ElementsComuns {
  GstElement  *mixer, *color_conv, *textoverlay, *sink;
  void crea(int k, GstElement *);
};

struct AudioPGM: public ElementsComuns {
  GstElement  *mixer, *volum, *sink;
  void crea(int k, GstElement *);
};

struct SortidaFitxer {
    GstElement  *bin_fitxer, *conv, *encoder;
    void crea(GstElement *, GstElement *, GstElement *, const char* type, const char* typeconverter, const char* typeencoder);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum { maxcam = 8 };

public slots:
    void canviacamara(int);
    void canviavolum(int);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);
    void keyPressEvent( QKeyEvent * event );

private slots:
    void on_checkBox_insereixtitol_stateChanged(int );
    void on_addButton_clicked();
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
    void creainterficie();
    void finishCameras();

    int numcam;
    int framerate;
    QSize resolucio;
    QString imagelogo;
    QStringList fontvideo;

    QLabel *Label_cam[maxcam];
    QLabel *Label_pgm;
    QComboBox *combobox_cam[maxcam];

    QWidget *widget_pgm;
    Widgetvideo *widget_cam[maxcam];
    QSlider *slideraudio[maxcam];

    // Grups d'elements d'entrada
    EntradaVideo  ventrades[maxcam];
    EntradaAudio  aentrades[maxcam];
    EntradaFitxer fentrades[maxcam];
    EntradaLogo   lentrada;

    //Grup d'elements de sortida
    VideoPGM  vpgm;
    AudioPGM  apgm;
    SortidaFitxer vfitxer;
    SortidaFitxer afitxer;

    GstElement  *mux_pgm;
    GstElement  *sink_fitxer;
    GstElement  *pipeline;
};

#endif // MAINWINDOW_H

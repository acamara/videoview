//Declaració de la clase MainWindow

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgetvideo.h"
#include "eprocessat.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QSlider;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

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
    void on_checkBox_insereixlogo_stateChanged(int );
    void on_removeButton_clicked();
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

    GstBus *bus;
    GstCaps *caps_resolucio;
    GstCaps *caps_color;
    GstElement  *mux_pgm;
    GstElement  *sink_fitxer;
    GstElement  *pipeline;
};

#endif // MAINWINDOW_H

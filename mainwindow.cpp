//Implementació de la clase MainWindow

#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

//Constructor de la clase MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopButton->setEnabled(false);
    ui->moscaButton->setEnabled(false);
    ui->checkBox_insereixlogo->setEnabled(false);
    ui->checkBox_insereixtitol->setEnabled(false);

    createActions();

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio,framerate);
    creainterficie();

    /* Inicialitzacio Gstreamer */
    gst_init (NULL, NULL);
    pipeline = NULL;
}

//Destructor de la clase MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
    on_stopButton_clicked();
    finishCameras();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//Mètode que controla si s'ha clicat el nùmero del widget, per fer el canvi de càmera
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    int k=event->key();
    if(48<k && k<(49+numcam)){
        canviacamara(k-49);
    }
}

//Mètode que genera la interfície gràfica segons el nombre de càmeres
void MainWindow::creainterficie()
{
    // Declaració variables gráfiques

    QString nom("CAM %1");

    for (int k=0;k<numcam;k++){
        Label_cam[k] = new QLabel(tr(nom.arg(k + 1).toAscii()));
        Label_cam[k]->setAlignment(Qt::AlignCenter);
        Label_cam[k]->setFont(QFont("arial", 10, QFont::Bold));
        Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");

        combobox_cam[k] = new QComboBox();
        QStringList list_boxcam;
        list_boxcam<<"Vídeo Test"<<"Fitxer"<< "Càmera";
        combobox_cam[k]->addItems(list_boxcam);

        widget_cam[k] = new Widgetvideo(ui->centralWidget);
        widget_cam[k]->setStyleSheet("background-color: rgb(0, 0, 0)");
        widget_cam[k]->setObjectName(nom.arg(k).toAscii());
        QObject::connect(widget_cam[k], SIGNAL(widgetClicked(int)), this, SLOT(canviacamara(int)));

        slideraudio[k] = new QSlider(Qt::Vertical,0);
        slideraudio[k]->setRange(0,10);
        slideraudio[k]->setObjectName(nom.arg(k).toAscii());
        ui->horizontalLayout_audio->addWidget(slideraudio[k]);
        QObject::connect(slideraudio[k], SIGNAL(valueChanged(int)), this, SLOT(canviavolum(int)));
    }

    Label_pgm = new QLabel(tr("PGM"));
    Label_pgm->setAlignment(Qt::AlignCenter);

    ui->gridLayout->addWidget(Label_pgm,1,3);

    widget_pgm = new QWidget(ui->centralWidget);
    widget_pgm->setStyleSheet("background-color: rgb(0, 0, 0)");

    ui->gridLayout->addWidget(widget_pgm,2,3);

    //Switch() de posicionament dels diferents widgets a la finestra

    switch(numcam)

    {
    default:
        ui->gridLayout->addWidget(Label_cam[3],4,2);
        ui->gridLayout->addWidget(widget_cam[3],5,2);
        ui->gridLayout->addWidget(combobox_cam[3],6,2);

    case 3:
        ui->gridLayout->addWidget(Label_cam[2],4,1);
        ui->gridLayout->addWidget(widget_cam[2],5,1);
        ui->gridLayout->addWidget(combobox_cam[2],6,1);

    case 2:
        ui->gridLayout->addWidget(Label_cam[0],1,1);
        ui->gridLayout->addWidget(Label_cam[1],1,2);

        ui->gridLayout->addWidget(widget_cam[0],2,1);
        ui->gridLayout->addWidget(widget_cam[1],2,2);

        ui->gridLayout->addWidget(combobox_cam[0],3,1);
        ui->gridLayout->addWidget(combobox_cam[1],3,2);
        break;
    case 1:
        ui->gridLayout->addWidget(Label_cam[0],1,1,1,2);
        ui->gridLayout->addWidget(widget_cam[0],2,1,4,2);
        ui->gridLayout->addWidget(combobox_cam[0],6,2);
    }

    ui->gridLayout->setColumnMinimumWidth(1,336);
    ui->gridLayout->setColumnMinimumWidth(2,336);
    ui->gridLayout->setColumnMinimumWidth(3,336);

    ui->gridLayout->setRowMinimumHeight(2,252);
    ui->gridLayout->setRowMinimumHeight(5,252);
}

//Mètode que controla l'acció nou fitxer
void MainWindow::newFile()
{
    finishCameras();
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio,framerate);
    creainterficie();
}

//Mètode que controla l'acció about
void MainWindow::about()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Ajuda de Capturadora"));
}

//Mètode que controla l'acció aboutQt
void MainWindow::aboutQt()
{
    QMessageBox::about(this, tr("Sobre Capturadora"),
            tr("Sobre capturadora, versió 1, Tots els drets reservats"));
}

//Mètode que crea les diferents accions de la finestra principal
void MainWindow::createActions()
{
    connect(ui->actionNova, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionSortir_2, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAjuda_de_Capturadora, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionSobre, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

//Métode que tanca tots els Layers de càmeres.
void MainWindow::finishCameras()
{
  for (int k = 0; k < numcam; k++) {
    delete Label_cam[k];
    delete combobox_cam[k];
    delete widget_cam[k];
    delete slideraudio[k];
  }
  delete widget_pgm;
}

static gboolean link_elements_with_filter (GstElement *element1, GstElement *element2, GstCaps *caps)
{
  gboolean link_ok;

  link_ok = gst_element_link_filtered (element1, element2, caps);

  if (!link_ok) {
    g_warning ("No s'ha pogut linkar element1 i element2 amb filtre demanat");
  }

  return link_ok;
}

static void cb_newpad_audio (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *audiopad;

  //Linkem només una vegada
  audiopad = gst_element_get_static_pad (grup->a.bin, "sink");
  if (GST_PAD_IS_LINKED (audiopad)) {
    g_object_unref (audiopad);
    return;
  }

  //Mirem els tipus de dades multimedia
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "audio")) {
    gst_caps_unref (caps);
    gst_object_unref (audiopad);
    return;
  }
  gst_caps_unref (caps);

  //Link'n'play
  gst_pad_link (pad, audiopad);

  g_object_unref (audiopad);
}

static void cb_newpad_video (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;

  //Linkem només una vegada
  videopad = gst_element_get_static_pad (grup->v.bin, "sink");
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }

  //Mirem els tipus de dades multimedia
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  gst_caps_unref (caps);

  //Link'n'play
  gst_pad_link (pad, videopad);

  g_object_unref (videopad);
}

/*
inline char *gennom(QString s, int k) {
  return (char*)s.arg(k).toStdString().c_str();
}
*/

void ElementsComuns::creacomuns(int k, QString ref)
{
    QString sbin("_bin_%1"), stee("_tee_%1"), squeue("_queue_%1"),squeue_m("_queue_mix_%1");

    bin =       gst_bin_new ((char*)sbin.prepend(ref).arg(k).toStdString().c_str());
    tee =       gst_element_factory_make("tee",    (char*)stee.prepend(ref).arg(k).toStdString().c_str());
    queue =     gst_element_factory_make("queue2", (char*)squeue.prepend(ref).arg(k).toStdString().c_str());
    queue_mix = gst_element_factory_make("queue2", (char*)squeue_m.prepend(ref).arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin || !tee || !queue || !queue_mix){
      g_printerr ("Un dels elements de comuns no s'ha pogut crear. Sortint.\n");
    }
}

void EntradaVideo::crea(int k, GstElement *pipeline, const char* type, QSize resolucio, int framerate) {
  //Elements de font d'entrada
  QString ssource("_source_%1"), ssink("_sink_%1");

  creacomuns(k,"video");
  creatransformadors(k);
  source =      gst_element_factory_make(type,          (char*)ssource.prepend("video").arg(k).toStdString().c_str());
  sink =        gst_element_factory_make("xvimagesink", (char*)ssink.prepend("video").arg(k).toStdString().c_str());

  //Comprovem que s'han pogut crear tots els elements d'entrada
  if(!source || !sink){
    g_printerr ("Un dels elements de l'entrada de vídeo no s'ha pogut crear. Sortint.\n");
  }

  //Afegim tots els elements al bin corresponent
  gst_bin_add_many (GST_BIN (bin), source, tee, queue, color_conv, scale, sink, queue_mix, scale_mix, NULL);

  //Afegim els bin al pipeline
  gst_bin_add (GST_BIN (pipeline), bin);

  //Linkem els elements
  caps = gst_caps_new_simple ("video/x-raw-yuv", "width", G_TYPE_INT, resolucio.width(),
                              "height", G_TYPE_INT, resolucio.height(), "framerate", GST_TYPE_FRACTION,framerate, 1, NULL);
  link_elements_with_filter (source,tee,caps);
  gst_element_link_many (tee, queue, scale, sink, NULL);
}

void EntradaVideo::creatransformadors(int k)
{
    QString svconv("colorconverter_%1"), svscale("videoscale_%1"), svscale_m("videoscale_mix_%1");

    color_conv =    gst_element_factory_make("ffmpegcolorspace",(char*)svconv.arg(k).toStdString().c_str());
    scale =         gst_element_factory_make("videoscale",      (char*)svscale.arg(k).toStdString().c_str());
    scale_mix =     gst_element_factory_make("videoscale",      (char*)svscale_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!color_conv || !scale || !scale_mix){
      g_printerr ("Un dels elements transformadors de vídeo no s'ha pogut crear. Sortint.\n");
    }

}

void EntradaFitxer::crea(int k, GstElement *pipeline, QString nom_fitxer)
{
    //Elements de font d'entrada de fitxer
    QString sbin("bin_font%1"),  ssource("source_%1"), sdec("decoder%1"), svolumen_m("volumen_mix%1"), squeue("audio_queue%1");
    QString saconv("audio_conv_%1"), sabin("bin_audio_%1"), sconv("video_conv_%1"), ssink("video_sink_%1");

    //Creem entrada de fitxer i el decodebin, els afegim al pipeline i els linkem.
    bin_font = gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());

    source = gst_element_factory_make ("filesrc",   (char*)ssource.arg(k).toStdString().c_str());
    dec = gst_element_factory_make ("decodebin2",   (char*)sdec.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_font || !source || !dec){
      g_printerr ("Un dels elements de l'entrada de fitxer no s'ha pogut crear. Sortint.\n");
    }

    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_audio), this);
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_video), this);
    gst_bin_add_many (GST_BIN (bin_font), source, dec, NULL);
    gst_element_link (source, dec);

    //Creem l'entrada d'àudio
    a.bin = gst_bin_new ((char*)sabin.arg(k).toStdString().c_str());
    conv_audio =    gst_element_factory_make("audioconvert",    (char*)saconv.arg(k).toStdString().c_str());
    audiopad =      gst_element_get_static_pad (conv_audio, "sink");
    a.queue_mix=    gst_element_factory_make("queue2",          (char*)squeue.arg(k).toStdString().c_str());
    a.volume_mix =  gst_element_factory_make("volume",          (char*)svolumen_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!a.bin || !conv_audio || !audiopad || !a.queue_mix || !a.volume_mix){
      g_printerr ("Un dels elements de l'entrada de fitxer d'àudio no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (a.bin), conv_audio, a.queue_mix, a.volume_mix, NULL);
    gst_element_link_many (conv_audio, a.queue_mix, a.volume_mix, NULL);
    gst_element_add_pad (a.bin, gst_ghost_pad_new ("sink", audiopad));
    gst_object_unref (audiopad);
    gst_bin_add (GST_BIN (bin_font), a.bin);

    //Creem l'entrada de vídeo
    v.creacomuns(k,"video_fitxer");
    v.creatransformadors(k);
    conv_video =    gst_element_factory_make ("ffmpegcolorspace",   (char*)sconv.arg(k).toStdString().c_str());
    videopad =      gst_element_get_static_pad (conv_video,         "sink");
    v.sink =        gst_element_factory_make ("xvimagesink",        (char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if( !videopad || !conv_video || !v.sink){
      g_printerr ("Un dels elements de l'entrada de fitxer de vídeo no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (v.bin), conv_video, v.tee, v.queue, v.scale, v.sink, v.queue_mix, v.color_conv, v.scale_mix, NULL);
    gst_element_link_many (conv_video, v.tee, v.queue, v.scale, v.sink, NULL);
    gst_element_add_pad (v.bin, gst_ghost_pad_new ("sink", videopad));
    gst_object_unref (videopad);
    gst_bin_add (GST_BIN (bin_font), v.bin);

    //Seleccionem el fitxer d'entrada
    const char *c_nom_fitxer = nom_fitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);
    gst_element_set_state(v.sink, GST_STATE_READY);

    //Afegim el bin_video_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_font);
}

void EntradaAudio::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada d'àudio
    QString sbin("bin_audio_%1"), ssource_a("audio_source_%1"), squeue("audio_queue_%1"), svolumen_m("volumen_mix_%1");

    bin = gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());
    source =        gst_element_factory_make("audiotestsrc",    (char*)ssource_a.arg(k).toStdString().c_str());
    queue_mix =     gst_element_factory_make("queue2",          (char*)squeue.arg(k).toStdString().c_str());
    volume_mix =    gst_element_factory_make("volume",          (char*)svolumen_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if (!source || !queue_mix || !volume_mix){
        g_printerr ("Un dels elements comuns no s'ha pogut crear. Sortint.\n");
    }

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (source), "wave",4, NULL);

    //Afegim tots els elements al bin_font corresponent
    gst_bin_add_many (GST_BIN (bin), source, queue_mix, volume_mix, NULL);

    //Afegim els bin_font al pipeline
    gst_bin_add (GST_BIN (pipeline), bin);

    //Linkem els elements
    gst_element_link_many (source, queue_mix, volume_mix,NULL);
}

void EntradaLogo::crea(GstElement *pipeline, QString nomfitxer)
{
    //Creem entrada de fitxer
    bin_logo = gst_bin_new ("bin_logo");
    source =        gst_element_factory_make ("filesrc", "fitxer_logo");
    dec =           gst_element_factory_make ("pngdec", "decoder_logo");
    imagefreeze =   gst_element_factory_make("imagefreeze", "imagefreeze_logo");
    queue =         gst_element_factory_make("queue2","queue_logo");
    conv_logo =     gst_element_factory_make("ffmpegcolorspace", "color_conv_logo");

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_logo || !source || !dec || !imagefreeze || !queue || !conv_logo){
      g_printerr ("Un dels elements de l'entrada de logo no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (bin_logo), source, dec, imagefreeze, queue, conv_logo, NULL);

    const char *c_nom_fitxer = nomfitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);

    //Afegim els elements al pipeline corresponent
    gst_bin_add_many (GST_BIN (pipeline),bin_logo, NULL);

    //Linkem els elements
    gst_element_link_many(source, dec, imagefreeze, queue, conv_logo, NULL);
}

void VideoPGM::crea(int k, GstElement *pipeline){

    QString smixer("videomixer_%1"), ssink("pgm_sink_%1");

    creacomuns(k,"videomixer");
    mixer =         gst_element_factory_make("videomixer" , (char*)smixer.arg(k).toStdString().c_str());
    color_conv =    gst_element_factory_make("ffmpegcolorspace","color_converer_pgm");
    textoverlay =   gst_element_factory_make("textoverlay", "textoverlay");
    sink =          gst_element_factory_make("xvimagesink", (char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements
    if (!mixer || !color_conv || !textoverlay || !sink) {
       g_printerr ("Un dels elements de videoPGM no s'ha pogut crear. Sortint.\n");
    }

    g_object_set(G_OBJECT(textoverlay),"silent",true, NULL);

    //Afegim tots els elements al bin_pgm corresponent
    gst_bin_add_many (GST_BIN (bin), mixer, color_conv, textoverlay, tee, queue, queue_mix, sink, NULL);

    //Afegim el bin_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin);

    //Linkem els elements
    gst_element_link_many(mixer, color_conv, textoverlay, tee, queue, sink, NULL);
    gst_element_link(tee, queue_mix);
}

void AudioPGM::crea(int k, GstElement *pipeline){

    QString smixer("audiomixer_%1"), ssink("pgm_sink_%1");

    creacomuns(k,"audiomixer");
    mixer =    gst_element_factory_make("adder" ,    (char*)smixer.arg(k).toStdString().c_str());
    volum =    gst_element_factory_make("volume",        "volum_pgm");
    sink =     gst_element_factory_make("autoaudiosink", (char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements
    if (!mixer || !volum || !sink) {
       g_printerr ("Un dels elements d'AudioPGM no s'ha pogut crear. Sortint.\n");
    }

    GstCaps *caps;
    caps = gst_caps_new_simple ("audio/x-raw-int","rate", G_TYPE_INT, 44100, "channels", G_TYPE_INT, 2, NULL);
    g_object_set(G_OBJECT(mixer),"caps",caps, NULL);

    //Afegim tots els elements al bin_pgm corresponent
    gst_bin_add_many (GST_BIN (bin), mixer, tee, queue, queue_mix, volum, sink, NULL);

    //Afegim el bin_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin);

    //Linkem els elements
    gst_element_link_many(mixer, tee, queue, volum, sink, NULL);
    gst_element_link(tee, queue_mix);
}

void SortidaFitxer::crea(GstElement *pipeline, GstElement *queue,  GstElement *mux_pgm,
                         const char* type, const char* typeconverter, const char* typeencoder)
{
    QString sbin("_bin_fitxer_%1"), sconv("_fitxer_%1"), sencoder("_encoder_%1");

    bin_fitxer = gst_bin_new ((char*)sbin.prepend(type).toStdString().c_str());
    conv =      gst_element_factory_make(typeconverter, (char*)sconv.prepend(typeconverter).toStdString().c_str());
    encoder =   gst_element_factory_make(typeencoder,   (char*)sencoder.prepend(typeencoder).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_fitxer || !conv || !encoder){
      g_printerr ("Un dels elements de la sortida a fitxer no s'ha pogut crear. Sortint.\n");
    }

    //Afegim tots els elements al bin corresponent
    gst_bin_add_many (GST_BIN (bin_fitxer), conv, encoder, NULL);

    //Afegim els bin_fitxer al pipeline
    gst_bin_add (GST_BIN (pipeline), bin_fitxer);

    //Linkem els elements
    gst_element_link_many (queue, conv, encoder, mux_pgm, NULL);
}

//Mètode que controla el botó capturar
void MainWindow::on_adquirirButton_clicked()
{
    //Creacio dels elements gstreamer
    pipeline = gst_pipeline_new ("video-mixer");

    mux_pgm =       gst_element_factory_make("oggmux",   "multiplexorfitxer");
    sink_fitxer =   gst_element_factory_make("filesink", "fitxerdesortida");

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!pipeline || !mux_pgm || !sink_fitxer){
      g_printerr ("El pipeline, el multiplexor o el sink_fitxer no s'ha pogut crear. Sortint.\n");
    }

    //Afegim els elements al pipeline corresponent
    gst_bin_add_many (GST_BIN (pipeline), mux_pgm, sink_fitxer, NULL);

    for (int k = 0; k < numcam; k++) {
        if(combobox_cam[k]->currentIndex()==2){
          ventrades[k].crea(k, pipeline, "v4l2src",resolucio,framerate);
          aentrades[k].crea(k, pipeline);
        }
        if(combobox_cam[k]->currentIndex()==1){
          if(fontvideo.size()<=k){
                fentrades[k].crea(k, pipeline,fontvideo.at(0));
          }
          else{
            fentrades[k].crea(k, pipeline,fontvideo.at(k));
          }
        }
        if(combobox_cam[k]->currentIndex()==0){
          ventrades[k].crea(k, pipeline, "videotestsrc",resolucio,framerate);
          aentrades[k].crea(k, pipeline);
          //Canvi de el tipus d'imatge test d'alguns elements
          //g_object_set (G_OBJECT (ventrades[k].source), "pattern", k ,"is-live", true, NULL);
        }
    }

    vpgm.crea(0, pipeline);
    apgm.crea(1, pipeline);

    vfitxer.crea(pipeline, vpgm.queue_mix, mux_pgm, "video","ffmpegcolorspace","theoraenc");
    afitxer.crea(pipeline, apgm.queue_mix, mux_pgm, "audio","audioconvert","vorbisenc");

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (vpgm.mixer), "background", 1 , NULL);
    g_object_set (G_OBJECT (sink_fitxer), "location","sortida.ogg" , NULL);

    caps_color = gst_caps_new_simple ("video/x-raw-yuv", "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('A', 'Y', 'U', 'V'), NULL);

    caps_resolucio = gst_caps_new_simple ("video/x-raw-yuv",   "width", G_TYPE_INT, resolucio.width(),
                                                               "height", G_TYPE_INT, resolucio.height(), NULL);

    //Linkem els elements d'entrada amb els de sortida
    for (int k = 0; k < numcam; k++) {
        if(combobox_cam[k]->currentIndex()==1){
            gst_element_link_many (fentrades[k].v.tee, fentrades[k].v.queue_mix, fentrades[k].v.color_conv, NULL);
            link_elements_with_filter(fentrades[k].v.color_conv, fentrades[k].v.scale_mix, caps_color);
            link_elements_with_filter(fentrades[k].v.scale_mix, vpgm.mixer, caps_resolucio);
            gst_element_link_many (fentrades[k].a.volume_mix, apgm.mixer, NULL);
        }
        else{
            gst_element_link_many (ventrades[k].tee, ventrades[k].queue_mix, ventrades[k].color_conv, NULL);
            link_elements_with_filter (ventrades[k].color_conv, ventrades[k].scale_mix, caps_color);
            link_elements_with_filter (ventrades[k].scale_mix, vpgm.mixer, caps_resolucio);
            gst_element_link_many (ventrades[k].scale_mix, vpgm.mixer, NULL);
            gst_element_link_many (aentrades[k].volume_mix, apgm.mixer, NULL);
        }
    }
    gst_element_link(mux_pgm, sink_fitxer);

    //Canviem l'estat del pipeline a "playing"
    g_print ("Now playing: %s\n","Mixer example");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //No acaba de funciona però això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    for (int k = 0; k < numcam; k++) {
        if(combobox_cam[k]->currentIndex()==1){
            gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (fentrades[k].v.sink),gulong(widget_cam[k]->winId()));
        }
        else{
            gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (ventrades[k].sink),gulong(widget_cam[k]->winId()));
        }
        QApplication::syncX();
    }
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (vpgm.sink),gulong(widget_pgm->winId()));
    QApplication::syncX();
    //---------------------------------------------------------------*/
    Label_cam[numcam-1]->setStyleSheet("background-color: rgb(255, 0, 0)");
    ui->stopButton->setEnabled(true);
    ui->moscaButton->setEnabled(true); 
    ui->checkBox_insereixtitol->setEnabled(true);
}

//Mètode que controla el botó stop
void MainWindow::on_stopButton_clicked()
{
    for (int k = 0; k < numcam; k++){
      Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
    }
    if (pipeline != NULL) {
      g_print ("Returned, stopping playback\n");
      gst_element_set_state (pipeline, GST_STATE_NULL);
      g_print ("Deleting pipeline\n");
      gst_object_unref (GST_OBJECT (pipeline));
      pipeline = NULL;

      ui->stopButton->setEnabled(false);
      ui->checkBox_insereixlogo->setChecked(false);
      ui->moscalabel->clear();
      ui->checkBox_insereixtitol->setChecked(false);
    }
}

//Mètode que controla el canvi de càmera
void MainWindow::canviacamara(int camactiva)
{
    QString senyal("sink_%1");
    GstPad *mixerpad;

    for (int k = 0; k < numcam; k++){
        mixerpad=gst_element_get_pad(vpgm.mixer,(char*)senyal.arg(k).toStdString().c_str());

        if(k==camactiva){
            g_object_set (G_OBJECT(mixerpad), "alpha",1.0,NULL);

            Label_cam[k]->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else{
            g_object_set (G_OBJECT(mixerpad), "alpha",0,NULL);

            Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
}

void MainWindow::canviavolum(int valor)
{
    QString nom = sender()->objectName();
    int k=nom[4].digitValue();

    if(combobox_cam[k]->currentIndex()==1){
       g_object_set(G_OBJECT(fentrades[k].a.volume_mix), "volume", double(valor), NULL);
    }
    else{
        g_object_set(G_OBJECT(aentrades[k].volume_mix), "volume", double(valor), NULL);
    }

}

void MainWindow::on_audioSlider_valueChanged(int value)
{
    g_object_set (G_OBJECT (apgm.volum), "volume", double(value) , NULL);
}

void MainWindow::on_gravarButton_clicked()
{
   ui->gravarButton->setStyleSheet("background-color: rgb(255,215,0)");
}

void MainWindow::on_stopButton_2_clicked()
{
    ui->gravarButton->setStyleSheet("background-color");
}

void MainWindow::on_moscaButton_clicked()
{
    imagelogo = QFileDialog::getOpenFileName( this,tr("Seleccioni el logo"),QDir::currentPath(),"Imatges (*.bmp *.png *.xpm *.jpg)");
    QImage  mosca,moscaresize;
    mosca.load (imagelogo);
    moscaresize=mosca.scaled(ui->moscalabel->width(),ui->moscalabel->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->moscalabel->setPixmap(QPixmap::fromImage(moscaresize));

    //--------------------------------------Serveix per crear els de logo
    lentrada.crea(pipeline,imagelogo);
    link_elements_with_filter(lentrada.conv_logo,vpgm.mixer, caps_color);
    gst_element_set_state (lentrada.bin_logo, GST_STATE_PLAYING);
    ui->checkBox_insereixlogo->setChecked(true);
    ui->checkBox_insereixlogo->setEnabled(true);
    //--------------------------------------
}

void MainWindow::on_templatesButton_clicked()
{
    QString imagefilename = QFileDialog::getOpenFileName( this,tr("Seleccioni el Template"),QDir::currentPath(),"Imatges (*.bmp *.png *.xpm *.jpg)");
    QImage  templates,templateresize;
    templates.load (imagefilename);
    templateresize=templates.scaled(ui->templatelabel->width(),ui->templatelabel->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->templatelabel->setPixmap(QPixmap::fromImage(templateresize));
}

void MainWindow::on_addButton_clicked()
{
    if(fontvideo.empty()){
        fontvideo = QFileDialog::getOpenFileNames(this, tr("Selecciona les fonts de Vídeo"),QDir::currentPath(),"Multimedia (*.avi *.mp4 *.mpg *.rm *.ogg *.mov *.asf *.wmv)");
        for (int i = 0; i < fontvideo.size(); ++i){
            QString fitxer=fontvideo.at(i);
            ui->listWidget->addItem(fitxer.remove(0,fitxer.lastIndexOf("/")));
        }
    }
    else{
        QStringList aux=QFileDialog::getOpenFileNames(this, tr("Selecciona les fonts de Vídeo"),QDir::currentPath(),"Multimedia (*.avi *.mp4 *.mpg *.rm *.ogg *.mov *.asf *.wmv)");
        fontvideo=fontvideo+aux;
        for (int i = 0; i < aux.size(); ++i){
            QString fitxer=aux.at(i);
            ui->listWidget->addItem(fitxer.remove(0,fitxer.lastIndexOf("/")));
        }
    }
}

void MainWindow::on_removeButton_clicked()
{   if(!fontvideo.empty()){
        fontvideo.removeAt(ui->listWidget->currentRow());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
        }
}

void MainWindow::on_checkBox_insereixtitol_stateChanged(int check)
{
    if(check==0){
        g_object_set(G_OBJECT(vpgm.textoverlay),"silent",true,NULL);
    }
    if(check==2){
        QString text=ui->plainTextEdit->toPlainText();
        g_object_set(G_OBJECT(vpgm.textoverlay),"silent",false,"font-desc","Calibri,PANGO_STYLE_NORMAL,25.0", "text",text.toStdString().c_str(), NULL);
        g_object_set(G_OBJECT(vpgm.textoverlay),"halignment", 1 ,"valignment", 2 , NULL);
    }
    else{
        return;
    }
}

void MainWindow::on_checkBox_insereixlogo_stateChanged(int check)
{
    QString senyal("sink_%1");
    GstPad *mixerpad;

    mixerpad=gst_element_get_pad(vpgm.mixer,(char*)senyal.arg(numcam).toStdString().c_str());

    if(check==0){
        g_object_set (G_OBJECT(mixerpad), "alpha",0.0,NULL);
    }
    if(check==2){
        g_object_set (G_OBJECT(mixerpad), "alpha",1.0,NULL);

    }
    else{
        return;
    }

}

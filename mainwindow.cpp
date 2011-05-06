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

    createActions();

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio,framerate);
    creainterficie();

    /* Inicialitzacio Gstreamer */
    gst_init (NULL, NULL);
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
        QObject::connect(widget_cam[k], SIGNAL(widgetClicked()), this, SLOT(canviacamara()));

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

static gboolean link_elements_with_filter (GstElement *element1, GstElement *element2, QSize resolucio, int framerate)
{
  gboolean link_ok;
  GstCaps *caps;

  caps = gst_caps_new_simple ("video/x-raw-yuv",
              "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('I', '4', '2', '0'),
              "width", G_TYPE_INT, resolucio.width(),
              "height", G_TYPE_INT, resolucio.height(),
              "framerate", GST_TYPE_FRACTION,framerate, 1,
              NULL);

  link_ok = gst_element_link_filtered (element1, element2, caps);
  gst_caps_unref (caps);

  if (!link_ok) {
    g_warning ("Failed to link element1 and element2!");
  }

  return link_ok;
}

static gboolean link_elements_with_resolucio (GstElement *element1, GstElement *element2, QSize resolucio)
{
  gboolean link_ok;
  GstCaps *caps;

  caps = gst_caps_new_simple ("video/x-raw-yuv",
              "width", G_TYPE_INT, resolucio.width(),
              "height", G_TYPE_INT, resolucio.height(),
              NULL);

  link_ok = gst_element_link_filtered (element1, element2, caps);
  gst_caps_unref (caps);

  if (!link_ok) {
    g_warning ("Failed to link element1 and element2!");
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

inline char *gennom(QString s, int k) {
  return (char*)s.arg(k).toStdString().c_str();
}

void ElementsComuns::creacomuns(int k, QString ref)
{
    QString sbin("bin_%1"), stee("tee_%1"), squeue("queue_%1"),squeue_m("queue_mix%1");

    bin =       gst_bin_new (gennom(sbin+ref, k));
    tee =       gst_element_factory_make("tee",    gennom(stee+ref, k));
    queue =     gst_element_factory_make("queue2", gennom(squeue+ref, k));
    queue_mix = gst_element_factory_make("queue2", gennom(squeue_m+ref, k));

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!tee || !queue || !queue_mix){
      g_printerr ("Un dels elements de comuns no s'ha pogut crear. Sortint.\n");
    }
}

void EntradaVideo::crea(int k, GstElement *pipeline, const char* type, QSize resolucio, int framerate) {
  //Elements de font d'entrada
  QString ssource("source_%1"), ssink("sink_%1");

  creacomuns(k,"video");
  creatransformadors(k);
  source =      gst_element_factory_make(type,          gennom(ssource, k));
  sink =        gst_element_factory_make("xvimagesink", gennom(ssink, k));

  //Comprovem que s'han pogut crear tots els elements d'entrada
  if(!source || !sink){
    g_printerr ("Un dels elements de l'entrada de vídeo no s'ha pogut crear. Sortint.\n");
  }

  //Afegim tots els elements al bin corresponent
  gst_bin_add_many (GST_BIN (bin), source, tee, queue, color_conv, scale, sink, queue_mix, scale_mix, NULL);

  //Afegim els bin al pipeline
  gst_bin_add (GST_BIN (pipeline), bin);

  //Linkem els elements
  link_elements_with_filter (source,tee,resolucio,framerate);
  gst_element_link_many (tee, queue, scale, sink, NULL);
}

void EntradaVideo::creatransformadors(int k)
{
    QString svconv("colorconverter_%1"), svscale("videoscale_%1"), svscale_m("videoscale_mix_%1");

    color_conv =    gst_element_factory_make("ffmpegcolorspace",gennom(svconv, k));
    scale =         gst_element_factory_make("videoscale",      gennom(svscale, k));
    scale_mix =     gst_element_factory_make("videoscale",      gennom(svscale_m, k));

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!color_conv || !scale || !scale_mix){
      g_printerr ("Un dels elements transformadors de vídeo no s'ha pogut crear. Sortint.\n");
    }

}

void EntradaFitxer::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada de fitxer
    QString sbin("bin_font%1"),  ssource("source_%1"), sdec("decoder%1"), svolumen_m("volumen_mix%1"), svolume("volume%1");
    QString saconv("conv_audio%1"), sasink("sink_audio%1"), sconv("conv_video%1"), ssink("sink_%1");

    //Creem entrada de fitxer i el decodebin, els afegim al pipeline i els linkem.
    bin_font = gst_bin_new (gennom(sbin, k));
    source = gst_element_factory_make ("filesrc", gennom(ssource, k));
    dec = gst_element_factory_make ("decodebin2", gennom(sdec, k));

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_font || !source || !dec){
      g_printerr ("Un dels elements de l'entrada de fitxer no s'ha pogut crear. Sortint.\n");
    }

    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_audio), this);
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_video), this);
    gst_bin_add_many (GST_BIN (bin_font), source, dec, NULL);
    gst_element_link (source, dec);

    //Creem l'entrada d'àudio
    a.creacomuns(k,"audio_fitxer");
    conv_audio =    gst_element_factory_make("audioconvert",gennom(saconv, k));
    audiopad =      gst_element_get_static_pad (conv_audio, "sink");
    a.sink =        gst_element_factory_make("autoaudiosink", gennom(sasink, k));
    a.volume =      gst_element_factory_make("volume", gennom(svolume, k));
    a.volume_mix =  gst_element_factory_make("volume", gennom(svolumen_m, k));

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if( !audiopad || !conv_audio || !a.volume || !a.volume_mix || !a.sink){
      g_printerr ("Un dels elements de l'entrada de fitxer d'àudio no s'ha pogut crear. Sortint.\n");
    }

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (a.volume), "mute", true , NULL);

    gst_bin_add_many (GST_BIN (a.bin), conv_audio, a.tee, a.queue, a.volume, a.queue_mix, a.volume_mix, a.sink, NULL);
    gst_element_link_many (conv_audio, a.tee, a.queue, a.volume, a.sink, NULL);
    gst_element_link_many (a.tee,a.queue_mix, a.volume_mix, NULL);
    gst_element_add_pad (a.bin, gst_ghost_pad_new ("sink", audiopad));
    gst_object_unref (audiopad);
    gst_bin_add (GST_BIN (bin_font), a.bin);

    //Creem l'entrada de vídeo
    v.creacomuns(k,"video_fitxer");
    v.creatransformadors(k);
    conv_video =    gst_element_factory_make ("ffmpegcolorspace",   gennom(sconv, k));
    videopad =      gst_element_get_static_pad (conv_video,         "sink");
    v.sink =        gst_element_factory_make ("xvimagesink",        gennom(ssink, k));

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
    QString nom_fitxer = QFileDialog::getOpenFileName();
    const char *c_nom_fitxer = nom_fitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);
    gst_element_set_state(v.sink, GST_STATE_READY);

    //Afegim el bin_video_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_font);
}

void EntradaAudio::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada d'àudio
    QString ssource_a("audio_source_%1"), svolumen("volumen_%1"), svolumen_m("volumen_mix%1"), ssink_audio("sink_audio%1");

    creacomuns(k,"audio");

    source =        gst_element_factory_make("audiotestsrc", gennom(ssource_a, k));
    volume =        gst_element_factory_make("volume", gennom(svolumen, k));
    volume_mix =    gst_element_factory_make("volume", gennom(svolumen_m, k));
    sink =          gst_element_factory_make("autoaudiosink", gennom(ssink_audio, k));

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if (!source || !volume || !volume_mix || !sink){
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (source), "wave",4, NULL);
    g_object_set (G_OBJECT (volume), "mute", true , NULL);

    //Afegim tots els elements al bin_font corresponent
    gst_bin_add_many (GST_BIN (bin), source, tee,  volume, queue, queue_mix, volume_mix, sink, NULL);

    //Afegim els bin_font al pipeline
    gst_bin_add (GST_BIN (pipeline), bin);

    //Linkem els elements
    gst_element_link_many (source, tee, volume, queue, sink, NULL);
    gst_element_link_many (tee, queue_mix, volume_mix,NULL);
}

void SortidaPGM::crea(int k, GstElement *pipeline, const char* type, const char* typesink,bool audio){

    QString smixer("mixer"), ssink("sink_");

    creacomuns(k,"mixer");
    mixer =    gst_element_factory_make(type ,    (char*)smixer.append(type).toStdString().c_str());
    sink =     gst_element_factory_make(typesink, (char*)ssink.append(typesink).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements
    if (!mixer || !sink) {
       g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    //Afegim tots els elements al bin_pgm corresponent
    gst_bin_add_many (GST_BIN (bin), mixer, tee, queue, queue_mix, sink, NULL);

    if(audio){
        volum = gst_element_factory_make("volume","volumen_pgm");
        gst_bin_add(GST_BIN (bin), volum);
    }

    //Afegim el bin_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin);

    //Linkem els elements
    if(audio){
        gst_element_link_many(mixer, tee, queue, volum, sink, NULL);
    }
    else{
        gst_element_link_many(mixer, tee, queue, sink, NULL);
    }
    gst_element_link(tee, queue_mix);
}

void SortidaFitxer::crea(GstElement *pipeline, GstElement *queue,  GstElement *mux_pgm,
                         const char* type, const char* typeconverter, const char* typeencoder)
{
    QString sbin("bin_fitxer_"), sconv("conv_fitxer_"), sencoder("encoder_");

    bin_fitxer = gst_bin_new ((char*)sbin.append(type).toStdString().c_str());
    conv =      gst_element_factory_make(typeconverter, (char*)sconv.append(typeconverter).toStdString().c_str());
    encoder =   gst_element_factory_make(typeencoder,   (char*)sencoder.append(typeencoder).toStdString().c_str());

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
          fentrades[k].crea(k, pipeline);
        }
        if(combobox_cam[k]->currentIndex()==0){
          ventrades[k].crea(k, pipeline, "videotestsrc",resolucio,framerate);
          aentrades[k].crea(k, pipeline);
          //Canvi de el tipus d'imatge test d'alguns elements
          //g_object_set (G_OBJECT (ventrades[k].source), "pattern", k ,"is-live", true, NULL);
        }
    }

    vpgm.crea(0, pipeline,"videomixer","xvimagesink", false);
    apgm.crea(1, pipeline,"liveadder","autoaudiosink",true);

    vfitxer.crea(pipeline, vpgm.queue_mix, mux_pgm, "video","ffmpegcolorspace","theoraenc");
    afitxer.crea(pipeline, apgm.queue_mix, mux_pgm, "audio","audioconvert","vorbisenc");

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (vpgm.mixer), "background", 1 , NULL);
    g_object_set (G_OBJECT (sink_fitxer), "location","sortida" , NULL);

    //Linkem els elements d'entrada amb els de sortida
    for (int k = 0; k < numcam; k++) {
        if(combobox_cam[k]->currentIndex()==1){
            gst_element_link_many (fentrades[k].v.tee, fentrades[k].v.queue_mix, fentrades[k].v.color_conv, fentrades[k].v.scale_mix, NULL);
            link_elements_with_resolucio(fentrades[k].v.scale_mix, vpgm.mixer,resolucio);
            gst_element_link_many (fentrades[k].a.volume_mix, apgm.mixer, NULL);
        }
        else{
            gst_element_link_many (ventrades[k].tee, ventrades[k].queue_mix, ventrades[k].color_conv, ventrades[k].scale_mix, NULL);
            link_elements_with_resolucio(ventrades[k].scale_mix, vpgm.mixer, resolucio);
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
 }

//Mètode que controla el botó stop
void MainWindow::on_stopButton_clicked()
{
    g_print ("Returned, stopping playback\n");

    gst_element_set_state (pipeline, GST_STATE_NULL);
    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (pipeline));
}

//Mètode que controla el canvi de càmera
void MainWindow::canviacamara()
{
    QString nom = sender()->objectName();
    QString senyal("sink_%1");
    GstPad *mixerpad;

    for (int k = 0; k < numcam; k++){
        if(k==nom[4].digitValue()){
            mixerpad=gst_element_get_pad(vpgm.mixer,gennom(senyal, k));
            g_object_set (G_OBJECT(mixerpad), "alpha",1.0,NULL);

            Label_cam[k]->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else{
            Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");

            mixerpad=gst_element_get_pad(vpgm.mixer, gennom(senyal, k));
            g_object_set (G_OBJECT(mixerpad), "alpha",0,NULL);
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
    QString imagefilename = QFileDialog::getOpenFileName( this,tr("Seleccioni el logo"),QDir::currentPath(),"Imatges (*.bmp *.png *.xpm *.jpg)");
    QImage  mosca,moscaresize;
    mosca.load (imagefilename);
    moscaresize=mosca.scaled(ui->moscalabel->width(),ui->moscalabel->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->moscalabel->setPixmap(QPixmap::fromImage(moscaresize));
}

void MainWindow::on_templatesButton_clicked()
{
    QString imagefilename = QFileDialog::getOpenFileName( this,tr("Seleccioni el Template"),QDir::currentPath(),"Imatges (*.bmp *.png *.xpm *.jpg)");
    QImage  templates,templateresize;
    templates.load (imagefilename);
    templateresize=templates.scaled(ui->templatelabel->width(),ui->templatelabel->height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->templatelabel->setPixmap(QPixmap::fromImage(templateresize));
}



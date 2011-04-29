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
    createMenus();

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio);
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

//Mètode que genera el menu
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}


//Mètode que controla l'acció nou fitxer
void MainWindow::newFile()
{
    finishCameras();
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio);
    creainterficie();
}

//Métode que tanca tots els Layers de càmeres.
void MainWindow::finishCameras()
{
  for (int k = 0; k < numcam; k++) {
    delete Label_cam[k];
    delete combobox_cam[k];
    delete widget_cam[k];
  }
  delete widget_pgm;
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
    newAct = new QAction(tr("&Nova"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Crear un nova gravació"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("Sortir"), this);
    exitAct->setShortcut(tr("Ctrl+S"));
    exitAct->setStatusTip(tr("Sortir de l'aplicació"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("Ajuda de Capturadora"), this);
    aboutAct->setStatusTip(tr("Mostra la ajuda de l'aplicació"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("Sobre"), this);
    aboutQtAct->setStatusTip(tr("Mostra informació de l'aplicació"));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

}

//Mètode que crea el menu de l'aplicació
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Captura"));
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edicio"));
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ajuda"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

//---------------------------------------------------------------------------------------------------------
//Haurien d'estar dins la classe MainWindow i crec que haurien de ser video_[k] i audio_[k]
//---------------------------------------------------------------------------------------------------------

static void cb_newpad_audio (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *audiopad;

  /* Linkem només una vegada */
  audiopad = gst_element_get_static_pad (grup->audiobin, "sink");
  if (GST_PAD_IS_LINKED (audiopad)) {
    g_object_unref (audiopad);
    return;
  }

  /* Mirem els tipus de dades multimedia */
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "audio")) {
    gst_caps_unref (caps);
    gst_object_unref (audiopad);
    return;
  }
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, audiopad);

  g_object_unref (audiopad);
}

static void cb_newpad_video (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;

  /* Linkem només una vegada */
  videopad = gst_element_get_static_pad (grup->videobin, "sink");
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }

  /* Mirem els tipus de dades multimedia*/
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, videopad);

  g_object_unref (videopad);
}

void EntradaVideo::crea(int k, GstElement *pipeline, const char* type) {
  //Elements de font d'entrada
  QString sbin("bin_font%1"), ssource("source_%1"), stee("tee_%1"), squeue("queue_%1");
  QString squeue_m("queue_mix%1"),ssink("sink_font%1");

  bin_font =  gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());
  source =    gst_element_factory_make(type,          (char*)ssource.arg(k).toStdString().c_str());
  tee =       gst_element_factory_make("tee",         (char*)stee.arg(k).toStdString().c_str());
  queue =     gst_element_factory_make("queue",       (char*)squeue.arg(k).toStdString().c_str());
  queue_mix = gst_element_factory_make("queue",       (char*)squeue_m.arg(k).toStdString().c_str());
  sink =      gst_element_factory_make("xvimagesink", (char*)ssink.arg(k).toStdString().c_str());

  /*Comprovem que s'han pogut crear tots els elements d'entrada*/
  if(!bin_font || !source || !tee || !queue || !queue_mix || !sink){
    g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
  }

  /* Afegim tots els elements al bin_font corresponent */
  gst_bin_add_many (GST_BIN (bin_font), source, tee, queue, queue_mix, sink, NULL);

  /* Afegim els bin_font al pipeline */
  gst_bin_add (GST_BIN (pipeline), bin_font);

  /*Linkem els elements */
  gst_element_link_many (source, tee, queue, sink, NULL);
}

void EntradaFitxer::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada
    QString sbin("bin_font%1"), ssource("source_%1"), stee("tee_%1"), squeue("queue_%1");
    QString squeue_m("queue_mix%1"), ssink("sink_font%1"), svolume("volume%1");
    QString sconv_audio("conv_audio%1"), ssink_audio("sink_audio%1"), sconv_video("conv_video%1");
    QString sdec("decoder%1");

    /* Creem el bin_font */
    v.bin_font = gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());

    /* Creem els elements i els afegim al bin */
    //Creem entrada de fitxer i el decodebin, els afegim al pipeline i els linkem.
    v.source = gst_element_factory_make ("filesrc", (char*)ssource.arg(k).toStdString().c_str());
    dec = gst_element_factory_make ("decodebin", (char*)sdec.arg(k).toStdString().c_str());

    //------------------------------------------------------------------------------------------------
    //Aquí hauríem de poder enviar paràmetres problema amb gpointer i void*
    //------------------------------------------------------------------------------------------------
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_audio), this);
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_video), this);
    gst_bin_add_many (GST_BIN (v.bin_font), v.source, dec, NULL);
    gst_element_link (v.source, dec);

    // Creem la sortida d'àudio
    audiobin =    gst_bin_new ("audiobin");
    conv_audio =  gst_element_factory_make("audioconvert",(char*)sconv_audio.arg(k).toStdString().c_str());
    audiopad =    gst_element_get_static_pad (conv_audio, "sink");
    a.sink =      gst_element_factory_make("autoaudiosink", (char*)ssink_audio.arg(k).toStdString().c_str());
    a.tee =       gst_element_factory_make("tee",    (char*)stee.arg(k).toStdString().c_str());
    a.volume =    gst_element_factory_make("volume", (char*)svolume.arg(k).toStdString().c_str());
    a.queue =     gst_element_factory_make("queue",  (char*)squeue.arg(k).toStdString().c_str());
    a.queue_mix = gst_element_factory_make("queue",  (char*)squeue_m.arg(k).toStdString().c_str());

    /*Canvi de les propietats d'alguns elements */
    g_object_set (G_OBJECT (a.volume), "mute", true , NULL);

    gst_bin_add_many (GST_BIN (audiobin), conv_audio, a.tee, a.queue, a.queue_mix, a.volume, a.sink, NULL);
    gst_element_link_many (conv_audio, a.tee, a.volume, a.queue, a.sink, NULL);
    gst_element_add_pad (audiobin, gst_ghost_pad_new ("sink", audiopad));
    gst_object_unref (audiopad);
    gst_bin_add (GST_BIN (a.bin_font), audiobin);
}

/*
    // Creem la sortida de vídeo
    videobin =   gst_bin_new ("videobin");

    conv_video = gst_element_factory_make ("ffmpegcolorspace", (char*)sconv_video.arg(k).toStdString().c_str());
    videopad =   gst_element_get_static_pad (conv_video, "sink");

    v.tee =      gst_element_factory_make ("tee", (char*)stee.arg(k).toStdString().c_str());
    queue =      gst_element_factory_make("queue", (char*)squeue.arg(k).toStdString().c_str());
    queue_mix =  gst_element_factory_make("queue", (char*)squeue_m.arg(k).toStdString().c_str());

    sink =       gst_element_factory_make ("xvimagesink",(char*)ssink.arg(k).toStdString().c_str());

    gst_bin_add_many (GST_BIN (videobin), conv_video, tee, queue, queue_mix, sink, NULL);

    gst_element_link_many (conv_video, tee, queue, sink, NULL);

    gst_element_add_pad (videobin, gst_ghost_pad_new ("sink", videopad));
    gst_object_unref (videopad);

    gst_bin_add (GST_BIN (bin_font), videobin);

    // Seleccionem el fitxer d'entrada
    QString nom_fitxer = QFileDialog::getOpenFileName();
    const char *c_nom_fitxer = nom_fitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);
    gst_element_set_state(sink, GST_STATE_READY);

    // Afegim el bin_video_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_font);

}
*/

void EntradaAudio::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada
    QString sbin_audio("bin_audio_font%1"), ssource_a("audio_source_%1"), stee_a("tee_audio%1"), squeue_a("queue_audio%1");
    QString svolumen("volumen_%1"), squeue_m_a("queue_mix%1"), ssink_audio("sink_audio%1");

    bin_font =  gst_bin_new ((char*)sbin_audio.arg(k).toStdString().c_str());
    source =    gst_element_factory_make("audiotestsrc", (char*)ssource_a.arg(k).toStdString().c_str());
    tee =       gst_element_factory_make("tee", (char*)stee_a.arg(k).toStdString().c_str());
    volume =    gst_element_factory_make("volume", (char*)svolumen.arg(k).toStdString().c_str());
    queue =     gst_element_factory_make("queue", (char*)squeue_a.arg(k).toStdString().c_str());
    queue_mix = gst_element_factory_make("queue", (char*)squeue_m_a.arg(k).toStdString().c_str());
    sink =      gst_element_factory_make("autoaudiosink", (char*)ssink_audio.arg(k).toStdString().c_str());

    /*Comprovem que s'han pogut crear tots els elements d'entrada*/
    if (!bin_font || !source || !tee || !queue || !queue_mix || !sink){
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    /*Canvi de les propietats d'alguns elements */
    g_object_set (G_OBJECT (source), "wave",5, NULL);
    g_object_set (G_OBJECT (volume), "mute", true , NULL);

    /* Afegim tots els elements al bin_font corresponent */
    gst_bin_add_many (GST_BIN (bin_font), source, tee,  volume, queue, queue_mix, sink, NULL);

    /* Afegim els bin_font al pipeline */
    gst_bin_add (GST_BIN (pipeline), bin_font);

    /*Linkem els elements */
    gst_element_link_many (source, tee, volume, queue, sink, NULL);

}

//Mètode que controla el botó capturar
void MainWindow::on_adquirirButton_clicked()
{
    /* Creacio dels elements gstreamer*/
    pipeline = gst_pipeline_new ("video-mixer");

    for (int k = 0; k < numcam; k++) {

        if(combobox_cam[k]->currentIndex()==2){
          ventrades[k].crea(k, pipeline, "v4l2src");
          aentrades[k].crea(k, pipeline);
        }
        if(combobox_cam[k]->currentIndex()==1){
          fentrades[k].crea(k, pipeline);
        }
        if(combobox_cam[k]->currentIndex()==0){
          ventrades[k].crea(k, pipeline, "videotestsrc");
          aentrades[k].crea(k, pipeline);
        }
    }

    //Elements de mix de vídeo i PGM
    bin_video_pgm = gst_bin_new ("bin_video_pgm");

    videomixer = gst_element_factory_make("videomixer", "videomixer");

    tee_video_pgm = gst_element_factory_make ("tee", "tee_video_pgm");
    queue_video_pgm = gst_element_factory_make("queue", "queue_video_pgm");
    sink_video_pgm = gst_element_factory_make("xvimagesink", "sink_video_pgm");

    /*Comprovem que s'han pogut crear tots els elements */
    if (!pipeline || !videomixer || !tee_video_pgm || !queue_video_pgm || !sink_video_pgm) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    /*Canvi de les propietats d'alguns elements */
    g_object_set (G_OBJECT (videomixer), "background", 1 , NULL);

    /* Afegim tots els elements al bin_video_pgm corresponent */
    gst_bin_add_many (GST_BIN (bin_video_pgm), videomixer, tee_video_pgm, queue_video_pgm, sink_video_pgm, NULL);

    /* Afegim el bin_video_pgm al pipeline */
    gst_bin_add (GST_BIN (pipeline),bin_video_pgm);

    //Elements de mix d'àudio i PGM
    bin_audio_pgm = gst_bin_new ("bin_audio_pgm");

    audiomixer = gst_element_factory_make("liveadder", "audiomixer");
    volume_audio_pgm = gst_element_factory_make ("volume", "volume_audio_pgm");

    tee_audio_pgm = gst_element_factory_make ("tee", "tee_audio_pgm");
    queue_audio_pgm = gst_element_factory_make("queue", "queue_audio_pgm");
    sink_audio_pgm = gst_element_factory_make("autoaudiosink", "sink_audio_pgm");

    /*Comprovem que s'han pogut crear tots els elements */
    if (!audiomixer || !tee_audio_pgm || !queue_audio_pgm || !volume_audio_pgm || !sink_audio_pgm) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    /* Afegim tots els elements al bin_audio_pgm corresponent */
    gst_bin_add_many (GST_BIN (bin_audio_pgm), audiomixer, tee_audio_pgm, queue_audio_pgm, volume_audio_pgm, sink_audio_pgm, NULL);

    /* Afegim el bin_audio_pgm al pipeline */
    gst_bin_add (GST_BIN (pipeline),bin_audio_pgm);

    //----------------------------------------------------------------------------------------------------------------------
    //Això hauria d'anar a on_gravarButton_clicked() però no sé com es linka dinàmicament
    //----------------------------------------------------------------------------------------------------------------------
    bin_fitxer_pgm = gst_bin_new ("bin_fitxer_pgm");

    queue_video_fitxer = gst_element_factory_make("queue", "queue_video_fitxer");
    conv_video_pgm = gst_element_factory_make ("ffmpegcolorspace","color-converter");
    encoder_video_pgm = gst_element_factory_make ("theoraenc", "video_encoder");;
    mux_pgm = gst_element_factory_make ("oggmux", "ogg_mux");;
    sink_fitxer = gst_element_factory_make ("filesink", "file_output");

    //Comprovem que s'han pogut crear tots els elements
    if (!queue_video_fitxer || !conv_video_pgm || !encoder_video_pgm || !mux_pgm || !sink_fitxer) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }
    //Establim el nom del fitxer de sortida
    g_object_set (G_OBJECT(sink_fitxer), "location", "sortida.ogg", NULL);
    gst_bin_add_many (GST_BIN (bin_fitxer_pgm),queue_video_fitxer, conv_video_pgm, encoder_video_pgm, mux_pgm, sink_fitxer, NULL);

    // Afegim el bin_fitxer_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_fitxer_pgm);

    // Linkem els elements entre ells
    gst_element_link_many (tee_video_pgm, queue_video_fitxer, conv_video_pgm, encoder_video_pgm, mux_pgm, sink_fitxer, NULL);

    //----------------------------------------------------------------------------------------------------------------------

    /* Linkem els elements d'entrada entre ells */
    /* source_1 -> tee_1 -> queue_1 -> sink_1
       source_2 -> tee_2 -> queue_2 -> sink_2
                            queue -> video-mixer -> tee -> queue -> video-output
                                                                 -> conv -> encoder -> mux -> sink*/
    for (int k = 0; k < numcam; k++) {
        gst_element_link_many (ventrades[k].tee, ventrades[k].queue_mix, videomixer, NULL);
        gst_element_link_many (aentrades[k].tee, aentrades[k].queue_mix, audiomixer, NULL);
    }

    gst_element_link_many(videomixer,tee_video_pgm, queue_video_pgm, sink_video_pgm, NULL);
    gst_element_link_many(audiomixer, tee_audio_pgm, queue_audio_pgm, volume_audio_pgm, sink_audio_pgm, NULL);

    /* Canviem l'estat del pipeline a "playing" */
    g_print ("Now playing: %s\n","Mixer example");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //No acaba de funciona però això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    for (int k = 0; k < numcam; k++) {
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (ventrades[k].sink),gulong(widget_cam[k]->winId()));
        QApplication::syncX();
    }
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_video_pgm),gulong(widget_pgm->winId()));
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
            mixerpad=gst_element_get_pad(videomixer,(char*)senyal.arg(k).toStdString().c_str());
            g_object_set (G_OBJECT(mixerpad), "alpha",1.0,NULL);

            Label_cam[k]->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else{
            Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");

            mixerpad=gst_element_get_pad(videomixer, (char*)senyal.arg(k).toStdString().c_str());
            g_object_set (G_OBJECT(mixerpad), "alpha",0,NULL);
        }
    }
}

void MainWindow::on_gravarButton_clicked()
{
    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    ui->gravarButton->setStyleSheet("background-color: rgb(255,215,0)");

    /*
    bin_fitxer_pgm = gst_bin_new ("bin_fitxer_pgm");

    queue_video_fitxer = gst_element_factory_make("queue", "queue_video_fitxer");
    conv_video_pgm = gst_element_factory_make ("ffmpegcolorspace","color-converter");
    encoder_video_pgm = gst_element_factory_make ("theoraenc", "video_encoder");;
    mux_pgm = gst_element_factory_make ("oggmux", "ogg_mux");;
    sink_fitxer = gst_element_factory_make ("filesink", "file_output");

    //Comprovem que s'han pogut crear tots els elements
    if (!queue_video_fitxer || !conv_video_pgm || !encoder_video_pgm || !mux_pgm || !sink_fitxer) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }
    //Establim el nom del fitxer de sortida
    g_object_set (G_OBJECT(sink_fitxer), "location", "sortida.ogg", NULL);
    gst_bin_add_many (GST_BIN (bin_fitxer_pgm),queue_video_fitxer, conv_video_pgm, encoder_video_pgm, mux_pgm, sink_fitxer, NULL);

    // Afegim el bin_fitxer_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_fitxer_pgm);

    // Linkem els elements entre ells
    gst_element_link_many (tee_video_pgm, queue_video_fitxer, conv_video_pgm, encoder_video_pgm, mux_pgm, sink_fitxer, NULL);
    */
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

void MainWindow::on_audioSlider_valueChanged(int value)
{
 g_object_set (G_OBJECT (volume_audio_pgm), "volume", double(value) , NULL);
}

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
    ui->Button_insereixMosca->setEnabled(false);
    ui->Button_insereixtitol->setEnabled(false);

    createActions();

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio,framerate);
    creainterficie();

    /* Inicialitzacio Gstreamer */
    gst_init (NULL, NULL);
    pipeline = NULL;
    lentrada.bin_logo = NULL;
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
        slideraudio[k]->setStyleSheet("QSlider::groove:vertical {border-image: url(:/slider/images/fonsslider.png);}"
                                       "QSlider::handle:vertical {image: url(:/slider/images/sliderboto.png);}");
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

static gboolean message_handler (GstBus * bus, GstMessage * message, gpointer data)
{
  QVUMeter *grup = (QVUMeter*)data;

  if (message->type == GST_MESSAGE_ELEMENT) {
    const GstStructure *s = gst_message_get_structure (message);
    const gchar *name = gst_structure_get_name (s);

    if (strcmp (name, "level") == 0) {
      gint channels,i;
      gdouble rms_dB, rms;
      const GValue *list, *value;

      /* we can get the number of channels as the length of any of the value lists */
      list = gst_structure_get_value (s, "rms");
      channels = gst_value_list_get_size (list);

      for (i = 0; i < channels; i++) {
        //g_print ("channel %d\n", i);
        list = gst_structure_get_value (s, "rms");
        value = gst_value_list_get_value (list, i);
        rms_dB = g_value_get_double (value);
        /* converting from dB to normal gives us a value between 0.0 and 1.0 */
        rms = pow (10, rms_dB / 20);
        //g_print ("    normalized rms value: %f\n", rms);
        if(i==0){
            grup->setLeftValue(rms);
        }
        else{
            grup->setRightValue(rms);
        }
      }
    }
  }
  /* we handled the message we want, and ignored the ones we didn't want. So the core can unref the message for us */
  return TRUE;
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

//Mètode que controla el botó capturar
void MainWindow::on_adquirirButton_clicked()
{
    //Creacio dels elements gstreamer
    pipeline = gst_pipeline_new ("video-mixer");

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, message_handler, ui->qvumeter);
    gst_object_unref (bus);

    mux_pgm =       gst_element_factory_make("avimux",   "multiplexorfitxer");
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
          if(fontvideo.empty()){
              QMessageBox::warning(this, tr("Video System"),
                                              tr("Has escollit entrada des de arxiu multimèdia.\n"
                                                 "Però no hi ha fitxers. Carrega fitxers abans d'adquirir."),
                                                  QMessageBox::Ok);
                return;
          }
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
    g_object_set (G_OBJECT (sink_fitxer), "location","sortida.avi" , NULL);

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
    ui->audioSlider->setEnabled(true);
    ui->Button_insereixtitol->setEnabled(true);
    ui->adquirirButton->setEnabled(false);
    double vol;
    g_object_get(G_OBJECT(apgm.volum),"volume",&vol, NULL);
    ui->audioSlider->setValue(vol);
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
      ui->audioSlider->setEnabled(false);
      ui->Button_insereixMosca->setChecked(false);
      ui->moscalabel->clear();
      ui->Button_insereixMosca->setEnabled(false);
      ui->Button_insereixtitol->setChecked(false);
      ui->adquirirButton->setEnabled(true);
      ui->audioSlider->setSliderPosition(0);
      ui->qvumeter->setLeftValue(0);
      ui->qvumeter->setRightValue(0);
    }
}

//Mètode que controla el canvi de càmera
void MainWindow::canviacamara(int camactiva)
{
    if (pipeline != NULL) {
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
}

void MainWindow::canviavolum(int valor)
{
    if (pipeline != NULL) {
        QString nom = sender()->objectName();
        int k=nom[4].digitValue();

        if(combobox_cam[k]->currentIndex()==1){
           g_object_set(G_OBJECT(fentrades[k].a.volume_mix), "volume", double(valor), NULL);
        }
        else{
            g_object_set(G_OBJECT(aentrades[k].volume_mix), "volume", double(valor), NULL);
        }
    }
}

void MainWindow::on_audioSlider_valueChanged(int value)
{
    if (pipeline != NULL) {
        g_object_set (G_OBJECT (apgm.volum), "volume", double(value) , NULL);
    }
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
    ui->Button_insereixMosca->setEnabled(true);
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
{
    if(!fontvideo.empty()){
        fontvideo.removeAt(ui->listWidget->currentRow());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
}

void MainWindow::on_Button_insereixtitol_toggled(bool checked)
{
    if (pipeline != NULL) {
        if(!checked){
            g_object_set(G_OBJECT(vpgm.textoverlay),"silent",true,NULL);
        }
        if(checked){
            QString text=ui->plainTextEdit->toPlainText();
            g_object_set(G_OBJECT(vpgm.textoverlay),"silent",false,"font-desc","Calibri,PANGO_STYLE_NORMAL,25.0", "text",text.toStdString().c_str(), NULL);
            g_object_set(G_OBJECT(vpgm.textoverlay),"halignment", 1 ,"valignment", 2 , NULL);
        }
        else{
            return;
        }
    }
}

void MainWindow::on_Button_insereixMosca_toggled(bool checked)
{
    if (pipeline != NULL) {
        QString senyal("sink_%1");
        GstPad *mixerpad;

        mixerpad=gst_element_get_pad(vpgm.mixer,(char*)senyal.arg(numcam).toStdString().c_str());

        if(!checked){
            //Eliminar els elements de logo;
            gst_element_release_request_pad     (vpgm.mixer,mixerpad);
            gst_element_unlink (lentrada.conv_logo,vpgm.mixer);
            gst_element_set_state (lentrada.bin_logo, GST_STATE_NULL);
            gst_bin_remove(GST_BIN (pipeline),lentrada.bin_logo);
            lentrada.bin_logo = NULL;
        }
        if(checked && lentrada.bin_logo == NULL){
            //--------------------------------------Serveix per crear els elements de logo
            lentrada.crea(pipeline,imagelogo);
            link_elements_with_filter(lentrada.conv_logo,vpgm.mixer, caps_color);
            gst_element_set_state (lentrada.bin_logo, GST_STATE_PLAYING);
            //--------------------------------------
        }
        else{
            return;
        }
    }
}

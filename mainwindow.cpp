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
        list_boxcam<<"Video Test"<<"Fitxer"<< "Camera";
        combobox_cam[k]->addItems(list_boxcam);

        widget_cam[k] = new QWidget(ui->centralWidget,0);
        widget_cam[k]->setStyleSheet("background-color: rgb(0, 0, 0)");
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

//Mètode que controla el botó capturar
void MainWindow::on_adquirirButton_clicked()
{
    /* Creacio dels elements gstreamer*/
    pipeline = gst_pipeline_new ("video-mixer");

    //Elements de font d'entrada
    QString bin("bin_font%1"), source("source_%1"), tee("tee_%1"), queue("queue_%1");
    QString queue_m("queue_mix%1"),sink("sink_font%1");

    for (int k = 0; k < numcam; k++) {
        bin_font[k] = gst_bin_new ((char*)bin.arg(k).toStdString().c_str());
        source_[k] = gst_element_factory_make ("videotestsrc", (char*)source.arg(k).toStdString().c_str());
        tee_[k] = gst_element_factory_make ("tee", (char*)tee.arg(k).toStdString().c_str());
        queue_[k] = gst_element_factory_make("queue", (char*)queue.arg(k).toStdString().c_str());
        queue_mix[k] = gst_element_factory_make("queue", (char*)queue_m.arg(k).toStdString().c_str());
        sink_[k] = gst_element_factory_make ("xvimagesink", (char*)sink.arg(k).toStdString().c_str());

        /*Comprovem que s'han pogut crear tots els elements d'entrada*/
        if(!bin_font[k] || !source_[k] || !tee_[k] || !queue_[k] || !queue_mix[k] || !sink_[k]){
          g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
        }

        /* Afegim tots els elements al bin_font corresponent */
        gst_bin_add_many (GST_BIN (bin_font[k]), source_[k], tee_[k], queue_[k], queue_mix[k], sink_[k], NULL);

        /* Afegim els bin_font al pipeline */
        gst_bin_add (GST_BIN (pipeline), bin_font[k]);
    }

    //Elements de mix i PGM
    bin_pgm = gst_bin_new ("bin_pgm");

    videomixer = gst_element_factory_make("videomixer", "videomixer");

    tee_pgm = gst_element_factory_make ("tee", "tee_pgm");
    queue_pgm = gst_element_factory_make("queue", "queue_pgm");
    sink_pgm = gst_element_factory_make("xvimagesink", "sink_pgm");

    /*Comprovem que s'han pogut crear tots els elements */
    if (!pipeline || !videomixer || !tee_pgm || !queue_pgm || !sink_pgm) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }

    /*Canvi de les propietats d'alguns elements */
    g_object_set (G_OBJECT (source_[0]), "pattern", 15 , NULL);

    g_object_set (G_OBJECT (videomixer), "background", 1 , NULL);


    /* Afegim tots els elements al bin_pgm corresponent */
    gst_bin_add_many (GST_BIN (bin_pgm), videomixer, tee_pgm, queue_pgm, sink_pgm, NULL);

    /* Afegim el bin_pgm al pipeline */
    gst_bin_add (GST_BIN (pipeline),bin_pgm);

    /* Linkem els elements d'entrada entre ells */
    /* source_1 -> tee_1 -> queue_1 -> sink_1
       source_2 -> tee_2 -> queue_2 -> sink_2
                            queue -> video-mixer -> tee -> queue -> video-output
                                                                 -> conv -> encoder -> mux -> sink*/
    for (int k = 0; k < numcam; k++) {
        gst_element_link_many (source_[k], tee_[k], queue_[k], sink_[k], NULL);
        gst_element_link_many (tee_[k], queue_mix[k], videomixer, NULL);
    }
    gst_element_link(videomixer,tee_pgm);
    gst_element_link_many (tee_pgm, queue_pgm, sink_pgm, NULL);

    /* Canviem l'estat del pipeline a "playing" */
    g_print ("Now playing: %s\n","Mixer example");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //No acaba de funciona per això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    for (int k = 0; k < numcam; k++) {
        gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_[k]),gulong(widget_cam[k]->winId()));
        QApplication::syncX();
    }
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_pgm),gulong(widget_pgm->winId()));
    //---------------------------------------------------------------*/
 }

 //Mètode que controla el botó stop
void MainWindow::on_stopButton_clicked()
{
  for (int k = 0; k < numcam; k++) {

  }
}


void MainWindow::on_gravarButton_clicked()
{
    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    ui->gravarButton->setStyleSheet("background-color: rgb(255,215,0)");

    bin_fitxer_pgm = gst_bin_new ("bin_fitxer_pgm");

    queue_fitxer = gst_element_factory_make("queue", "queue_fitxersortida");
    conv_video_pgm = gst_element_factory_make ("ffmpegcolorspace","color-converter");
    encoder_pgm = gst_element_factory_make ("theoraenc", "video_encoder");;
    mux_pgm = gst_element_factory_make ("oggmux", "ogg_mux");;
    sink_fitxer = gst_element_factory_make ("filesink", "file_output");

    /*Comprovem que s'han pogut crear tots els elements */
    if (!queue_fitxer || !conv_video_pgm || !encoder_pgm || !mux_pgm || !sink_fitxer) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    }
    /*Establim el nom del fitxer de sortida */
    g_object_set (G_OBJECT(sink_fitxer), "location", "sortida.ogg", NULL);
    gst_bin_add_many (GST_BIN (bin_fitxer_pgm),queue_fitxer, conv_video_pgm, encoder_pgm, mux_pgm, sink_fitxer, NULL);

    /* Afegim el bin_fitxer_pgm al pipeline */
    gst_bin_add (GST_BIN (pipeline),bin_fitxer_pgm);

    /* Linkem els elements entre ells */
    gst_element_link_many (tee_pgm, queue_fitxer, conv_video_pgm, encoder_pgm, mux_pgm, sink_fitxer, NULL);
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

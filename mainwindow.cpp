//Implementaci� de la clase MainWindow

#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


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
}

//Destructor de la clase MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
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

//M�tode que genera la interf�cie gr�fica segons el nombre de c�meres
void MainWindow::creainterficie()
{

    // Declaraci� variables gr�fiques

    QString nom("CAM %1");

    for (int k=0;k<numcam;k++){
        Label_cam[k] = new QLabel(tr(nom.arg(k + 1).toAscii()));
        Label_cam[k]->setAlignment(Qt::AlignCenter);
        Label_cam[k]->setFont(QFont("arial", 10, QFont::Bold));
        Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
    }

    Label_pgm = new QLabel(tr("PGM"));
    Label_pgm->setAlignment(Qt::AlignCenter);

    glWidget_pgm = new PGMWidget();
    ui->gridLayout->addWidget(Label_pgm,1,3);
    ui->verticalLayout_PGM->addWidget(glWidget_pgm);

    for (int k = 0; k < numcam; k++) {
      glWidget_cam[k] = new GLWidget(glWidget_pgm);
      glWidget_cam[k]->setObjectName(nom.arg(k).toAscii());
      QObject::connect(glWidget_cam[k], SIGNAL(widgetClicked()),
                       this,            SLOT(canviacamara()));
    }

    //Switch() de posicionament dels diferents widgets a la finestra

    switch(numcam)

    {
    default:
        ui->gridLayout->addWidget(Label_cam[3],3,2);
        ui->gridLayout->addWidget(glWidget_cam[3],4,2);

    case 3:
        ui->gridLayout->addWidget(Label_cam[2],3,1);
        ui->gridLayout->addWidget(glWidget_cam[2],4,1);

    case 2:
        ui->gridLayout->addWidget(Label_cam[0],1,1);
        ui->gridLayout->addWidget(glWidget_cam[0],2,1);

        ui->gridLayout->addWidget(Label_cam[1],1,2);
        ui->gridLayout->addWidget(glWidget_cam[1],2,2);

        break;
    case 1:
        ui->gridLayout->addWidget(Label_cam[0],1,1,1,2);
        ui->gridLayout->addWidget(glWidget_cam[0],2,1,4,2);
    }

    ui->gridLayout->setColumnMinimumWidth(1,400);
    ui->gridLayout->setColumnMinimumWidth(2,400);
    ui->gridLayout->setColumnMinimumWidth(3,400);

    ui->gridLayout->setRowMinimumHeight(2,300);
    ui->gridLayout->setRowMinimumHeight(4,300);
}

//M�tode que genera el menu
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}

//M�tode que tanca tots els GLWidgets i Layers de c�meres.
void MainWindow::finishCameras()
{
  delete Label_pgm;
  glWidget_pgm->finishRendering();

  for (int k = 0; k < numcam; k++) {
    glWidget_cam[k]->finishRendering();
    delete glWidget_cam[k];
    delete Label_cam[k];
    }
  delete glWidget_pgm;
}

//M�tode que controla l'acci� nou fitxer
void MainWindow::newFile()
{
    finishCameras();

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio);
    creainterficie();
}

//M�tode que controla l'acci� about
void MainWindow::about()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Ajuda de Capturadora"));
}

//M�tode que controla l'acci� aboutQt
void MainWindow::aboutQt()
{
    QMessageBox::about(this, tr("Sobre Capturadora"),
            tr("Sobre capturadora, versi� 1, Tots els drets reservats"));
}

//M�tode que crea les diferents accions de la finestra principal
void MainWindow::createActions()
{
    newAct = new QAction(tr("&Nova"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Crear un nova gravaci�"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("Sortir"), this);
    exitAct->setShortcut(tr("Ctrl+S"));
    exitAct->setStatusTip(tr("Sortir de l'aplicaci�"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("Ajuda de Capturadora"), this);
    aboutAct->setStatusTip(tr("Mostra la ajuda de l'aplicaci�"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("Sobre"), this);
    aboutQtAct->setStatusTip(tr("Mostra informaci� de l'aplicaci�"));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

}

//M�tode que crea el menu de l'aplicaci�
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Captura"));
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edici�"));
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ajuda"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

//M�tode que controla el bot� capturar
void MainWindow::on_adquirirButton_clicked()
{
    //Inici del QThread d'adquisici� que renderitza a GLWidget el senyal d'entrada
    for (int k = 0; k < numcam; k++) {
       glWidget_cam[k]->finishRendering();
    }

    QStringList fontvideo = QFileDialog::getOpenFileNames(this, tr("Selecciona les fonts de V�deo"),QDir::currentPath(),"Multimedia (*.avi *.mp4 *.mpg *.rm *.ogm *.mov *.asf *.wmv)");
    if (fontvideo.isEmpty()){
        return;
    }

    for (int k = 0; k < numcam; k++) {
      capture[k] = cvCaptureFromFile(fontvideo[k].toAscii());
      if (!capture[k]) return;
    }

    for (int k = 0; k < numcam; k++) {
      QString nom("CAM %1");
      glWidget_cam[k]->initadquirir(capture[k], nom.arg(k).toAscii());
    }

    //Inici del QThread de gravaci� que grava a fitxer i renderitza a PGMWidget el senyal de PGM
    glWidget_pgm->initPGM(resolucio,25,ui->spinBox_duracio->value());
    connect(ui->comboBox_tipus, SIGNAL(activated (int)),glWidget_pgm->pthreadgravar,SLOT(selectransicio(int)));
    connect(ui->spinBox_duracio, SIGNAL(valueChanged (int)),glWidget_pgm->pthreadgravar,SLOT(selecduratransicio(int)));

    //-----------------------------------------------------------
    //Labels de comprovaci� s'han de treure en el programa final
    //-----------------------------------------------------------
    for (int k = 0; k < numcam; k++) {
      TempsQthread[k] = new QLabel();
      TempscvQuery[k] = new QLabel();
      ui->gridLayout_3->addWidget(TempscvQuery[k],k+1,1);
      ui->gridLayout_3->addWidget(TempsQthread[k],k+1,2);
      connect(glWidget_cam[k]->pthreadrender, SIGNAL(MostraTempsQT(int,double)),
              this,                  SLOT(MostraTempsQT(int,double)));
      connect(glWidget_cam[k]->pthreadrender, SIGNAL(MostraTempscvQuery(int,double)),
              this,                  SLOT(MostraTempscvQuery(int,double)));
    }
    //-----------------------------------------------------------
}

//M�tode que controla el bot� stop
void MainWindow::on_stopButton_clicked()
{
  for (int k = 0; k < numcam; k++) {
    glWidget_cam[k]->finishRendering();
  }
}

//M�tode que controla el canvi de c�mera
void MainWindow::canviacamara()
{
    if(ui->comboBox_tipus->currentIndex()>0){
        glWidget_pgm->pthreadgravar->activatransicio(true);
    }
    QString nom = sender()->objectName();

    for (int k = 0; k < numcam; k++){
        glWidget_cam[k]->canviacamactiva(nom);

        if(k==nom[4].digitValue()){
            Label_cam[k]->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else{
            Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
}


void MainWindow::on_gravarButton_clicked()
{
    //Activaci� de la gravaci� en el thread de gravaci�

    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    //QString nomdeprojecte =("sortida.avi");
    glWidget_pgm->pthreadgravar->setgravar(true);
}

void MainWindow::on_stopButton_2_clicked()
{
    glWidget_pgm->finishGravar();
}

//-----------------------------------------------------------
//M�todes de comprovaci� s'han de treure en el programa final
//-----------------------------------------------------------
void MainWindow::MostraTempsQT(int cam,double timebucle)
{
   TempsQthread[cam]->setNum(timebucle);
}

void MainWindow::MostraTempscvQuery(int cam,double time){
   TempscvQuery[cam]->setNum(time);
}
//-----------------------------------------------------------------

//Implementació de la clase MainWindow

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
    numcam=configdialog->get_config();
    creainterficie();

}

//Destructor de la clase MainWindow
MainWindow::~MainWindow()
{
    delete ui;
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

//Funció menbre que genera la interfície gràfica segons el nombre de càmeres
void MainWindow::creainterficie()
{
    // Declaració variables gráfiques

    QString nom("CAM %1");

    for (int k=0;k<numcam;k++){
        Label_cam[k] = new QLabel(tr(nom.arg(k).toAscii()));
        Label_cam[k]->setAlignment(Qt::AlignCenter);
        Label_cam[k]->setFont(QFont("arial", 10, QFont::Bold));
        Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
    }

    Label_pgm = new QLabel(tr("PGM"));
    Label_pgm->setAlignment(Qt::AlignCenter);

    glWidget_pgm = new GLWidget();
    ui->gridLayout->addWidget(Label_pgm,1,3);
    ui->verticalLayout_PGM->addWidget(glWidget_pgm);

    for (int k = 0; k < numcam; k++) {
      glWidget_cam[k] = new GLWidget();
      glWidget_cam[k]->setObjectName(nom.arg(k).toAscii());
      QObject::connect(glWidget_cam[k], SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
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

//Funció menbre que genera el menu
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}

//Funció menbre que controla l'acció nou fitxer
void MainWindow::newFile()
{
    Label_pgm->~QLabel();
    glWidget_pgm->glt.terminate();
    glWidget_pgm->~QGLWidget();

    for (int k = 0; k < numcam; k++) {
      glWidget_cam[k]->glt.terminate();
      glWidget_cam[k]->~QGLWidget();
      Label_cam[k]->~QLabel();
    }

    Dialog *configdialog = new Dialog;
    configdialog->exec();
    numcam=configdialog->get_config();
    creainterficie();

}

//Funció menbre que controla l'acció about
void MainWindow::about()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Ajuda de Capturadora"));
}

//Funció menbre que controla l'acció aboutQt
void MainWindow::aboutQt()
{
    QMessageBox::about(this, tr("Sobre Capturadora"),
            tr("Sobre capturadora, versió 1, Tots els drets reservats"));
}

//Funció menbre que crea les diferents accions de la finestra principal
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

//Funció menbre que crea el menu de l'aplicació
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Captura"));
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edició"));
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ajuda"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

//Funció menbre que controla el botó capturar
void MainWindow::on_adquirirButton_clicked()
{
    QString fontvideo[numwidgets];

    for (int k = 0; k < numcam; k++) {
      fontvideo[k] = QFileDialog::getOpenFileName();
      capture[k] = cvCaptureFromFile(fontvideo[k].toAscii());
      if (!capture[k]) return;
    }

    for (int k = 0; k < numcam; k++) {
      QString nom("CAM %1");
      glWidget_cam[k]->initadquirir(capture[k], nom.arg(k).toAscii());
    }
}

//Funció menbre que controla el botó stop
void MainWindow::on_stopButton_clicked()
{
  for (int k = 0; k < numcam; k++) {
    glWidget_cam[k]->finishRendering();
  }
}

//Funció menbre que controla el canvi de càmera
void MainWindow::canviacamara()
{  
    QString canvi=sender()->objectName();
    
    for (int k = 0; k < numcam; k++){
        glWidget_cam[k]->canviacamactiva(canvi);

        if(k==canvi[4].digitValue()){
            Label_cam[k]->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else{
             Label_cam[k]->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
}


void MainWindow::on_gravarButton_clicked()
{
    //Creació dels thread de gravació

    QString nomdeprojecte = QFileDialog::getSaveFileName();

    //QString nomdeprojecte =("sortida.avi");

    Gravarthread = new GravarThread(this,nomdeprojecte,ui->comboBox_tipus->currentIndex(),25);

    connect(ui->comboBox_tipus, SIGNAL(activated (int)),Gravarthread,SLOT(selectransicio(int)));

    connect(ui->spinBox_duracio, SIGNAL(valueChanged (int)),Gravarthread,SLOT(selecduratransicio(int)));

    for (int k = 0; k < numcam; k++){
      connect(&glWidget_cam[k]->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
    }

    Gravarthread->start();

}

void MainWindow::on_stopButton_2_clicked()
{
    Gravarthread->terminate();
}

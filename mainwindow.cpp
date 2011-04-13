//Implementaci� de la clase MainWindow

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

    ui->gridLayout->addWidget(Label_pgm,1,3);


    //Switch() de posicionament dels diferents widgets a la finestra

    switch(numcam)

    {
    default:
        ui->gridLayout->addWidget(Label_cam[3],3,2);

    case 3:
        ui->gridLayout->addWidget(Label_cam[2],3,1);

    case 2:
        ui->gridLayout->addWidget(Label_cam[0],1,1);

        ui->gridLayout->addWidget(Label_cam[1],1,2);

        break;
    case 1:
        ui->gridLayout->addWidget(Label_cam[0],1,1,1,2);
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


//M�tode que controla l'acci� nou fitxer
void MainWindow::newFile()
{
    finishCameras();
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    configdialog->get_config(numcam,resolucio);
    creainterficie();
}

//M�tode que tanca tots els Layers de c�meres.
void MainWindow::finishCameras()
{
  for (int k = 0; k < numcam; k++) {
    delete Label_cam[k];
  }
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
    for (int k = 0; k < numcam; k++) {

    }
}

//M�tode que controla el bot� stop
void MainWindow::on_stopButton_clicked()
{
  for (int k = 0; k < numcam; k++) {

  }
}


void MainWindow::on_gravarButton_clicked()
{
    //Activaci� de la gravaci� en el thread de gravaci�

    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    //QString nomdeprojecte =("sortida.avi");
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

#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "configdialog.h"
#include "CapturaThread.h"

#include <cxcore.h>
#include <cv.h>
#include <highgui.h>


int numcam;

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
    // declaracio variables gráfiques

    cam1Label = new QLabel(tr("CAM 1"));
    cam1Label->setAlignment(Qt::AlignCenter);
    cam1Label->setFont(QFont("arial", 10, QFont::Bold));

    cam2Label = new QLabel(tr("CAM 2"));
    cam2Label->setAlignment(Qt::AlignCenter);
    cam2Label->setFont(QFont("arial", 10, QFont::Bold));

    cam3Label = new QLabel(tr("CAM 3"));
    cam3Label->setAlignment(Qt::AlignCenter);
    cam3Label->setFont(QFont("arial", 10, QFont::Bold));

    cam4Label = new QLabel(tr("CAM 4"));
    cam4Label->setAlignment(Qt::AlignCenter);
    cam4Label->setFont(QFont("arial", 10, QFont::Bold));

    pgmLabel = new QLabel(tr("PGM"));
    pgmLabel->setAlignment(Qt::AlignCenter);


    //comença switch()

    switch(numcam)

    {
    case 1:
        //QMessageBox::information(this, "Informació", "Has entrat en case 1, una Càmera");

        glWidget_cam1 = new GLWidget();
        glWidget_pgm = new GLWidget();

        QObject::connect(glWidget_cam1, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(pgmLabel,1,2);
        ui->gridLayout->addWidget(glWidget_pgm,2,2);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    case 2:
        //QMessageBox::information(this, "Informació", "Has entrat en case 2, dues Càmeres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_pgm = new GLWidget();

        QObject::connect(glWidget_cam1, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam2, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);
        
        ui->gridLayout->addWidget(cam2Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam2,4,1);

        ui->gridLayout->addWidget(pgmLabel,1,2);
        ui->gridLayout->addWidget(glWidget_pgm,2,2);

        ui->gridLayout->setColumnMinimumWidth(1,395);
        ui->gridLayout->setColumnMinimumWidth(2,395);
        ui->gridLayout->setColumnMinimumWidth(3,395);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    case 3:
        //QMessageBox::information(this, "Informació", "Has entrat en case 3, tres Càmeres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_cam3 = new GLWidget();
        glWidget_pgm = new GLWidget();

        QObject::connect(glWidget_cam1, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam2, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam3, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(cam3Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam3,4,1);

        ui->gridLayout->addWidget(cam2Label,1,2);
        ui->gridLayout->addWidget(glWidget_cam2,2,2);

        ui->gridLayout->addWidget(pgmLabel,1,3);
        ui->gridLayout->addWidget(glWidget_pgm,2,3);

        ui->gridLayout->setColumnMinimumWidth(1,395);
        ui->gridLayout->setColumnMinimumWidth(2,395);
        ui->gridLayout->setColumnMinimumWidth(3,395);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    default:
        //QMessageBox::information(this, "Informacin", "Has entrat en case default, quatre Càmeres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_cam3 = new GLWidget();
        glWidget_cam4 = new GLWidget();
        glWidget_pgm = new GLWidget();

        QObject::connect(glWidget_cam1, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam2, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam3, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));
        QObject::connect(glWidget_cam4, SIGNAL(mousePressEvent(QMouseEvent *)),this, SLOT(cambiarcamara(QMouseEvent *)));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(cam3Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam3,4,1);

        ui->gridLayout->addWidget(cam2Label,1,2);
        ui->gridLayout->addWidget(glWidget_cam2,2,2);

        ui->gridLayout->addWidget(cam4Label,3,2);
        ui->gridLayout->addWidget(glWidget_cam4,4,2);

        ui->gridLayout->addWidget(pgmLabel,1,3);
        ui->gridLayout->addWidget(glWidget_pgm,2,3);

        ui->gridLayout->setColumnMinimumWidth(1,395);
        ui->gridLayout->setColumnMinimumWidth(2,395);
        ui->gridLayout->setColumnMinimumWidth(3,395);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");

    }
}

//Funció menbre que genera el menu
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}


void MainWindow::newFile()
{
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    numcam=configdialog->get_config();
    creainterficie();

}


void MainWindow::about()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Ajuda de Capturadora"));
}


void MainWindow::aboutQt()
{
    QMessageBox::about(this, tr("Sobre Capturadora"),
            tr("Sobre capturadora, versió 1, Tots els drets reservats"));
}


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




void MainWindow::on_capturaButton_clicked()
{
    QString fontdevideo1;
    QString fontdevideo2;
    QString fontdevideo3;
    QString fontdevideo4;

    //Creació dels thread de gravació

    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    QString nomdeprojecte =("salida.avi");

    Gravarthread = new GravarThread(this,nomdeprojecte,ui->comboBox->currentIndex (),25);

    connect(ui->comboBox, SIGNAL(activated (int)),Gravarthread,SLOT(selectransicio(int)));

    //connect(ui->spinBox, SIGNAL(valueChanged (int)),Gravarthread,SLOT(selecduratransicio(int)));

    Gravarthread->start();


    switch(numcam)
    {
    case 1:
       //Creació dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1);

       fontdevideo1 = QFileDialog::getOpenFileName();
       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();

       QObject::connect(Capturathread1, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));

       break;

    case 2:
       //Creació dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2);

       QObject::connect(Capturathread1, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread2, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));

       fontdevideo1 = QFileDialog::getOpenFileName();
       fontdevideo2 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();
       Capturathread2->selecfontvideo(fontdevideo2);
       Capturathread2->start();
       break;

    case 3:
       //Creació dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2);
       Capturathread3 = new CapturaThread(this,"cam3",glWidget_cam3);

       QObject::connect(Capturathread1, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread2, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread3, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));


       fontdevideo1 = QFileDialog::getOpenFileName();
       fontdevideo2 = QFileDialog::getOpenFileName();
       fontdevideo3 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();
       Capturathread2->selecfontvideo(fontdevideo2);
       Capturathread2->start();
       Capturathread3->selecfontvideo(fontdevideo3);
       Capturathread3->start();

       break;

    default:
       //Creació dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2);
       Capturathread3 = new CapturaThread(this,"cam3",glWidget_cam3);
       Capturathread4 = new CapturaThread(this,"cam4",glWidget_cam4);

       QObject::connect(Capturathread1, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread2, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread3, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));
       QObject::connect(Capturathread4, SIGNAL(mostrarframe(IplImage *, char *,GLWidget *)),this, SLOT(processCam(IplImage *, char *,GLWidget *)));


       fontdevideo1 = QFileDialog::getOpenFileName();
       fontdevideo2 = QFileDialog::getOpenFileName();
       fontdevideo3 = QFileDialog::getOpenFileName();
       fontdevideo4 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();
       Capturathread2->selecfontvideo(fontdevideo2);
       Capturathread2->start();
       Capturathread3->selecfontvideo(fontdevideo3);
       Capturathread3->start();
       Capturathread4->selecfontvideo(fontdevideo4);
       Capturathread4->start();

       }

}

void MainWindow::on_stopButton_clicked()
{
    Capturathread1->terminate();
    Capturathread2->terminate();
    Capturathread3->terminate();
    Capturathread4->terminate();
}

void MainWindow::processCam(IplImage *frame, char *camera,GLWidget *glWidgetcam) {

   if (frame->imageData) {
            //this->processFrame(frame);
            glWidgetcam->sendImage(frame);
            ui->statusBar->showMessage("Adquirint....");
    }
    return;
}


void MainWindow::cambiarcamara(QMouseEvent *)
{
    qDebug()<<"Has clicat un widget";
   switch (1)
   {
        case 1:
            cam1Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            break;

        case 2:
            cam2Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            break;

        case 3:
            cam3Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");

            break;

        default:
            cam4Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");

   }
}

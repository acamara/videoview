//Implementaci� de la clase MainWindow

#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "configdialog.h"
#include "CapturaThread.h"

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

//Funci� menbre que genera la interf�cie gr�fica segons el nombre de c�meres
void MainWindow::creainterficie()
{
    // declaracio variables gr�fiques

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


    //comen�a switch()

    switch(numcam)

    {
    case 1:
        //QMessageBox::information(this, "Informaci�", "Has entrat en case 1, una C�mera");

        glWidget_cam1 = new GLWidget();
        glWidget_pgm = new GLWidget();

        glWidget_cam1->setObjectName("cam1");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(pgmLabel,1,2);
        ui->gridLayout->addWidget(glWidget_pgm,2,2);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    case 2:
        //QMessageBox::information(this, "Informaci�", "Has entrat en case 2, dues C�meres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_pgm = new GLWidget();

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));

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
        //QMessageBox::information(this, "Informaci�", "Has entrat en case 3, tres C�meres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_cam3 = new GLWidget();
        glWidget_pgm = new GLWidget();

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");
        glWidget_cam3->setObjectName("cam3");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));

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
        //QMessageBox::information(this, "Informacin", "Has entrat en case default, quatre C�meres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_cam3 = new GLWidget();
        glWidget_cam4 = new GLWidget();
        glWidget_pgm = new GLWidget();

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");
        glWidget_cam3->setObjectName("cam3");
        glWidget_cam4->setObjectName("cam4");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));
        QObject::connect(glWidget_cam4, SIGNAL(widgetClicked()),this, SLOT(cambiarcamara()));

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

//Funci� menbre que genera el menu
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}

//Funci� menbre que controla l'acci� nou fitxer
void MainWindow::newFile()
{
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    numcam=configdialog->get_config();
    creainterficie();

}

//Funci� menbre que controla l'acci� about
void MainWindow::about()
{
    QMessageBox::about(this, tr("Ajuda"),tr("Ajuda de Capturadora"));
}

//Funci� menbre que controla l'acci� aboutQt
void MainWindow::aboutQt()
{
    QMessageBox::about(this, tr("Sobre Capturadora"),
            tr("Sobre capturadora, versi� 1, Tots els drets reservats"));
}

//Funci� menbre que crea les diferents accions de la finestra principal
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

//Funci� menbre que crea el menu de l'aplicaci�
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

//Funci� menbre que controla el bot� capturar
void MainWindow::on_capturaButton_clicked()
{
    QString fontdevideo1;
    QString fontdevideo2;
    QString fontdevideo3;
    QString fontdevideo4;

    //Creaci� dels thread de gravaci�

    //QString nomdeprojecte = QFileDialog::getSaveFileName();

    QString nomdeprojecte =("salida.avi");

    Gravarthread = new GravarThread(this,nomdeprojecte,ui->comboBox->currentIndex (),25);

    connect(ui->comboBox, SIGNAL(activated (int)),Gravarthread,SLOT(selectransicio(int)));

    connect(ui->spinBox, SIGNAL(valueChanged (int)),Gravarthread,SLOT(selecduratransicio(int)));

    Gravarthread->start();


    switch(numcam)
    {
    case 1:
       //Creaci� dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1,glWidget_pgm);

       fontdevideo1 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();

       QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),Capturathread1, SLOT(selecfontPGM()));

       break;

    case 2:
       //Creaci� dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1,glWidget_pgm);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2,glWidget_pgm);

       fontdevideo1 = QFileDialog::getOpenFileName();
       fontdevideo2 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();
       Capturathread2->selecfontvideo(fontdevideo2);
       Capturathread2->start();

       QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),Capturathread1, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),Capturathread2, SLOT(selecfontPGM()));

       break;

    case 3:
       //Creaci� dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1,glWidget_pgm);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2,glWidget_pgm);
       Capturathread3 = new CapturaThread(this,"cam3",glWidget_cam3,glWidget_pgm);

       fontdevideo1 = QFileDialog::getOpenFileName();
       fontdevideo2 = QFileDialog::getOpenFileName();
       fontdevideo3 = QFileDialog::getOpenFileName();

       Capturathread1->selecfontvideo(fontdevideo1);
       Capturathread1->start();
       Capturathread2->selecfontvideo(fontdevideo2);
       Capturathread2->start();
       Capturathread3->selecfontvideo(fontdevideo3);
       Capturathread3->start();

       QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),Capturathread1, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),Capturathread2, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),Capturathread3, SLOT(selecfontPGM()));

       break;

    default:
       //Creaci� dels threads de captura
       Capturathread1 = new CapturaThread(this,"cam1",glWidget_cam1,glWidget_pgm);
       Capturathread2 = new CapturaThread(this,"cam2",glWidget_cam2,glWidget_pgm);
       Capturathread3 = new CapturaThread(this,"cam3",glWidget_cam3,glWidget_pgm);
       Capturathread4 = new CapturaThread(this,"cam4",glWidget_cam4,glWidget_pgm);

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

       QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),Capturathread1, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),Capturathread2, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),Capturathread3, SLOT(selecfontPGM()));
       QObject::connect(glWidget_cam4, SIGNAL(widgetClicked()),Capturathread4, SLOT(selecfontPGM()));

       }

}

//Funci� menbre que controla el bot� stop
void MainWindow::on_stopButton_clicked()
{
    switch(numcam)
    {
    case 1:
        Capturathread1->terminate();

    break;

    case 2:
        Capturathread1->terminate();
        Capturathread2->terminate();

    break;

    case 3:
        Capturathread1->terminate();
        Capturathread2->terminate();
        Capturathread3->terminate();

    break;

    default:
        Capturathread1->terminate();
        Capturathread2->terminate();
        Capturathread3->terminate();
        Capturathread4->terminate();

    break;
    }
}

//Funci� menbre que controla el canvi de c�mera
void MainWindow::cambiarcamara()
{  
      if(sender()->objectName()=="cam1")
      {
            cam1Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");
      }

      if(sender()->objectName()=="cam2")
      {
            cam2Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");
      }

      if(sender()->objectName()=="cam3")
      {
            cam3Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");

      }

      if(sender()->objectName()=="cam4")
      {
            cam4Label->setStyleSheet("background-color: rgb(255, 0, 0)");

            cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
            cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
      }
}

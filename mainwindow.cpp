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

//Mètode que genera la interfície gràfica segons el nombre de càmeres
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

    glWidget_pgm = new GLWidget();
    ui->gridLayout->addWidget(pgmLabel,1,3);
    ui->verticalLayout_PGM->addWidget(glWidget_pgm);

    //comença switch()

    switch(numcam)

    {
    case 1:
        //QMessageBox::information(this, "Informació", "Has entrat en case 1, una Càmera");

        glWidget_cam1 = new GLWidget();

        glWidget_cam1->setObjectName("cam1");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),glWidget_cam1, SLOT(canviacamactiva()));

        ui->gridLayout->addWidget(cam1Label,1,1,1,2);
        ui->gridLayout->addWidget(glWidget_cam1,2,1,4,2);

        ui->gridLayout->setColumnMinimumWidth(1,400);
        ui->gridLayout->setColumnMinimumWidth(2,400);
        ui->gridLayout->setColumnMinimumWidth(3,400);

        ui->gridLayout->setRowMinimumHeight(2,300);
        ui->gridLayout->setRowMinimumHeight(4,300);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    case 2:
        //QMessageBox::information(this, "Informació", "Has entrat en case 2, dues Càmeres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),glWidget_cam1, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),glWidget_cam2, SLOT(canviacamactiva()));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);
        
        ui->gridLayout->addWidget(cam2Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam2,4,1);

        ui->gridLayout->setColumnMinimumWidth(1,400);
        ui->gridLayout->setColumnMinimumWidth(2,400);
        ui->gridLayout->setColumnMinimumWidth(3,400);

        ui->gridLayout->setRowMinimumHeight(2,300);
        ui->gridLayout->setRowMinimumHeight(4,300);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");

        break;

    case 3:
        //QMessageBox::information(this, "Informació", "Has entrat en case 3, tres Càmeres");

        glWidget_cam1 = new GLWidget();
        glWidget_cam2 = new GLWidget();
        glWidget_cam3 = new GLWidget();

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");
        glWidget_cam3->setObjectName("cam3");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),glWidget_cam1, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),glWidget_cam2, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),glWidget_cam3, SLOT(canviacamactiva()));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(cam3Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam3,4,1);

        ui->gridLayout->addWidget(cam2Label,1,2);
        ui->gridLayout->addWidget(glWidget_cam2,2,2);

        ui->gridLayout->setColumnMinimumWidth(1,400);
        ui->gridLayout->setColumnMinimumWidth(2,400);
        ui->gridLayout->setColumnMinimumWidth(3,400);

        ui->gridLayout->setRowMinimumHeight(2,300);
        ui->gridLayout->setRowMinimumHeight(4,300);

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

        glWidget_cam1->setObjectName("cam1");
        glWidget_cam2->setObjectName("cam2");
        glWidget_cam3->setObjectName("cam3");
        glWidget_cam4->setObjectName("cam4");

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));
        QObject::connect(glWidget_cam4, SIGNAL(widgetClicked()),this, SLOT(canviacamara()));

        QObject::connect(glWidget_cam1, SIGNAL(widgetClicked()),glWidget_cam1, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam2, SIGNAL(widgetClicked()),glWidget_cam2, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam3, SIGNAL(widgetClicked()),glWidget_cam3, SLOT(canviacamactiva()));
        QObject::connect(glWidget_cam4, SIGNAL(widgetClicked()),glWidget_cam4, SLOT(canviacamactiva()));

        ui->gridLayout->addWidget(cam1Label,1,1);
        ui->gridLayout->addWidget(glWidget_cam1,2,1);

        ui->gridLayout->addWidget(cam3Label,3,1);
        ui->gridLayout->addWidget(glWidget_cam3,4,1);

        ui->gridLayout->addWidget(cam2Label,1,2);
        ui->gridLayout->addWidget(glWidget_cam2,2,2);

        ui->gridLayout->addWidget(cam4Label,3,2);
        ui->gridLayout->addWidget(glWidget_cam4,4,2);

        ui->gridLayout->setColumnMinimumWidth(1,400);
        ui->gridLayout->setColumnMinimumWidth(2,400);
        ui->gridLayout->setColumnMinimumWidth(3,400);

        ui->gridLayout->setRowMinimumHeight(2,300);
        ui->gridLayout->setRowMinimumHeight(4,300);

        cam1Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam2Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam3Label->setStyleSheet("background-color: rgb(255, 255, 255)");
        cam4Label->setStyleSheet("background-color: rgb(255, 255, 255)");

    }
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
    Dialog *configdialog = new Dialog;
    configdialog->exec();
    numcam=configdialog->get_config();
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

    editMenu = menuBar()->addMenu(tr("&Edició"));
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ajuda"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

}

//Mètode que controla el botó adquirir
void MainWindow::on_adquirirButton_clicked()
{
    QString fontvideo1,fontvideo2,fontvideo3,fontvideo4;

    switch(numcam)
    {
    case 1:
        fontvideo1= QFileDialog::getOpenFileName();
        capture= cvCaptureFromFile(fontvideo1.toAscii());
        if (!capture) return;
        glWidget_cam1->initRendering(capture,"cam1");

       break;

    case 2:
       fontvideo1= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo1.toAscii());
       if (!capture) return;
       glWidget_cam1->initRendering(capture,"cam1");

       fontvideo2= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo2.toAscii());
       if (!capture) return;
       glWidget_cam2->initRendering(capture,"cam2");

       break;

    case 3:
       fontvideo1= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo1.toAscii());
       if (!capture) return;
       glWidget_cam1->initRendering(capture,"cam1");

       fontvideo2= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo2.toAscii());
       if (!capture) return;
       glWidget_cam2->initRendering(capture,"cam2");

       fontvideo3= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo3.toAscii());
       if (!capture) return;
       glWidget_cam3->initRendering(capture,"cam3");

       break;

    default:
       fontvideo1= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo1.toAscii());
       if (!capture) return;
       glWidget_cam1->initRendering(capture,"cam1");

       fontvideo2= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo2.toAscii());
       if (!capture) return;
       glWidget_cam2->initRendering(capture,"cam2");

       fontvideo3= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo3.toAscii());
       if (!capture) return;
       glWidget_cam3->initRendering(capture,"cam3");

       fontvideo4= QFileDialog::getOpenFileName();
       capture= cvCaptureFromFile(fontvideo4.toAscii());
       if (!capture) return;
       glWidget_cam4->initRendering(capture,"cam4");

       break;
       }

}

//Mètode que controla el botó stop
void MainWindow::on_stopButton_clicked()
{
    switch(numcam)
    {
    case 1:
        glWidget_cam1->finishRendering();
        break;

    case 2:
        glWidget_cam1->finishRendering();
        glWidget_cam2->finishRendering();
        break;

    case 3:
        glWidget_cam1->finishRendering();
        glWidget_cam2->finishRendering();
        glWidget_cam3->finishRendering();
        break;

    default:
        glWidget_cam1->finishRendering();
        glWidget_cam2->finishRendering();
        glWidget_cam3->finishRendering();
        glWidget_cam4->finishRendering();
        break;
    }
}

//Mètode que controla el canvi de càmera
void MainWindow::canviacamara()
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

//Mètode que controla el botó gravar
void MainWindow::on_gravarButton_clicked()
{
    //Creació dels thread de gravació

    QString nomdeprojecte = QFileDialog::getSaveFileName();

    //QString nomdeprojecte =("salida.avi");

    Gravarthread = new GravarThread(this,nomdeprojecte,ui->comboBox_tipus->currentIndex(),25);

    connect(ui->comboBox_tipus, SIGNAL(activated (int)),Gravarthread,SLOT(selectransicio(int)));

    connect(ui->spinBox_duracio, SIGNAL(valueChanged (int)),Gravarthread,SLOT(selecduratransicio(int)));

    switch(numcam)
    {
        case 1:
            connect(&glWidget_cam1->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
        break;
        case 2:
            connect(&glWidget_cam1->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
            connect(&glWidget_cam2->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
        break;
        case 3:
            connect(&glWidget_cam1->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
            connect(&glWidget_cam2->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
            connect(&glWidget_cam3->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
        default:
           connect(&glWidget_cam1->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
           connect(&glWidget_cam2->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
           connect(&glWidget_cam3->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
           connect(&glWidget_cam4->glt, SIGNAL(enviaragravar(IplImage *)),Gravarthread,SLOT(rebregravar(IplImage *)));
        break;
    }

    Gravarthread->start();

}

//Mètode que controla el botó stop gravació
void MainWindow::on_stopButton_2_clicked()
{
    Gravarthread->terminate();
}

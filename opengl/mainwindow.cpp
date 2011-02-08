#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glWidget = new GLWidget();

    this->ui->verticalLayout->addWidget(glWidget);

    connect(ui->actionAdquirir,SIGNAL(triggered()),this,SLOT(startCam()));
    connect(ui->actionParar,SIGNAL(triggered()),this,SLOT(stopCam()));

}

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

void MainWindow::startCam() {
    QString video= QFileDialog::getOpenFileName();
    capture= cvCaptureFromFile(video.toAscii());
    if (!capture) return;
    parar=false;
    this->processCam();

}
void MainWindow::stopCam() {
    parar=true;
    ui->statusBar->showMessage("Parat");
}

void MainWindow::processCam() {
    if (capture && !parar)
    {
        timer.restart();
        IplImage *frame;
        frame=cvQueryFrame(capture);
        if (frame->imageData) {
            glWidget->sendImage(frame);
            ui->statusBar->showMessage("Adquirint....");
            QTimer::singleShot(40, this, SLOT(processCam()));
        }
    }
    return;
}

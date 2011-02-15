//Implementació de la classe MainWindow

#include "mainwindow.h"
#include "ui_mainwindow.h"

//Constructor de la classe Mainwindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glWidget = new GLWidget();
    glWidget2 = new GLWidget();

    this->ui->horizontalLayout->addWidget(glWidget);
    this->ui->horizontalLayout->addWidget(glWidget2);

}

//Destructor de la classe Mainwindow
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

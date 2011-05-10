#include "widgetvideo.h"
#include "ui_widgetvideo.h"

Widgetvideo::Widgetvideo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widgetvideo)
{
    ui->setupUi(this);
}

Widgetvideo::~Widgetvideo()
{
    delete ui;
}

//Mètode que controla si s'ha clicat el widget, per fer el canvi de càmera
void Widgetvideo::mousePressEvent(QMouseEvent *event)
{
    QString nom = objectName();
    emit widgetClicked(nom[4].digitValue());
    QWidget::mousePressEvent(event);
}

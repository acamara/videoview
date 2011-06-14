#include "widgetvideo.h"
#include "ui_widgetvideo.h"
#include <QPainter>
#include <QPaintEvent>

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

void Widgetvideo::paintEvent(QPaintEvent * event) {
  QPainter p;
  p.begin(this);
  QRect area = event->rect();
  p.fillRect(area, Qt::black);
  p.end();
}

int Widgetvideo::heightForWidth(int w) const {
  return (w * 3) / 4; // 4:3
}

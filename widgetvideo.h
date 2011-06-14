#ifndef WIDGETVIDEO_H
#define WIDGETVIDEO_H

#include <QWidget>

namespace Ui {
    class Widgetvideo;
}

class Widgetvideo : public QWidget
{
    Q_OBJECT

public:
    explicit Widgetvideo(QWidget *parent = 0);
    ~Widgetvideo();

    int heightForWidth(int w) const;

signals:
    void widgetClicked(int);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent * event);

private:
    Ui::Widgetvideo *ui;
};

#endif // WIDGETVIDEO_H

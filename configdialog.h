//Declaració de la clase Dialog (Diàleg de configuració)

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    int get_config();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_Aceptar_clicked();
    void on_CancelarBoton_clicked();

private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H

//Implementaci� de la clase Dialog (Di�leg de configuraci�)

#include "configdialog.h"
#include "ui_configdialog.h"

#include <QMessageBox>

//Constructor de la clase Dialog
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Benvingut a Capturadora"));

}

//Destructor de la clase Dialog
Dialog::~Dialog()
{
    delete ui;
}


void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//M�tode que controla el bot� cancelar
void Dialog::on_CancelarBoton_clicked()
{
    close ();

}

//M�tode que controla el bot� aceptar
void Dialog::on_Aceptar_clicked()
{
    /*
    QMessageBox::information(this, "Informaci�n de depuraci�n",
    "Modo de edici�n: "+QString(ui->Modoedicion->itemText(ui->Modoedicion->currentIndex ()))
    +"\n\nFrames segundo: "+QString(ui->Framessegundo->itemText(ui->Framessegundo->currentIndex ()))
    +"\n\nRelaci�n de aspecto: "+QString(ui->Aspecto->itemText(ui->Aspecto->currentIndex ()))
    +"\n\nTama�o de cuadro: "+QString(ui->Tamanycuadre->itemText(ui->Tamanycuadre->currentIndex ()))
    +"\n\nN�mero de c�maras: "+QString(ui->NumCam->itemText(ui->NumCam->currentIndex ())));
    */

}

//M�tode que retorna l'informaci� obtiguda de la configuraci�
int Dialog::get_config()
{
    return(ui->NumCam->currentIndex ()+1);

}

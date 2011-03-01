//Implementació de la clase Dialog (Diàleg de configuració)

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

//Mètode que controla el botó cancelar
void Dialog::on_CancelarBoton_clicked()
{
    close ();

}

//Mètode que controla el botó aceptar
void Dialog::on_Aceptar_clicked()
{
    /*
    QMessageBox::information(this, "Información de depuración",
    "Modo de edición: "+QString(ui->Modoedicion->itemText(ui->Modoedicion->currentIndex ()))
    +"\n\nFrames segundo: "+QString(ui->Framessegundo->itemText(ui->Framessegundo->currentIndex ()))
    +"\n\nRelación de aspecto: "+QString(ui->Aspecto->itemText(ui->Aspecto->currentIndex ()))
    +"\n\nTamaño de cuadro: "+QString(ui->Tamanycuadre->itemText(ui->Tamanycuadre->currentIndex ()))
    +"\n\nNúmero de cámaras: "+QString(ui->NumCam->itemText(ui->NumCam->currentIndex ())));
    */

}

//Mètode que retorna l'informació obtiguda de la configuració
int Dialog::get_config()
{
    return(ui->NumCam->currentIndex ()+1);

}

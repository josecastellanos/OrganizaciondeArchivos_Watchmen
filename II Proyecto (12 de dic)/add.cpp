#include "add.h"
#include "ui_add.h"

add::add(int val, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::add)
{
    ui->setupUi(this);
    ultimoId = val;
}

add::~add()
{
    delete ui;
}


void add::on_Add_clicked()
{
    if( ui->No->text()=="" || ui->time->text()=="" ||
        ui->source->text()=="" || ui->destino->text()=="" ||
        ui->protocolo->text()==""|| ui->info->text()=="" )
    {
        QMessageBox::information(this,"Error","Alguno de los Datos esta mal!",0,0);
    }
    else
    {
        if(ui->No->text().toInt() <= ultimoId)
        {
            QMessageBox::warning(this,"Error","El id que ud Ingreso ya se encuentra en el Sistema",0,0);
            return;
        }
        emit addRecord(ui->No->text().toLong(),ui->time->text().toDouble(),
                       (char*)ui->source->text().toStdString().c_str(),(char*)ui->destino->text().toStdString().c_str(),
                       (char*)ui->protocolo->text().toStdString().c_str(),(char*)ui->info->text().toStdString().c_str());
        this->close();
    }    
}

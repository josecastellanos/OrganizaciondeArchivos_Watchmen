#include "add.h"
#include "ui_add.h"

add::add(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::add)
{
    ui->setupUi(this);
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
        QMessageBox::information(0,"Error","Alguno de los Datos esta mal!",0,0);
    }
    else
    {
        emit addRecord(ui->No->text().toLong(),ui->time->text().toDouble(),
                       (char*)ui->source->text().toStdString().c_str(),(char*)ui->destino->text().toStdString().c_str(),
                       (char*)ui->protocolo->text().toStdString().c_str(),(char*)ui->info->text().toStdString().c_str());
        this->close();
    }    
}

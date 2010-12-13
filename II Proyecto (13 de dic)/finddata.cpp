#include "finddata.h"
#include "ui_finddata.h"

findData::findData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::findData)
{
    ui->setupUi(this);
}

findData::~findData()
{
    delete ui;
}

void findData::on_find_clicked()
{    
    if(ui->rbDestino->isChecked())
    {
        //Destino

        if(ui->lineEdit->text().count() > 15)
        {
            QMessageBox::warning(this,"Error","Ha tecleado mal el Destino",0,0);
            return;
        }

        emit searchDestino((char*)ui->lineEdit->text().toStdString().c_str());
    }
    else if(ui->rbProtocolo->isChecked())
    {
        //Protocolo
        emit searchProtocolo((char*)ui->lineEdit->text().toStdString().c_str());
    }
    else if(ui->rdSource->isChecked())
    {
        //Source        

        if(ui->lineEdit->text().count() > 15)
        {
            QMessageBox::warning(this,"Error","Ha tecleado mal el Source",0,0);
            return;
        }

        emit searchSource((char*)ui->lineEdit->text().toStdString().c_str());
    }

    this->close();
}

void findData::on_rdSource_clicked()
{
    ui->lineEdit->setMaxLength(15);
}

void findData::on_rbDestino_clicked()
{
    ui->lineEdit->setMaxLength(15);
}

void findData::on_rbProtocolo_clicked()
{
    ui->lineEdit->setMaxLength(20);
}

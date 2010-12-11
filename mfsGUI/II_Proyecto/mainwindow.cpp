#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPlastiqueStyle *sp = new QPlastiqueStyle();
    ui->tbFind->setStyle(sp);
    ui->tbLog->setStyle(sp);
    ui->txtLog->setStyle(sp);

    this->open_disk();
}

void MainWindow::open_disk()
{
    archivo = QFileDialog::getOpenFileName(0,"Selecciona el Archivo",
                                                   "","Archivos de Texto(*.txt)");
    disco = QFileDialog::getSaveFileName(0,"Ubicacion del disco",
                                           "","Archivo Binario(*.dat)");    
    this->setWindowTitle(archivo);

    QFile file(archivo);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

    QFile _disco(disco);
    if(_disco.exists())
    {
        return;
    }

    registros=new dataFile((char *)disco.toUtf8().constData(),ui->txtLog);

    QTextStream in(&file);
    QList <QString> list;
    int c=0;

    while (!in.atEnd())
    {
        in.readLine();
        c++;
    }

    registros->create(c+c*(0.1));
    in.seek(0);
    c=0;

    in.readLine();

    ui->tbLog->clearContents();

    for(int i=0; i<ui->tbLog->rowCount();i++)
        ui->tbLog->removeRow(i);

    QString temp;

    while (!in.atEnd())
    {
        QString line = in.readLine();

        ui->tbLog->insertRow(c);

        int d=0;
        list.clear();

        for(int i=0; i<6; i++)
        {
            while(line.at(d)==' ')
            {
                d++;
            }

            int e=d;

            while(line.at(d)!=' ')
            {
                d++;
            }

            temp.clear();

            if(i!=5)
            {
                temp=line.mid(e,d-e);
            }
            else
            {
                temp=line.mid(e,250);
            }

            list.append(temp);

            ui->tbLog->setItem(c,i,new QTableWidgetItem(temp));
        }

        registros->add(list.at(0).toLong(),
                       list.at(1).toDouble(),
                       (char *)list.at(2).toUtf8().constData(),
                       (char *)list.at(3).toUtf8().constData(),
                       (char *)list.at(4).toUtf8().constData(),
                       (char *)list.at(5).toUtf8().constData());

        c++;
    }

    int hee=1000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    this->ventanaAdd = new add(this);
    this->ventanaAdd->show();
}

void MainWindow::on_btnDelete_clicked()
{
    this->ventanaDelete = new deleteId(this);
    this->ventanaDelete->show();
}

void MainWindow::on_btnFind_clicked()
{
    this->ventanaFind = new findData(this);
    this->ventanaFind->show();
}

void MainWindow::on_btnUpdate_clicked()
{
    this->ventanaUpdate = new updateId(this);
    this->ventanaUpdate->show();
}

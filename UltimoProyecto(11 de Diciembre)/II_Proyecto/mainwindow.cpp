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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    this->ventanaAdd = new add(this);
    this->ventanaAdd->show();

    connect(ventanaAdd,SIGNAL(addRecord(long,double,char*,char*,char*,char*)),this,SLOT(addRecord(long,double,char*,char*,char*,char*)));
}

void MainWindow::on_btnFind_clicked()
{
    this->ventanaFind = new findData(this);
    this->ventanaFind->show();

    connect(ventanaFind,SIGNAL(searchDestino(char*)),this,SLOT(searchDestino(char*)));

    connect(ventanaFind,SIGNAL(searchProtocolo(char*)),this,SLOT(searchProtocolo(char*)));

    connect(ventanaFind,SIGNAL(searchSource(char*)),this,SLOT(searchSource(char*)));
}

void MainWindow::addRecord(long id, double time, char *source, char *destino, char *protocolo, char *info)
{
    registros->add(id,time,source,destino,protocolo,info);
}

void MainWindow::searchSource(char *source)
{
    registros->searchHashSource(source,ui->tbFind);
}

void MainWindow::searchDestino(char *destino)
{
    registros->searchHashDestino(destino,ui->tbFind);
}

void MainWindow::searchProtocolo(char *protocolo)
{
    registros->searchAVL(protocolo,ui->tbFind);
}

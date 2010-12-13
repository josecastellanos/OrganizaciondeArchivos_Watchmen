#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    agrego=false;

    QPlastiqueStyle *sp = new QPlastiqueStyle();
    ui->tbFind->setStyle(sp);
    ui->tbLog->setStyle(sp);
    ui->txtLog->setStyle(sp);

    this->timeAddLlavesPrimarias=0;
    this->timeAddLlavesSecundarias=0;
    this->timeSearchAvl=0;
    this->timeSearchHashDestino=0;
    this->timeSearchHashSource=0;
    this->timeTotalPrimarias=0;
    this->timeTotalSecundarias=0;

    this->open_disk();
    this->timing(agrego);
}

void MainWindow::timing(bool agrega)
{
    if(agrega)
    {
        ui->lcdPrimario->display(QString::number(registros->timeLlavesPrimarias()));
        ui->lcdSecundaria->display(QString::number(registros->timeLlavesSecundarias()));
        this->timeTotalPrimarias = registros->timeLlavesPrimarias();
        this->timeTotalSecundarias = registros->timeLlavesSecundarias();
    }
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

    registros->create(c*(1.1));
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
    agrego = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    int max = ui->tbLog->rowCount();
    this->ventanaAdd = new add(max,this);
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
    registros->resetTime();

    registros->add(id,time,source,destino,protocolo,info);

    this->timeAddLlavesPrimarias = registros->timeLlavesPrimarias();
    this->timeAddLlavesSecundarias = registros->timeLlavesSecundarias();

    QString t = "Tiempo para Agregarse a Llave Primaria: " + QString::number(this->timeAddLlavesPrimarias)+" ms";
    QString t2 = "Tiempo para Agregarse a Llaves Secundarias: "+QString::number(this->timeAddLlavesSecundarias)+" ms";
    QString t3 = t + "\n" + t2;
    QMessageBox::information(this,"mensaje",t3,0,0);

    int row = ui->tbLog->rowCount();

    ui->tbLog->insertRow(row);
    int col = 0;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString::number(id)));
    col++;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString::number(time)));
    col++;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString(source)));
    col++;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString(destino)));
    col++;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString(protocolo)));
    col++;

    ui->tbLog->setItem(row,col,new QTableWidgetItem(QString(info)));
}

void MainWindow::searchSource(char *source)
{
    registros->resetTime();
    registros->searchHashSource(source,ui->tbFind);
    this->timeSearchHashSource = registros->timeSearchHS();
    QString t = "Tiempo de Busqueda para el Source: "+QString::number(this->timeSearchHashSource)+" ms";
    QMessageBox::information(this,"Mensaje",t,0,0);
}

void MainWindow::searchDestino(char *destino)
{
    registros->resetTime();
    registros->searchHashDestino(destino,ui->tbFind);
    this->timeSearchHashDestino = registros->timeSearchHD();
    QString t = "Tiempo de Busqueda para el Destino: "+QString::number(this->timeSearchHashDestino)+" ms";
    QMessageBox::information(this,"Mensaje",t,0,0);

}

void MainWindow::searchProtocolo(char *protocolo)
{
    registros->resetTime();
    registros->searchAVL(protocolo,ui->tbFind);
    this->timeSearchAvl = registros->timeSearchAV();
    QString t = "Tiempo de Busqueda para el Protocolo: "+QString::number(this->timeSearchHashSource)+" ms";
    QMessageBox::information(this,"Mensaje",t,0,0);
}

void MainWindow::on_actionAbout_II_Proyecto_triggered()
{
    about = new acercaDe(this);
    about->show();
}

void MainWindow::on_actionCreate_Disk_triggered()
{
    agrego=false;

    this->timeAddLlavesPrimarias=0;
    this->timeAddLlavesSecundarias=0;
    this->timeSearchAvl=0;
    this->timeSearchHashDestino=0;
    this->timeSearchHashSource=0;
    this->timeTotalPrimarias=0;
    this->timeTotalSecundarias=0;

    this->open_disk();
    this->timing(agrego);
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    // Preguntar si queremos salir o cerrar la ventana principal
    if(QMessageBox::question(0, "Desea Salir",
            "Desea guardar un archivo con los tiempos \nque tomo durante esta sesion?",
            "&Si", "&No",
            QString::null, 0, 1 )
      )
    {
        event->accept();
    }
    else
    {
        disco = QFileDialog::getSaveFileName(0,"Ubicacion del disco",
                                               "","Archivo Binario(*.txt)");

        tiempo.setFileName(disco);

        if (!tiempo.open(QIODevice::WriteOnly | QIODevice::Text))
                 return;

        QTextStream in(&tiempo);

        in << "Tiempo Total Llaves Primarias:\t" << QString::number(this->timeTotalPrimarias) << " ms\n";
        in << "Tiempo Total Llaves Secundarias:\t" << QString::number(this->timeTotalSecundarias) << " ms\n";
        in << "Tiempo en Agregarse el Ultimo Registro a Llave Primaria:\t" << QString::number(this->timeAddLlavesPrimarias) << " ms\n";
        in << "Tiempo en Agregarse el Ultimo Registro a Llaves Secundarias:\t" << QString::number(this->timeAddLlavesSecundarias) << " ms\n";
        in << "Tiempo en Buscar el Ultimo Protocolo:\t" << QString::number(this->timeSearchAvl) << " ms\n";
        in << "Tiempo en Buscar el Ultimo Destino:\t" << QString::number(this->timeSearchHashDestino) << " ms\n";
        in << "Tiempo en Buscar el Ultimo Source:\t" << QString::number(this->timeSearchHashSource) << " ms\n";

        tiempo.close();

        event->accept();
    }
 }

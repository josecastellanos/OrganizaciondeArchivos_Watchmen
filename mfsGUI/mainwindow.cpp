#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Sets para hacer nuestro QListWidget se vea Macizo
    ui->lwSongs->setViewMode(QListWidget::IconMode);
    ui->lwSongs->setMovement(QListWidget::Free);
    ui->lwSongs->setFlow(QListWidget::LeftToRight);
    ui->lwSongs->setIconSize(QSize(32,32));
    ui->lwSongs->setSpacing(12);

    //Para que se mire macizo los Widgets
    QPlastiqueStyle *sp = new QPlastiqueStyle();
    ui->lwSongs->setStyle(sp);
    ui->lwTags->setStyle(sp);
    ui->InfoCancion->setStyle(sp);
    this->setStyle(sp);


    //Aqui Miramos si existe algun archivo con la info necesaria!
    QString pathTemp = QDir::homePath() + "/" + ".Settings";

    QFile *file = new QFile(pathTemp);
    if(file->exists())
    {
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
                 return;

        QString texto;
        QTextStream salida(file);
        texto = salida.readAll();
// \n\r
        QStringList lista = texto.split("#");
        this->ExePath = lista.at(0);
        this->DiskPath = lista.at(1);
        this->Mp3Folder = lista.at(2);

        this->on_actionShow_Tags_triggered();

        file->close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parsearOutput(QString output)
{
    if(output.startsWith("LISTARTAGS:"))
    {
        //Show Tags (-l)
        output = output.mid(11);
        QStringList listaTags = output.split(",");
        ui->lwTags->clear();
        for(int i=0; i<(listaTags.count()-1); i++)
        {
            ui->lwTags->addItem(listaTags.at(i));
        }
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("CREAR:"))
    {
        output=output.mid(6);
        QMessageBox::information(0,"Disk Created",output,0,0);
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("SEARCHTAG:"))
    {
        output = output.mid(10);
        this->listaInodos = output.split(",");
        output.clear();
        listaParametros.clear();

        for(int i=0; i<listaInodos.count()-1; i++)
        {
            queryInodo(i);
        }
    }
    else if(output.startsWith("QUERYINODO:"))
    {
        output = output.mid(11);
        QStringList InfoCancion = output.split("#");
        QListWidgetItem *item = new QListWidgetItem(0);
        QString nombre = InfoCancion.at(0);
        nombre = nombre.mid(20);
        item->setText(nombre);
        item->setIcon(QIcon(":/images/icono.png"));
        ui->lwSongs->addItem(item);
        output.clear();
        listaParametros.clear();
        //Aqui Van todas las demas cosas que no se donde ponerlas!
    }
    else if(output.startsWith("QUERYFILE:$"))
    {
        // Este Parse Me funciona para el ItemClicked!
        output = output.mid(22);
        QStringList InfoCancion = output.split("#");
        QString info;
        for(int i=0; i<InfoCancion.count();i++)
            info.append(InfoCancion.at(i)+"\n");
        ui->InfoCancion->setText(info);
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("ADD:"))
    {
        output = output.mid(4);
        QMessageBox::information(0,"Song Added",output,0,0);
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("EXPORTAR:"))
    {
        output = output.mid(9);
        QMessageBox::information(0,"Song Exported",output,0,0);
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("DELETE:"))
    {
        output = output.mid(7);
        QMessageBox::information(0,"Song Deleted",output,0,0);
        output.clear();
        listaParametros.clear();
    }
    else if(output.startsWith("TAG:"))
    {
        output = output.mid(4);
        QMessageBox::information(this,"Song Tagged",output,0,0);
        output.clear();
        listaParametros.clear();
        this->on_actionShow_Tags_triggered();
    }
}

void MainWindow::on_actionExit_triggered()
{
   this->close();
}

void MainWindow::on_actionNew_Disk_triggered()
{
    this->newDiskDialog = new Dialog(this);
    newDiskDialog->show();
    connect(newDiskDialog,SIGNAL(createDisk(QString*,QStringList*)),this,SLOT(createDisk(QString*,QStringList*)));
}

void MainWindow::on_actionSet_exe_triggered()
{
    ExePath = QFileDialog::getOpenFileName(this, tr("Set .exe"), "/", tr("*.exe"));
}

void MainWindow::createDisk(QString *ExePath, QStringList *listaParametros)
{
    this->ExePath=*ExePath;
    DiskPath = newDiskDialog->diskPath + "/" + newDiskDialog->diskName;
    Mp3Folder = newDiskDialog->Mp3Folder;

    QString pathTemp = QDir::homePath() + "/" + ".Settings";
    QFile *file= new QFile(pathTemp);

    QDir dir(Mp3Folder);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QTextStream in(file);
    in << this->ExePath << "#" << DiskPath << "#" << Mp3Folder;

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    file->close();

    this->listaParametros=*listaParametros;
    ejecutar();

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName().contains(".mp3"))
        {
            this->listaParametros.clear();
            this->listaParametros.append("-a");
            this->listaParametros.append(fileInfo.absoluteFilePath());
            this->listaParametros.append(DiskPath);
            ejecutar();
        }
    }
    this->on_actionShow_Tags_triggered();
}

bool MainWindow::ejecutar()
{
    ps.start(ExePath,listaParametros);
    if (!ps.waitForStarted())
        return false;

    if (!ps.waitForFinished())
        return false;

    QString result = ps.readAll()+"\n";
    parsearOutput(result);

    return true;
}

void MainWindow::on_actionAdd_Song_triggered()
{
    QStringList archivosMp3 = QFileDialog::getOpenFileNames(this,"Selecciona Archivos Para Agregar al Disco",
                                                            "","Mp3 Files(*.mp3)");

    for(int i=0; i<archivosMp3.count();i++)
    {
        listaParametros.clear();
        listaParametros.append("-a");
        listaParametros.append(archivosMp3.at(i));
        listaParametros.append(this->DiskPath);
        ejecutar();
    }
    this->on_actionShow_Tags_triggered();
}

void MainWindow::on_actionExport_Song_triggered()
{
    //ExePath="/home/joc/Escritorio/mfs.exe";
    //"/home/joc/Escritorio/disco.dat"
    bool ok;
    QString nombreCancion = QInputDialog::getText(this, tr("Mp3 Name"),
                                              tr("Mp3 Name to Export: "), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);

    QString destino = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "",tr("Mp3 Files (*.mp3)"));

    listaParametros.clear();
    listaParametros.append("-e");
    listaParametros.append(nombreCancion);
    listaParametros.append(destino);
    listaParametros.append(this->DiskPath);
    ejecutar();
}

void MainWindow::on_actionDelete_Song_triggered()
{
    bool ok;
    QString nombreCancion = QInputDialog::getText(this, tr("Mp3 Name"),
                                              tr("Mp3 Name to Export: "), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);

    listaParametros.clear();
    listaParametros.append("-d");
    listaParametros.append(nombreCancion);
    listaParametros.append(this->DiskPath);
    ejecutar();
    this->on_actionShow_Tags_triggered();
}

void MainWindow::on_actionTag_Song_triggered()
{
    bool ok;
    QString nombreCancion = QInputDialog::getText(this, tr("Mp3 Name"),
                                                  tr("Mp3 Name to Export: "), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
    QString tag = QInputDialog::getText(this, tr("Tag"),
                                              tr("Tag: "), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);


    listaParametros.clear();
    listaParametros.append("-t");
    listaParametros.append(tag);
    listaParametros.append(nombreCancion);
    listaParametros.append(this->DiskPath);
    ejecutar();
    this->on_actionShow_Tags_triggered();
}

void MainWindow::on_actionShow_Tags_triggered()
{
    listaParametros.clear();
    listaParametros.append("-l");
    listaParametros.append(this->DiskPath);
    ejecutar();
}

void MainWindow::on_lwTags_itemClicked(QListWidgetItem* item)
{
    //ListWidget de Tags
    ui->lwSongs->clear();
    listaParametros.clear();
    listaParametros.append("-s");
    listaParametros.append(item->text());
    listaParametros.append(this->DiskPath);
    ejecutar();
}

void MainWindow::on_lwSongs_itemClicked(QListWidgetItem* item)
{
    //ListWidget de Songs
    listaParametros.clear();
    listaParametros.append("-qa");
    listaParametros.append(item->text());
    listaParametros.append(this->DiskPath);
    ejecutar();
}

void MainWindow::on_actionOpen_Disk_triggered()
{
    QString nuevoDisco = QFileDialog::getOpenFileName(this,"Selecciona El Nuevo Disco!",
                                                            "","Archivos de Disco(*.dat)");
    DiskPath = nuevoDisco;
    QMessageBox::information(this,"Disk Open","The Disk has opened succesfully",QMessageBox::Accepted,0);
    if(ExePath=="")
    {
        QMessageBox::warning(this,"Warning","Our System detected you \nhavent set a .exe File ",QMessageBox::Accepted,0);
        this->on_actionSet_exe_triggered();
    }
    this->on_actionShow_Tags_triggered();
}

void MainWindow::on_lwSongs_itemDoubleClicked(QListWidgetItem* item)
{
    bool ok;
    QString tag = QInputDialog::getText(this, tr("Tag"),
                                             tr("Tag: "), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
    listaParametros.clear();
    listaParametros.append("-t");
    listaParametros.append(tag);
    listaParametros.append(item->text());
    listaParametros.append(DiskPath);
    ejecutar();
}

void MainWindow::queryInodo(int indice)
{
    listaParametros.clear();
    listaParametros.append("-qi");
    listaParametros.append(listaInodos.at(indice));
    listaParametros.append(DiskPath);
    ejecutar();
}

void MainWindow::on_actionAbout_mfs_triggered()
{
    this->acerca = new about(this);
    acerca->show();
}

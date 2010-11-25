#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnDiskPath_clicked()
{
    diskPath = QFileDialog::getExistingDirectory(this,
                               tr("Disk Path"), QDir::currentPath());

    ui->lnDiskPath->setText(diskPath);
}

void Dialog::on_btnMp3Folder_clicked()
{
    Mp3Folder = QFileDialog::getExistingDirectory(this,
                                                  tr("Mp3 Folder"), QDir::currentPath());

    ui->lnMp3Folder->setText(Mp3Folder);
}

void Dialog::on_btnSetExe_clicked()
{
    ExePath = QFileDialog::getOpenFileName(this, tr("Set .exe"), "/", tr("*.exe"));
    ui->lnSetExe->setText(ExePath);
}

void Dialog::on_btnCreate_clicked()
{
    if(ui->lnDiskName->text()!="" && ui->lnDiskPath->text()!="" && ui->lnDiskSize->text()!="" &&
       ui->lnFileEntries->text()!="" && ui->lnMp3Folder->text()!="" && ui->lnTagEntries->text()!="" && ui->lnSetExe->text()!="")
    {
        diskName = ui->lnDiskName->text();
        DiskSize = ui->lnDiskSize->text();
        TagEntries = ui->lnTagEntries->text();
        FileEntries = ui->lnFileEntries->text();
        ExePath = ui->lnSetExe->text();
    }
    else
    {
        return;
    }

    QStringList listaParametros;
    listaParametros.append("-c");
    listaParametros.append(DiskSize);
    listaParametros.append(FileEntries);
    listaParametros.append(TagEntries);
    listaParametros.append(diskPath+"/"+diskName);
    emit createDisk(&ExePath,&listaParametros);

    this->close();
}

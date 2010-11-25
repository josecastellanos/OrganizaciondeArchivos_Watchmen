#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QString>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString diskPath,Mp3Folder,diskName,FileEntries,TagEntries,DiskSize,ExePath;

private:
    Ui::Dialog *ui;

private slots:
    void on_btnCreate_clicked();
    void on_btnSetExe_clicked();
    void on_btnMp3Folder_clicked();
    void on_btnDiskPath_clicked();

signals:
    void createDisk(QString *ExePath,QStringList *listaParametros);
};


#endif // DIALOG_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "about.h"
#include <QMainWindow>
#include <QProcess>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QPlastiqueStyle>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Dialog *newDiskDialog;
    about *acerca;
    QString DiskPath,Mp3Folder,ExePath;
    QStringList listaInodos,listaParametros;
    QDir dir;
    QProcess ps;
    QString output;
    QFile *archivo;
    int indiceDeLista;
    void AddTagsToList();
    bool mostrarInfo(QString inodo);
    void queryInodo(int indice);
    void parsearOutput(QString output);
    bool mostrarCancion(int i);
    bool ejecutar();

private slots:
    void on_actionAbout_mfs_triggered();
    void on_actionOpen_Disk_triggered();
    void on_lwSongs_itemClicked(QListWidgetItem* item);
    void on_lwSongs_itemDoubleClicked(QListWidgetItem* item);
    void on_lwTags_itemClicked(QListWidgetItem* item);
    void on_actionShow_Tags_triggered();
    void on_actionTag_Song_triggered();
    void on_actionDelete_Song_triggered();
    void on_actionExport_Song_triggered();
    void on_actionAdd_Song_triggered();
    void on_actionSet_exe_triggered();
    void on_actionExit_triggered();
    void on_actionNew_Disk_triggered();

protected slots:
    void createDisk(QString *ExePath,QStringList *listaParametros);

};

#endif // MAINWINDOW_H

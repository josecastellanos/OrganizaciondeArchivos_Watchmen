#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "add.h"
#include "deleteid.h"
#include "finddata.h"
#include "updateid.h"
#include "datafile.h"
#include <QPlastiqueStyle>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

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
    //Funciones
    void open_disk();

    //Ventanas
    add* ventanaAdd;
    updateId* ventanaUpdate;
    findData* ventanaFind;
    deleteId* ventanaDelete;

    //Resto de atributos
    dataFile *registros;    
    QString archivo, disco;

private slots:
    void on_btnFind_clicked();
    void on_btnAdd_clicked();

protected slots:
    void addRecord(long id, double time, char *source, char *destino, char *protocolo, char *info);
    void searchSource(char *source);
    void searchDestino(char *destino);
    void searchProtocolo(char *protocolo);

};

#endif // MAINWINDOW_H

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
#include <QCloseEvent>
#include <QTextStream>
#include "acercade.h"

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
    void timing(bool agrego);

    //Ventanas
    add* ventanaAdd;
    findData* ventanaFind;
    acercaDe* about;
    //deleteId* ventanaDelete;
    //updateId* ventanaUpdate;

    //Resto de atributos
    dataFile *registros;    
    QString archivo, disco;

    //Para guardar Los tiempos!
    QFile tiempo;
    QString aSalvar;

    //Variables
    bool agrego;

    //Variables Para El Tiempo
    double timeTotalPrimarias;
    double timeTotalSecundarias;
    double timeAddLlavesPrimarias;
    double timeAddLlavesSecundarias;
    double timeSearchHashDestino;
    double timeSearchHashSource;
    double timeSearchAvl;



private slots:
    void on_actionCreate_Disk_triggered();
    void on_actionAbout_II_Proyecto_triggered();
    void on_btnFind_clicked();
    void on_btnAdd_clicked();

protected slots:
    void closeEvent(QCloseEvent *event);
    void addRecord(long id, double time, char *source, char *destino, char *protocolo, char *info);
    void searchSource(char *source);
    void searchDestino(char *destino);
    void searchProtocolo(char *protocolo);

};

#endif // MAINWINDOW_H

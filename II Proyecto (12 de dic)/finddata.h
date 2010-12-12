#ifndef FINDDATA_H
#define FINDDATA_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
    class findData;
}

class findData : public QMainWindow
{
    Q_OBJECT

public:
    explicit findData(QWidget *parent = 0);
    ~findData();

private:
    Ui::findData *ui;
    bool dest, sour, prot;

private slots:
    void on_rbProtocolo_clicked();
    void on_rbDestino_clicked();
    void on_rdSource_clicked();
    void on_find_clicked();

signals:
    void searchSource(char *source);
    void searchDestino(char *destino);
    void searchProtocolo(char *protocolo);
};

#endif // FINDDATA_H

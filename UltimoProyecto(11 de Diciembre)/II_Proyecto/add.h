#ifndef ADD_H
#define ADD_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
    class add;
}

class add : public QMainWindow
{
    Q_OBJECT

public:
    explicit add(QWidget *parent = 0);
    ~add();

private:
    Ui::add *ui;


private slots:
    void on_Add_clicked();

signals:
    void addRecord(long id, double time, char *source, char *destino, char *protocolo, char *info);

};

#endif // ADD_H

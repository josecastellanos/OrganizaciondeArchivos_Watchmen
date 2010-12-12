#ifndef ACERCADE_H
#define ACERCADE_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
    class acercaDe;
}

class acercaDe : public QDialog
{
    Q_OBJECT

public:
    explicit acercaDe(QWidget *parent = 0);
    ~acercaDe();

private:
    Ui::acercaDe *ui;

private slots:
    void on_richard_clicked();
    void on_dago_clicked();
};

#endif // ACERCADE_H

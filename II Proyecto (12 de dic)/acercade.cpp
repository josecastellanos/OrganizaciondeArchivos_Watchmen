#include "acercade.h"
#include "ui_acercade.h"

acercaDe::acercaDe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::acercaDe)
{
    ui->setupUi(this);
}

acercaDe::~acercaDe()
{
    delete ui;
}

void acercaDe::on_dago_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.facebook.com/iJXDX"));
}

void acercaDe::on_richard_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.youtube.com/limpion"));
}

#include "updateid.h"
#include "ui_updateid.h"

updateId::updateId(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::updateId)
{
    ui->setupUi(this);
}

updateId::~updateId()
{
    delete ui;
}

#include "deleteid.h"
#include "ui_deleteid.h"

deleteId::deleteId(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deleteId)
{
    ui->setupUi(this);
}

deleteId::~deleteId()
{
    delete ui;
}

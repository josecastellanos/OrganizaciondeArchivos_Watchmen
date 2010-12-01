#include "finddata.h"
#include "ui_finddata.h"

findData::findData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::findData)
{
    ui->setupUi(this);
}

findData::~findData()
{
    delete ui;
}

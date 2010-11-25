#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
}

about::~about()
{
    delete ui;
}

void about::on_commandLinkButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.youtube.com/limpion"));
}

void about::on_commandLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.facebook.com/iJXDX"));
}

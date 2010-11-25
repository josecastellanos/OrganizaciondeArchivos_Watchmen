#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
    class about;
}

class about : public QDialog
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = 0);
    ~about();

private:
    Ui::about *ui;

private slots:
    void on_commandLinkButton_clicked();
    void on_commandLinkButton_2_clicked();
};

#endif // ABOUT_H

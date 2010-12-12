#ifndef UPDATEID_H
#define UPDATEID_H

#include <QMainWindow>

namespace Ui {
    class updateId;
}

class updateId : public QMainWindow
{
    Q_OBJECT

public:
    explicit updateId(QWidget *parent = 0);
    ~updateId();

private:
    Ui::updateId *ui;
};

#endif // UPDATEID_H

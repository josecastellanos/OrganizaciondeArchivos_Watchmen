#ifndef DELETEID_H
#define DELETEID_H

#include <QMainWindow>

namespace Ui {
    class deleteId;
}

class deleteId : public QMainWindow
{
    Q_OBJECT

public:
    explicit deleteId(QWidget *parent = 0);
    ~deleteId();

private:
    Ui::deleteId *ui;
};

#endif // DELETEID_H

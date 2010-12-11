#ifndef ADD_H
#define ADD_H

#include <QMainWindow>

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
};

#endif // ADD_H

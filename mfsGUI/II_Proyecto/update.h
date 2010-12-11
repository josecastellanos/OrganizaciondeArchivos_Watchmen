#ifndef UPDATE_H
#define UPDATE_H

#include <QMainWindow>

namespace Ui {
    class update;
}

class update : public QMainWindow
{
    Q_OBJECT

public:
    explicit update(QWidget *parent = 0);
    ~update();

private:
    Ui::update *ui;
};

#endif // UPDATE_H

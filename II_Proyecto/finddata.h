#ifndef FINDDATA_H
#define FINDDATA_H

#include <QMainWindow>

namespace Ui {
    class findData;
}

class findData : public QMainWindow
{
    Q_OBJECT

public:
    explicit findData(QWidget *parent = 0);
    ~findData();

private:
    Ui::findData *ui;
};

#endif // FINDDATA_H

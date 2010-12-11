/********************************************************************************
** Form generated from reading UI file 'finddata.ui'
**
** Created: Fri Dec 10 11:32:10 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDATA_H
#define UI_FINDDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_findData
{
public:
    QWidget *centralwidget;
    QPushButton *find;
    QRadioButton *rdSource;
    QRadioButton *rbProtocolo;
    QLineEdit *lineEdit;
    QRadioButton *rbDestino;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *findData)
    {
        if (findData->objectName().isEmpty())
            findData->setObjectName(QString::fromUtf8("findData"));
        findData->resize(500, 302);
        centralwidget = new QWidget(findData);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        find = new QPushButton(centralwidget);
        find->setObjectName(QString::fromUtf8("find"));
        find->setGeometry(QRect(10, 130, 481, 111));
        find->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"image: url(:/images/find.png); \n"
"background-color: transparent;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: transparent;\n"
"font: bold 22px;\n"
"min-width:4m;\n"
"padding: 6px;\n"
"}"));
        rdSource = new QRadioButton(centralwidget);
        rdSource->setObjectName(QString::fromUtf8("rdSource"));
        rdSource->setGeometry(QRect(110, 40, 82, 21));
        rbProtocolo = new QRadioButton(centralwidget);
        rbProtocolo->setObjectName(QString::fromUtf8("rbProtocolo"));
        rbProtocolo->setGeometry(QRect(110, 100, 82, 21));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(190, 70, 200, 21));
        lineEdit->setMaxLength(20);
        rbDestino = new QRadioButton(centralwidget);
        rbDestino->setObjectName(QString::fromUtf8("rbDestino"));
        rbDestino->setGeometry(QRect(110, 70, 82, 21));
        findData->setCentralWidget(centralwidget);
        menubar = new QMenuBar(findData);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 500, 23));
        findData->setMenuBar(menubar);
        statusbar = new QStatusBar(findData);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        findData->setStatusBar(statusbar);

        retranslateUi(findData);

        QMetaObject::connectSlotsByName(findData);
    } // setupUi

    void retranslateUi(QMainWindow *findData)
    {
        findData->setWindowTitle(QApplication::translate("findData", "MainWindow", 0, QApplication::UnicodeUTF8));
        find->setText(QString());
        rdSource->setText(QApplication::translate("findData", "Source", 0, QApplication::UnicodeUTF8));
        rbProtocolo->setText(QApplication::translate("findData", "Protocolo", 0, QApplication::UnicodeUTF8));
        rbDestino->setText(QApplication::translate("findData", "Destino", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class findData: public Ui_findData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDATA_H

/********************************************************************************
** Form generated from reading UI file 'finddata.ui'
**
** Created: Mon Nov 29 17:21:24 2010
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
    QPushButton *pushButton_8;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLineEdit *lineEdit_6;
    QRadioButton *radioButton_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *findData)
    {
        if (findData->objectName().isEmpty())
            findData->setObjectName(QString::fromUtf8("findData"));
        findData->resize(500, 302);
        centralwidget = new QWidget(findData);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(10, 140, 481, 111));
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton\n"
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
        radioButton = new QRadioButton(centralwidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(110, 40, 82, 21));
        radioButton_2 = new QRadioButton(centralwidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(110, 100, 82, 21));
        lineEdit_6 = new QLineEdit(centralwidget);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(190, 70, 200, 21));
        radioButton_3 = new QRadioButton(centralwidget);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(110, 70, 82, 21));
        findData->setCentralWidget(centralwidget);
        menubar = new QMenuBar(findData);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 500, 21));
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
        pushButton_8->setText(QString());
        radioButton->setText(QApplication::translate("findData", "Source", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("findData", "Protocolo", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("findData", "Destino", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class findData: public Ui_findData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDATA_H

/********************************************************************************
** Form generated from reading UI file 'updateid.ui'
**
** Created: Sun Dec 12 10:10:57 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEID_H
#define UI_UPDATEID_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_updateId
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton_8;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *updateId)
    {
        if (updateId->objectName().isEmpty())
            updateId->setObjectName(QString::fromUtf8("updateId"));
        updateId->resize(500, 242);
        centralwidget = new QWidget(updateId);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 40, 61, 21));
        lineEdit_6 = new QLineEdit(centralwidget);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(190, 40, 200, 21));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(10, 80, 481, 111));
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"image: url(:/images/update.png); \n"
"background-color: transparent;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: transparent;\n"
"font: bold 22px;\n"
"min-width:4m;\n"
"padding: 6px;\n"
"}"));
        updateId->setCentralWidget(centralwidget);
        menubar = new QMenuBar(updateId);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 500, 21));
        updateId->setMenuBar(menubar);
        statusbar = new QStatusBar(updateId);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        updateId->setStatusBar(statusbar);

        retranslateUi(updateId);

        QMetaObject::connectSlotsByName(updateId);
    } // setupUi

    void retranslateUi(QMainWindow *updateId)
    {
        updateId->setWindowTitle(QApplication::translate("updateId", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("updateId", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">No.:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class updateId: public Ui_updateId {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEID_H

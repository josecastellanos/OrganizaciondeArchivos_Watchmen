/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Thu Nov 25 14:03:08 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_about
{
public:
    QLabel *label;
    QLabel *label_2;
    QCommandLinkButton *commandLinkButton;
    QCommandLinkButton *commandLinkButton_2;

    void setupUi(QDialog *about)
    {
        if (about->objectName().isEmpty())
            about->setObjectName(QString::fromUtf8("about"));
        about->resize(300, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        about->setWindowIcon(icon);
        about->setStyleSheet(QString::fromUtf8("QDialog{\n"
"	background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:1.061, fx:0.5, fy:0.92, stop:0 rgba(188, 188, 188, 255), stop:1 rgba(255, 255, 255, 255));\n"
"}"));
        label = new QLabel(about);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 281, 81));
        QFont font;
        font.setPointSize(28);
        label->setFont(font);
        label_2 = new QLabel(about);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(180, 190, 101, 91));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_2->setScaledContents(true);
        commandLinkButton = new QCommandLinkButton(about);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(10, 150, 191, 41));
        commandLinkButton_2 = new QCommandLinkButton(about);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setGeometry(QRect(10, 200, 172, 41));
        QWidget::setTabOrder(commandLinkButton_2, commandLinkButton);

        retranslateUi(about);

        QMetaObject::connectSlotsByName(about);
    } // setupUi

    void retranslateUi(QDialog *about)
    {
        about->setWindowTitle(QApplication::translate("about", "About mfs", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("about", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:28pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:20pt; font-weight:600; color:#000000;\">Music File System</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        commandLinkButton->setText(QApplication::translate("about", "Dagoberto Castellanos", 0, QApplication::UnicodeUTF8));
        commandLinkButton_2->setText(QApplication::translate("about", "Richard Siwady", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class about: public Ui_about {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H

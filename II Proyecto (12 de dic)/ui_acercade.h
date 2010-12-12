/********************************************************************************
** Form generated from reading UI file 'acercade.ui'
**
** Created: Sun Dec 12 16:39:26 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACERCADE_H
#define UI_ACERCADE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_acercaDe
{
public:
    QCommandLinkButton *dago;
    QCommandLinkButton *richard;
    QLabel *label;

    void setupUi(QDialog *acercaDe)
    {
        if (acercaDe->objectName().isEmpty())
            acercaDe->setObjectName(QString::fromUtf8("acercaDe"));
        acercaDe->resize(300, 300);
        acercaDe->setStyleSheet(QString::fromUtf8("QDialog\n"
"{\n"
"	background-image: url(:/images/add.jpg);\n"
"}"));
        dago = new QCommandLinkButton(acercaDe);
        dago->setObjectName(QString::fromUtf8("dago"));
        dago->setGeometry(QRect(50, 150, 201, 41));
        richard = new QCommandLinkButton(acercaDe);
        richard->setObjectName(QString::fromUtf8("richard"));
        richard->setGeometry(QRect(50, 200, 201, 41));
        label = new QLabel(acercaDe);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 281, 131));
        label->setAcceptDrops(false);
        label->setAutoFillBackground(false);
        label->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"image: url(:/images/logo.png); \n"
"background-color: transparent;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: transparent;\n"
"font: bold 22px;\n"
"min-width:4m;\n"
"padding: 6px;\n"
"}"));

        retranslateUi(acercaDe);

        QMetaObject::connectSlotsByName(acercaDe);
    } // setupUi

    void retranslateUi(QDialog *acercaDe)
    {
        acercaDe->setWindowTitle(QApplication::translate("acercaDe", "Acerca de...", 0, QApplication::UnicodeUTF8));
        dago->setText(QApplication::translate("acercaDe", "Dagoberto Castellanos", 0, QApplication::UnicodeUTF8));
        richard->setText(QApplication::translate("acercaDe", "Richard Siwady", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class acercaDe: public Ui_acercaDe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACERCADE_H

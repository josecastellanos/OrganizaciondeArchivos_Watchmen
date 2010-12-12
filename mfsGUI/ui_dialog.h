/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Sun Dec 12 10:51:56 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label;
    QPushButton *btnMp3Folder;
    QLineEdit *lnSetExe;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lnDiskPath;
    QPushButton *btnCreate;
    QLineEdit *lnTagEntries;
    QLabel *label_6;
    QLineEdit *lnDiskName;
    QLabel *label_4;
    QLabel *label_8;
    QPushButton *btnDiskPath;
    QLabel *label_7;
    QPushButton *btnSetExe;
    QLineEdit *lnMp3Folder;
    QLineEdit *lnDiskSize;
    QLabel *label_5;
    QLineEdit *lnFileEntries;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(520, 400);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Dialog->setWindowIcon(icon);
        Dialog->setStyleSheet(QString::fromUtf8("QDialog\n"
"{\n"
"	background-image: url(:/images/fondond.png);\n"
"}"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 501, 91));
        label->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"	image: url(:/images/newdisk.png);\n"
"background-color: transparent;\n"
"border-style: outset;\n"
"border-width: 2px;\n"
"border-radius: 10px;\n"
"border-color: transparent;\n"
"font: bold 22px;\n"
"min-width:4m;\n"
"padding: 6px;\n"
"}"));
        btnMp3Folder = new QPushButton(Dialog);
        btnMp3Folder->setObjectName(QString::fromUtf8("btnMp3Folder"));
        btnMp3Folder->setGeometry(QRect(450, 230, 20, 26));
        lnSetExe = new QLineEdit(Dialog);
        lnSetExe->setObjectName(QString::fromUtf8("lnSetExe"));
        lnSetExe->setGeometry(QRect(130, 270, 311, 25));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 110, 100, 16));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(270, 110, 101, 16));
        lnDiskPath = new QLineEdit(Dialog);
        lnDiskPath->setObjectName(QString::fromUtf8("lnDiskPath"));
        lnDiskPath->setGeometry(QRect(130, 190, 311, 25));
        btnCreate = new QPushButton(Dialog);
        btnCreate->setObjectName(QString::fromUtf8("btnCreate"));
        btnCreate->setGeometry(QRect(180, 310, 161, 71));
        QFont font;
        font.setPointSize(22);
        font.setBold(false);
        font.setWeight(50);
        btnCreate->setFont(font);
        lnTagEntries = new QLineEdit(Dialog);
        lnTagEntries->setObjectName(QString::fromUtf8("lnTagEntries"));
        lnTagEntries->setGeometry(QRect(350, 150, 150, 25));
        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(50, 190, 101, 16));
        lnDiskName = new QLineEdit(Dialog);
        lnDiskName->setObjectName(QString::fromUtf8("lnDiskName"));
        lnDiskName->setGeometry(QRect(100, 110, 150, 25));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 150, 101, 16));
        label_8 = new QLabel(Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 270, 80, 16));
        btnDiskPath = new QPushButton(Dialog);
        btnDiskPath->setObjectName(QString::fromUtf8("btnDiskPath"));
        btnDiskPath->setGeometry(QRect(450, 190, 20, 26));
        label_7 = new QLabel(Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(50, 230, 80, 16));
        btnSetExe = new QPushButton(Dialog);
        btnSetExe->setObjectName(QString::fromUtf8("btnSetExe"));
        btnSetExe->setGeometry(QRect(450, 270, 20, 26));
        lnMp3Folder = new QLineEdit(Dialog);
        lnMp3Folder->setObjectName(QString::fromUtf8("lnMp3Folder"));
        lnMp3Folder->setGeometry(QRect(130, 230, 311, 25));
        lnDiskSize = new QLineEdit(Dialog);
        lnDiskSize->setObjectName(QString::fromUtf8("lnDiskSize"));
        lnDiskSize->setGeometry(QRect(100, 150, 150, 25));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(270, 150, 101, 16));
        lnFileEntries = new QLineEdit(Dialog);
        lnFileEntries->setObjectName(QString::fromUtf8("lnFileEntries"));
        lnFileEntries->setGeometry(QRect(350, 110, 150, 25));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Create Disk", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        btnMp3Folder->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        lnSetExe->setText(QString());
        label_2->setText(QApplication::translate("Dialog", "Disk Name:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "File Entries:", 0, QApplication::UnicodeUTF8));
        lnDiskPath->setText(QString());
        btnCreate->setText(QApplication::translate("Dialog", "Create!", 0, QApplication::UnicodeUTF8));
        lnTagEntries->setText(QApplication::translate("Dialog", "32", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog", "Disk Path:", 0, QApplication::UnicodeUTF8));
        lnDiskName->setText(QApplication::translate("Dialog", "disco.dat", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "Disk Size:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Dialog", "Set .exe:", 0, QApplication::UnicodeUTF8));
        btnDiskPath->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Dialog", "Mp3 Folder:", 0, QApplication::UnicodeUTF8));
        btnSetExe->setText(QApplication::translate("Dialog", "...", 0, QApplication::UnicodeUTF8));
        lnMp3Folder->setText(QString());
        lnDiskSize->setText(QApplication::translate("Dialog", "204800", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "Tag Entries:", 0, QApplication::UnicodeUTF8));
        lnFileEntries->setText(QApplication::translate("Dialog", "7", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H

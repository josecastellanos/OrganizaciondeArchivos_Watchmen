/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 25. Nov 14:30:14 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Disk;
    QAction *actionAdd_Song;
    QAction *actionExport_Song;
    QAction *actionDelete_Song;
    QAction *actionOpen_Disk;
    QAction *actionExit;
    QAction *actionSet_exe;
    QAction *actionShow_Tags;
    QAction *actionTag_Song;
    QAction *actionAbout_mfs;
    QWidget *centralWidget;
    QListWidget *lwTags;
    QListWidget *lwSongs;
    QGroupBox *groupBox;
    QLabel *InfoCancion;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuNew_Disk;
    QMenu *menuTools;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(900, 700);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow\n"
"{\n"
"background-image: url(:/images/fondo.png);\n"
"}"));
        actionNew_Disk = new QAction(MainWindow);
        actionNew_Disk->setObjectName(QString::fromUtf8("actionNew_Disk"));
        actionAdd_Song = new QAction(MainWindow);
        actionAdd_Song->setObjectName(QString::fromUtf8("actionAdd_Song"));
        actionExport_Song = new QAction(MainWindow);
        actionExport_Song->setObjectName(QString::fromUtf8("actionExport_Song"));
        actionDelete_Song = new QAction(MainWindow);
        actionDelete_Song->setObjectName(QString::fromUtf8("actionDelete_Song"));
        actionOpen_Disk = new QAction(MainWindow);
        actionOpen_Disk->setObjectName(QString::fromUtf8("actionOpen_Disk"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionSet_exe = new QAction(MainWindow);
        actionSet_exe->setObjectName(QString::fromUtf8("actionSet_exe"));
        actionShow_Tags = new QAction(MainWindow);
        actionShow_Tags->setObjectName(QString::fromUtf8("actionShow_Tags"));
        actionTag_Song = new QAction(MainWindow);
        actionTag_Song->setObjectName(QString::fromUtf8("actionTag_Song"));
        actionAbout_mfs = new QAction(MainWindow);
        actionAbout_mfs->setObjectName(QString::fromUtf8("actionAbout_mfs"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lwTags = new QListWidget(centralWidget);
        lwTags->setObjectName(QString::fromUtf8("lwTags"));
        lwTags->setGeometry(QRect(20, 20, 200, 601));
        lwTags->setStyleSheet(QString::fromUtf8(""));
        lwSongs = new QListWidget(centralWidget);
        lwSongs->setObjectName(QString::fromUtf8("lwSongs"));
        lwSongs->setGeometry(QRect(240, 80, 641, 321));
        lwSongs->setStyleSheet(QString::fromUtf8(""));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(240, 420, 421, 200));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox\n"
"{\n"
"	background-image: url(:/images/info2.png);\n"
"     border-style: outset;\n"
"     border-width: 1px;\n"
"     border-radius: 20px;\n"
"     border-color: white;\n"
"     font: bold 12px;\n"
"     min-width:4m;\n"
"     padding: 6px;\n"
"}\n"
""));
        InfoCancion = new QLabel(groupBox);
        InfoCancion->setObjectName(QString::fromUtf8("InfoCancion"));
        InfoCancion->setEnabled(true);
        InfoCancion->setGeometry(QRect(0, 0, 421, 200));
        InfoCancion->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(679, 420, 201, 200));
        label->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"	background-image: url(:/images/logo.png);\n"
"}"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuNew_Disk = new QMenu(menuBar);
        menuNew_Disk->setObjectName(QString::fromUtf8("menuNew_Disk"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuNew_Disk->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuNew_Disk->addAction(actionNew_Disk);
        menuNew_Disk->addAction(actionAdd_Song);
        menuNew_Disk->addAction(actionExport_Song);
        menuNew_Disk->addAction(actionTag_Song);
        menuNew_Disk->addAction(actionDelete_Song);
        menuNew_Disk->addAction(actionOpen_Disk);
        menuNew_Disk->addAction(actionExit);
        menuTools->addAction(actionSet_exe);
        menuTools->addAction(actionShow_Tags);
        menuAbout->addAction(actionAbout_mfs);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "mfs", 0, QApplication::UnicodeUTF8));
        actionNew_Disk->setText(QApplication::translate("MainWindow", "New Disk", 0, QApplication::UnicodeUTF8));
        actionAdd_Song->setText(QApplication::translate("MainWindow", "Add Song", 0, QApplication::UnicodeUTF8));
        actionExport_Song->setText(QApplication::translate("MainWindow", "Export Song", 0, QApplication::UnicodeUTF8));
        actionDelete_Song->setText(QApplication::translate("MainWindow", "Delete Song", 0, QApplication::UnicodeUTF8));
        actionOpen_Disk->setText(QApplication::translate("MainWindow", "Open Disk", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionSet_exe->setText(QApplication::translate("MainWindow", "Set .exe", 0, QApplication::UnicodeUTF8));
        actionShow_Tags->setText(QApplication::translate("MainWindow", "Show Tags", 0, QApplication::UnicodeUTF8));
        actionTag_Song->setText(QApplication::translate("MainWindow", "Tag Song", 0, QApplication::UnicodeUTF8));
        actionAbout_mfs->setText(QApplication::translate("MainWindow", "About mfs", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", 0, QApplication::UnicodeUTF8));
        InfoCancion->setText(QString());
        label->setText(QString());
        menuNew_Disk->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

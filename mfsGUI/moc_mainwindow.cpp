/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Nov 25 14:16:10 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      43,   11,   11,   11, 0x08,
      79,   74,   11,   11, 0x08,
     120,   74,   11,   11, 0x08,
     167,   74,   11,   11, 0x08,
     207,   11,   11,   11, 0x08,
     238,   11,   11,   11, 0x08,
     268,   11,   11,   11, 0x08,
     301,   11,   11,   11, 0x08,
     334,   11,   11,   11, 0x08,
     364,   11,   11,   11, 0x08,
     393,   11,   11,   11, 0x08,
     419,   11,   11,   11, 0x08,
     473,  449,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionAbout_mfs_triggered()\0"
    "on_actionOpen_Disk_triggered()\0item\0"
    "on_lwSongs_itemClicked(QListWidgetItem*)\0"
    "on_lwSongs_itemDoubleClicked(QListWidgetItem*)\0"
    "on_lwTags_itemClicked(QListWidgetItem*)\0"
    "on_actionShow_Tags_triggered()\0"
    "on_actionTag_Song_triggered()\0"
    "on_actionDelete_Song_triggered()\0"
    "on_actionExport_Song_triggered()\0"
    "on_actionAdd_Song_triggered()\0"
    "on_actionSet_exe_triggered()\0"
    "on_actionExit_triggered()\0"
    "on_actionNew_Disk_triggered()\0"
    "ExePath,listaParametros\0"
    "createDisk(QString*,QStringList*)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_actionAbout_mfs_triggered(); break;
        case 1: on_actionOpen_Disk_triggered(); break;
        case 2: on_lwSongs_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: on_lwSongs_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: on_lwTags_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: on_actionShow_Tags_triggered(); break;
        case 6: on_actionTag_Song_triggered(); break;
        case 7: on_actionDelete_Song_triggered(); break;
        case 8: on_actionExport_Song_triggered(); break;
        case 9: on_actionAdd_Song_triggered(); break;
        case 10: on_actionSet_exe_triggered(); break;
        case 11: on_actionExit_triggered(); break;
        case 12: on_actionNew_Disk_triggered(); break;
        case 13: createDisk((*reinterpret_cast< QString*(*)>(_a[1])),(*reinterpret_cast< QStringList*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

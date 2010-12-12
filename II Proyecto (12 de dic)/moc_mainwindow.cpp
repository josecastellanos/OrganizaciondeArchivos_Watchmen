/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Dec 12 16:39:37 2010
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      45,   11,   11,   11, 0x08,
      84,   11,   11,   11, 0x08,
     105,   11,   11,   11, 0x08,
     131,  125,   11,   11, 0x09,
     194,  156,   11,   11, 0x09,
     248,  241,   11,   11, 0x09,
     276,  268,   11,   11, 0x09,
     307,  297,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionCreate_Disk_triggered()\0"
    "on_actionAbout_II_Proyecto_triggered()\0"
    "on_btnFind_clicked()\0on_btnAdd_clicked()\0"
    "event\0closeEvent(QCloseEvent*)\0"
    "id,time,source,destino,protocolo,info\0"
    "addRecord(long,double,char*,char*,char*,char*)\0"
    "source\0searchSource(char*)\0destino\0"
    "searchDestino(char*)\0protocolo\0"
    "searchProtocolo(char*)\0"
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
        case 0: on_actionCreate_Disk_triggered(); break;
        case 1: on_actionAbout_II_Proyecto_triggered(); break;
        case 2: on_btnFind_clicked(); break;
        case 3: on_btnAdd_clicked(); break;
        case 4: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 5: addRecord((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< char*(*)>(_a[3])),(*reinterpret_cast< char*(*)>(_a[4])),(*reinterpret_cast< char*(*)>(_a[5])),(*reinterpret_cast< char*(*)>(_a[6]))); break;
        case 6: searchSource((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 7: searchDestino((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 8: searchProtocolo((*reinterpret_cast< char*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

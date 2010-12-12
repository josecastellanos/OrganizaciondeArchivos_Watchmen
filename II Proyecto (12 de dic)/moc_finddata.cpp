/****************************************************************************
** Meta object code from reading C++ file 'finddata.h'
**
** Created: Sun Dec 12 10:11:24 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "finddata.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'finddata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_findData[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   10,    9,    9, 0x05,
      45,   37,    9,    9, 0x05,
      76,   66,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,    9,    9,    9, 0x08,
     124,    9,    9,    9, 0x08,
     147,    9,    9,    9, 0x08,
     169,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_findData[] = {
    "findData\0\0source\0searchSource(char*)\0"
    "destino\0searchDestino(char*)\0protocolo\0"
    "searchProtocolo(char*)\0on_rbProtocolo_clicked()\0"
    "on_rbDestino_clicked()\0on_rdSource_clicked()\0"
    "on_find_clicked()\0"
};

const QMetaObject findData::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_findData,
      qt_meta_data_findData, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &findData::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *findData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *findData::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_findData))
        return static_cast<void*>(const_cast< findData*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int findData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: searchSource((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 1: searchDestino((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 2: searchProtocolo((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: on_rbProtocolo_clicked(); break;
        case 4: on_rbDestino_clicked(); break;
        case 5: on_rdSource_clicked(); break;
        case 6: on_find_clicked(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void findData::searchSource(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void findData::searchDestino(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void findData::searchProtocolo(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

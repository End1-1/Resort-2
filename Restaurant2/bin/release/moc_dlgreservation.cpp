/****************************************************************************
** Meta object code from reading C++ file 'dlgreservation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dlgreservation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgreservation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgReservation_t {
    QByteArrayData data[11];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgReservation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgReservation_t qt_meta_stringdata_DlgReservation = {
    {
QT_MOC_LITERAL(0, 0, 14), // "DlgReservation"
QT_MOC_LITERAL(1, 15, 7), // "kdbText"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "text"
QT_MOC_LITERAL(4, 29, 9), // "kbdAccept"
QT_MOC_LITERAL(5, 39, 23), // "on_leSearch_textChanged"
QT_MOC_LITERAL(6, 63, 4), // "arg1"
QT_MOC_LITERAL(7, 68, 19), // "on_btnClear_clicked"
QT_MOC_LITERAL(8, 88, 18), // "on_tblData_clicked"
QT_MOC_LITERAL(9, 107, 5), // "index"
QT_MOC_LITERAL(10, 113, 18) // "on_tblItem_clicked"

    },
    "DlgReservation\0kdbText\0\0text\0kbdAccept\0"
    "on_leSearch_textChanged\0arg1\0"
    "on_btnClear_clicked\0on_tblData_clicked\0"
    "index\0on_tblItem_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgReservation[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       4,    0,   47,    2, 0x08 /* Private */,
       5,    1,   48,    2, 0x08 /* Private */,
       7,    0,   51,    2, 0x08 /* Private */,
       8,    1,   52,    2, 0x08 /* Private */,
      10,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    9,
    QMetaType::Void, QMetaType::QModelIndex,    9,

       0        // eod
};

void DlgReservation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgReservation *_t = static_cast<DlgReservation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->kdbText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->kbdAccept(); break;
        case 2: _t->on_leSearch_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_btnClear_clicked(); break;
        case 4: _t->on_tblData_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->on_tblItem_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DlgReservation::staticMetaObject = {
    { &BaseExtendedDialog::staticMetaObject, qt_meta_stringdata_DlgReservation.data,
      qt_meta_data_DlgReservation,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgReservation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgReservation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgReservation.stringdata0))
        return static_cast<void*>(this);
    return BaseExtendedDialog::qt_metacast(_clname);
}

int DlgReservation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseExtendedDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

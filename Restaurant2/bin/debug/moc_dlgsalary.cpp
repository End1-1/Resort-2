/****************************************************************************
** Meta object code from reading C++ file 'dlgsalary.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dlgsalary.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgsalary.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgSalary_t {
    QByteArrayData data[9];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgSalary_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgSalary_t qt_meta_stringdata_DlgSalary = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DlgSalary"
QT_MOC_LITERAL(1, 10, 19), // "on_btnClose_clicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "on_btnSave_clicked"
QT_MOC_LITERAL(4, 50, 17), // "on_btnAdd_clicked"
QT_MOC_LITERAL(5, 68, 20), // "on_btnRemove_clicked"
QT_MOC_LITERAL(6, 89, 20), // "on_btnAmount_clicked"
QT_MOC_LITERAL(7, 110, 21), // "on_deDate_dateChanged"
QT_MOC_LITERAL(8, 132, 4) // "date"

    },
    "DlgSalary\0on_btnClose_clicked\0\0"
    "on_btnSave_clicked\0on_btnAdd_clicked\0"
    "on_btnRemove_clicked\0on_btnAmount_clicked\0"
    "on_deDate_dateChanged\0date"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgSalary[] = {

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
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    1,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    8,

       0        // eod
};

void DlgSalary::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgSalary *_t = static_cast<DlgSalary *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnClose_clicked(); break;
        case 1: _t->on_btnSave_clicked(); break;
        case 2: _t->on_btnAdd_clicked(); break;
        case 3: _t->on_btnRemove_clicked(); break;
        case 4: _t->on_btnAmount_clicked(); break;
        case 5: _t->on_deDate_dateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DlgSalary::staticMetaObject = {
    { &BaseExtendedDialog::staticMetaObject, qt_meta_stringdata_DlgSalary.data,
      qt_meta_data_DlgSalary,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgSalary::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgSalary::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgSalary.stringdata0))
        return static_cast<void*>(this);
    return BaseExtendedDialog::qt_metacast(_clname);
}

int DlgSalary::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

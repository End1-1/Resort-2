/****************************************************************************
** Meta object code from reading C++ file 'dlgdeptholder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dlgdeptholder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgdeptholder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgDeptHolder_t {
    QByteArrayData data[8];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgDeptHolder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgDeptHolder_t qt_meta_stringdata_DlgDeptHolder = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DlgDeptHolder"
QT_MOC_LITERAL(1, 14, 20), // "on_btnCancel_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 18), // "on_btnSave_clicked"
QT_MOC_LITERAL(4, 55, 23), // "on_btnCreateNew_clicked"
QT_MOC_LITERAL(5, 79, 25), // "on_lstHolders_itemClicked"
QT_MOC_LITERAL(6, 105, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(7, 122, 4) // "item"

    },
    "DlgDeptHolder\0on_btnCancel_clicked\0\0"
    "on_btnSave_clicked\0on_btnCreateNew_clicked\0"
    "on_lstHolders_itemClicked\0QListWidgetItem*\0"
    "item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgDeptHolder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    1,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void DlgDeptHolder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DlgDeptHolder *_t = static_cast<DlgDeptHolder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnCancel_clicked(); break;
        case 1: _t->on_btnSave_clicked(); break;
        case 2: _t->on_btnCreateNew_clicked(); break;
        case 3: _t->on_lstHolders_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DlgDeptHolder::staticMetaObject = {
    { &BaseExtendedDialog::staticMetaObject, qt_meta_stringdata_DlgDeptHolder.data,
      qt_meta_data_DlgDeptHolder,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DlgDeptHolder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgDeptHolder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgDeptHolder.stringdata0))
        return static_cast<void*>(this);
    return BaseExtendedDialog::qt_metacast(_clname);
}

int DlgDeptHolder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseExtendedDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

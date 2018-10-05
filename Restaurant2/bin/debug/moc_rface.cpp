/****************************************************************************
** Meta object code from reading C++ file 'rface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../rface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RFace_t {
    QByteArrayData data[17];
    char stringdata0[266];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RFace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RFace_t qt_meta_stringdata_RFace = {
    {
QT_MOC_LITERAL(0, 0, 5), // "RFace"
QT_MOC_LITERAL(1, 6, 7), // "timeout"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 15), // "socketReadyRead"
QT_MOC_LITERAL(4, 31, 18), // "socketDisconnected"
QT_MOC_LITERAL(5, 50, 12), // "parseCommand"
QT_MOC_LITERAL(6, 63, 7), // "command"
QT_MOC_LITERAL(7, 71, 22), // "on_tableWidget_clicked"
QT_MOC_LITERAL(8, 94, 5), // "index"
QT_MOC_LITERAL(9, 100, 18), // "on_btnExit_clicked"
QT_MOC_LITERAL(10, 119, 20), // "on_tblTables_clicked"
QT_MOC_LITERAL(11, 140, 16), // "on_btnUp_clicked"
QT_MOC_LITERAL(12, 157, 18), // "on_btnDown_clicked"
QT_MOC_LITERAL(13, 176, 24), // "on_btnChangeHall_clicked"
QT_MOC_LITERAL(14, 201, 23), // "on_btnBreakFast_clicked"
QT_MOC_LITERAL(15, 225, 20), // "on_btnBanket_clicked"
QT_MOC_LITERAL(16, 246, 19) // "on_btnTools_clicked"

    },
    "RFace\0timeout\0\0socketReadyRead\0"
    "socketDisconnected\0parseCommand\0command\0"
    "on_tableWidget_clicked\0index\0"
    "on_btnExit_clicked\0on_tblTables_clicked\0"
    "on_btnUp_clicked\0on_btnDown_clicked\0"
    "on_btnChangeHall_clicked\0"
    "on_btnBreakFast_clicked\0on_btnBanket_clicked\0"
    "on_btnTools_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RFace[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    1,   82,    2, 0x08 /* Private */,
       7,    1,   85,    2, 0x08 /* Private */,
       9,    0,   88,    2, 0x08 /* Private */,
      10,    1,   89,    2, 0x08 /* Private */,
      11,    0,   92,    2, 0x08 /* Private */,
      12,    0,   93,    2, 0x08 /* Private */,
      13,    0,   94,    2, 0x08 /* Private */,
      14,    0,   95,    2, 0x08 /* Private */,
      15,    0,   96,    2, 0x08 /* Private */,
      16,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RFace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RFace *_t = static_cast<RFace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timeout(); break;
        case 1: _t->socketReadyRead(); break;
        case 2: _t->socketDisconnected(); break;
        case 3: _t->parseCommand((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->on_tableWidget_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->on_btnExit_clicked(); break;
        case 6: _t->on_tblTables_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: _t->on_btnUp_clicked(); break;
        case 8: _t->on_btnDown_clicked(); break;
        case 9: _t->on_btnChangeHall_clicked(); break;
        case 10: _t->on_btnBreakFast_clicked(); break;
        case 11: _t->on_btnBanket_clicked(); break;
        case 12: _t->on_btnTools_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject RFace::staticMetaObject = {
    { &BaseExtendedDialog::staticMetaObject, qt_meta_stringdata_RFace.data,
      qt_meta_data_RFace,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RFace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RFace::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RFace.stringdata0))
        return static_cast<void*>(this);
    return BaseExtendedDialog::qt_metacast(_clname);
}

int RFace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseExtendedDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

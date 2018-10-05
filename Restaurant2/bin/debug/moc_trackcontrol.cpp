/****************************************************************************
** Meta object code from reading C++ file 'trackcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Controls/trackcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trackcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TrackControl_t {
    QByteArrayData data[15];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackControl_t qt_meta_stringdata_TrackControl = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TrackControl"
QT_MOC_LITERAL(1, 13, 19), // "lineEditTextChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "text"
QT_MOC_LITERAL(4, 39, 16), // "edateTextChanged"
QT_MOC_LITERAL(5, 56, 15), // "dateEditChanged"
QT_MOC_LITERAL(6, 72, 4), // "date"
QT_MOC_LITERAL(7, 77, 19), // "textEditTextChanged"
QT_MOC_LITERAL(8, 97, 15), // "checkBoxClicked"
QT_MOC_LITERAL(9, 113, 17), // "comboIndexChanged"
QT_MOC_LITERAL(10, 131, 5), // "index"
QT_MOC_LITERAL(11, 137, 16), // "spinValueChanged"
QT_MOC_LITERAL(12, 154, 5), // "value"
QT_MOC_LITERAL(13, 160, 15), // "timeEditChanged"
QT_MOC_LITERAL(14, 176, 4) // "time"

    },
    "TrackControl\0lineEditTextChanged\0\0"
    "text\0edateTextChanged\0dateEditChanged\0"
    "date\0textEditTextChanged\0checkBoxClicked\0"
    "comboIndexChanged\0index\0spinValueChanged\0"
    "value\0timeEditChanged\0time"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    1,   57,    2, 0x08 /* Private */,
       5,    1,   60,    2, 0x08 /* Private */,
       7,    0,   63,    2, 0x08 /* Private */,
       8,    0,   64,    2, 0x08 /* Private */,
       9,    1,   65,    2, 0x08 /* Private */,
      11,    1,   68,    2, 0x08 /* Private */,
      13,    1,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QDate,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::QTime,   14,

       0        // eod
};

void TrackControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrackControl *_t = static_cast<TrackControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->lineEditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->edateTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->dateEditChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 3: _t->textEditTextChanged(); break;
        case 4: _t->checkBoxClicked(); break;
        case 5: _t->comboIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->spinValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->timeEditChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TrackControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrackControl.data,
      qt_meta_data_TrackControl,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TrackControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TrackControl.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TrackControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

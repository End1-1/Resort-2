/****************************************************************************
** Meta object code from reading C++ file 'rkeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Restaurant2/rkeyboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rkeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RKeyboard_t {
    QByteArrayData data[18];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RKeyboard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RKeyboard_t qt_meta_stringdata_RKeyboard = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RKeyboard"
QT_MOC_LITERAL(1, 10, 11), // "textChanged"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "text"
QT_MOC_LITERAL(4, 28, 8), // "accepted"
QT_MOC_LITERAL(5, 37, 8), // "rejected"
QT_MOC_LITERAL(6, 46, 14), // "btnTextClicked"
QT_MOC_LITERAL(7, 61, 19), // "on_btnSpace_clicked"
QT_MOC_LITERAL(8, 81, 23), // "on_btnBackspace_clicked"
QT_MOC_LITERAL(9, 105, 18), // "on_btnCaps_clicked"
QT_MOC_LITERAL(10, 124, 20), // "on_btnLShift_clicked"
QT_MOC_LITERAL(11, 145, 20), // "on_btnCancel_clicked"
QT_MOC_LITERAL(12, 166, 16), // "on_btnOk_clicked"
QT_MOC_LITERAL(13, 183, 20), // "on_btnRShift_clicked"
QT_MOC_LITERAL(14, 204, 16), // "on_btnEn_clicked"
QT_MOC_LITERAL(15, 221, 16), // "on_btnAm_clicked"
QT_MOC_LITERAL(16, 238, 16), // "on_btnRu_clicked"
QT_MOC_LITERAL(17, 255, 18) // "on_btnList_clicked"

    },
    "RKeyboard\0textChanged\0\0text\0accepted\0"
    "rejected\0btnTextClicked\0on_btnSpace_clicked\0"
    "on_btnBackspace_clicked\0on_btnCaps_clicked\0"
    "on_btnLShift_clicked\0on_btnCancel_clicked\0"
    "on_btnOk_clicked\0on_btnRShift_clicked\0"
    "on_btnEn_clicked\0on_btnAm_clicked\0"
    "on_btnRu_clicked\0on_btnList_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RKeyboard[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    0,   92,    2, 0x06 /* Public */,
       5,    0,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    0,  104,    2, 0x08 /* Private */,
      17,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RKeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RKeyboard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->accepted(); break;
        case 2: _t->rejected(); break;
        case 3: _t->btnTextClicked(); break;
        case 4: _t->on_btnSpace_clicked(); break;
        case 5: _t->on_btnBackspace_clicked(); break;
        case 6: _t->on_btnCaps_clicked(); break;
        case 7: _t->on_btnLShift_clicked(); break;
        case 8: _t->on_btnCancel_clicked(); break;
        case 9: _t->on_btnOk_clicked(); break;
        case 10: _t->on_btnRShift_clicked(); break;
        case 11: _t->on_btnEn_clicked(); break;
        case 12: _t->on_btnAm_clicked(); break;
        case 13: _t->on_btnRu_clicked(); break;
        case 14: _t->on_btnList_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RKeyboard::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RKeyboard::textChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RKeyboard::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RKeyboard::accepted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RKeyboard::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RKeyboard::rejected)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RKeyboard::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_RKeyboard.data,
    qt_meta_data_RKeyboard,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RKeyboard.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void RKeyboard::textChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RKeyboard::accepted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RKeyboard::rejected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

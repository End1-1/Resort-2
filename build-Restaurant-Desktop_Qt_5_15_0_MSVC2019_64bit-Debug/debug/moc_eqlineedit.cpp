/****************************************************************************
** Meta object code from reading C++ file 'eqlineedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Controls/eqlineedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eqlineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EQLineEdit_t {
    QByteArrayData data[16];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EQLineEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EQLineEdit_t qt_meta_stringdata_EQLineEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EQLineEdit"
QT_MOC_LITERAL(1, 11, 19), // "customButtonClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "value"
QT_MOC_LITERAL(4, 38, 7), // "focusIn"
QT_MOC_LITERAL(5, 46, 8), // "focusOut"
QT_MOC_LITERAL(6, 55, 13), // "buttonClicked"
QT_MOC_LITERAL(7, 69, 21), // "correctButtonPosition"
QT_MOC_LITERAL(8, 91, 17), // "hiddenTextChanged"
QT_MOC_LITERAL(9, 109, 4), // "text"
QT_MOC_LITERAL(10, 114, 10), // "ShowButton"
QT_MOC_LITERAL(11, 125, 11), // "AlwaysUpper"
QT_MOC_LITERAL(12, 137, 5), // "Field"
QT_MOC_LITERAL(13, 143, 10), // "PrimaryKey"
QT_MOC_LITERAL(14, 154, 16), // "EnableHiddenText"
QT_MOC_LITERAL(15, 171, 19) // "HiddenTextValidator"

    },
    "EQLineEdit\0customButtonClicked\0\0value\0"
    "focusIn\0focusOut\0buttonClicked\0"
    "correctButtonPosition\0hiddenTextChanged\0"
    "text\0ShowButton\0AlwaysUpper\0Field\0"
    "PrimaryKey\0EnableHiddenText\0"
    "HiddenTextValidator"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EQLineEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       6,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,
       5,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   49,    2, 0x08 /* Private */,
       7,    0,   52,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,

 // properties: name, type, flags
      10, QMetaType::Bool, 0x00095003,
      11, QMetaType::Bool, 0x00095103,
      12, QMetaType::QString, 0x00095103,
      13, QMetaType::Bool, 0x00095103,
      14, QMetaType::Bool, 0x00095003,
      15, QMetaType::QString, 0x00095103,

       0        // eod
};

void EQLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EQLineEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->customButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->focusIn(); break;
        case 2: _t->focusOut(); break;
        case 3: _t->buttonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->correctButtonPosition(); break;
        case 5: _t->hiddenTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EQLineEdit::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EQLineEdit::customButtonClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EQLineEdit::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EQLineEdit::focusIn)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EQLineEdit::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EQLineEdit::focusOut)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<EQLineEdit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getShowButton(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getAlwaysUpper(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getField(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->getPrimaryKey(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->getHiddenTextEnabled(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getHiddenTextValidator(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<EQLineEdit *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setShowButtonOnFocus(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setAlwaysUpper(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setField(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setPrimaryKey(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setHiddenTextEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setHiddenTextValidator(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject EQLineEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QLineEdit::staticMetaObject>(),
    qt_meta_stringdata_EQLineEdit.data,
    qt_meta_data_EQLineEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EQLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EQLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EQLineEdit.stringdata0))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int EQLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void EQLineEdit::customButtonClicked(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EQLineEdit::focusIn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void EQLineEdit::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

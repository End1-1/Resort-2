/****************************************************************************
** Meta object code from reading C++ file 'pprintpreview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Print/pprintpreview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pprintpreview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PPrintPreview_t {
    QByteArrayData data[12];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PPrintPreview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PPrintPreview_t qt_meta_stringdata_PPrintPreview = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PPrintPreview"
QT_MOC_LITERAL(1, 14, 19), // "on_btnPrint_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 21), // "on_btnZoomOut_clicked"
QT_MOC_LITERAL(4, 57, 29), // "on_cbZoom_currentIndexChanged"
QT_MOC_LITERAL(5, 87, 5), // "index"
QT_MOC_LITERAL(6, 93, 20), // "on_btnZoopIn_clicked"
QT_MOC_LITERAL(7, 114, 18), // "on_btnBack_clicked"
QT_MOC_LITERAL(8, 133, 18), // "on_btnNext_clicked"
QT_MOC_LITERAL(9, 152, 19), // "on_btnFirst_clicked"
QT_MOC_LITERAL(10, 172, 18), // "on_btnLast_clicked"
QT_MOC_LITERAL(11, 191, 39) // "on_cbPrintSelection_currentIn..."

    },
    "PPrintPreview\0on_btnPrint_clicked\0\0"
    "on_btnZoomOut_clicked\0"
    "on_cbZoom_currentIndexChanged\0index\0"
    "on_btnZoopIn_clicked\0on_btnBack_clicked\0"
    "on_btnNext_clicked\0on_btnFirst_clicked\0"
    "on_btnLast_clicked\0"
    "on_cbPrintSelection_currentIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PPrintPreview[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    1,   61,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    1,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void PPrintPreview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PPrintPreview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnPrint_clicked(); break;
        case 1: _t->on_btnZoomOut_clicked(); break;
        case 2: _t->on_cbZoom_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_btnZoopIn_clicked(); break;
        case 4: _t->on_btnBack_clicked(); break;
        case 5: _t->on_btnNext_clicked(); break;
        case 6: _t->on_btnFirst_clicked(); break;
        case 7: _t->on_btnLast_clicked(); break;
        case 8: _t->on_cbPrintSelection_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PPrintPreview::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_PPrintPreview.data,
    qt_meta_data_PPrintPreview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PPrintPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PPrintPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PPrintPreview.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PPrintPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'socketconnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Restaurant2/socketconnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'socketconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SocketConnection_t {
    QByteArrayData data[18];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SocketConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SocketConnection_t qt_meta_stringdata_SocketConnection = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SocketConnection"
QT_MOC_LITERAL(1, 17, 7), // "stopped"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "connected"
QT_MOC_LITERAL(4, 36, 14), // "connectionLost"
QT_MOC_LITERAL(5, 51, 9), // "dataReady"
QT_MOC_LITERAL(6, 61, 11), // "QByteArray&"
QT_MOC_LITERAL(7, 73, 17), // "externalDataReady"
QT_MOC_LITERAL(8, 91, 3), // "run"
QT_MOC_LITERAL(9, 95, 8), // "sendData"
QT_MOC_LITERAL(10, 104, 1), // "d"
QT_MOC_LITERAL(11, 106, 7), // "timeout"
QT_MOC_LITERAL(12, 114, 9), // "encrypted"
QT_MOC_LITERAL(13, 124, 12), // "disconnected"
QT_MOC_LITERAL(14, 137, 9), // "readyRead"
QT_MOC_LITERAL(15, 147, 13), // "errorOccurred"
QT_MOC_LITERAL(16, 161, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(17, 190, 5) // "error"

    },
    "SocketConnection\0stopped\0\0connected\0"
    "connectionLost\0dataReady\0QByteArray&\0"
    "externalDataReady\0run\0sendData\0d\0"
    "timeout\0encrypted\0disconnected\0readyRead\0"
    "errorOccurred\0QAbstractSocket::SocketError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SocketConnection[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,
       7,    3,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   87,    2, 0x0a /* Public */,
       9,    1,   88,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x08 /* Private */,
      12,    0,   92,    2, 0x08 /* Private */,
      13,    0,   93,    2, 0x08 /* Private */,
      14,    0,   94,    2, 0x08 /* Private */,
      15,    1,   95,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::UShort, QMetaType::UInt, QMetaType::QByteArray,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void SocketConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SocketConnection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopped(); break;
        case 1: _t->connected(); break;
        case 2: _t->connectionLost(); break;
        case 3: _t->dataReady((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->externalDataReady((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 5: _t->run(); break;
        case 6: _t->sendData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->timeout(); break;
        case 8: _t->encrypted(); break;
        case 9: _t->disconnected(); break;
        case 10: _t->readyRead(); break;
        case 11: _t->errorOccurred((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SocketConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketConnection::stopped)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SocketConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketConnection::connected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SocketConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketConnection::connectionLost)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SocketConnection::*)(QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketConnection::dataReady)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SocketConnection::*)(quint16 , quint32 , QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketConnection::externalDataReady)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SocketConnection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SocketConnection.data,
    qt_meta_data_SocketConnection,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SocketConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SocketConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SocketConnection.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SocketConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SocketConnection::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SocketConnection::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SocketConnection::connectionLost()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SocketConnection::dataReady(QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SocketConnection::externalDataReady(quint16 _t1, quint32 _t2, QByteArray _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

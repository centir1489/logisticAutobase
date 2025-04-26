/****************************************************************************
** Meta object code from reading C++ file 'parcelCard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../Client/include/parcelCard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parcelCard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ParcelCard_t {
    QByteArrayData data[17];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParcelCard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParcelCard_t qt_meta_stringdata_ParcelCard = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ParcelCard"
QT_MOC_LITERAL(1, 11, 13), // "openRequested"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "parcelData"
QT_MOC_LITERAL(4, 37, 13), // "editRequested"
QT_MOC_LITERAL(5, 51, 13), // "moveRequested"
QT_MOC_LITERAL(6, 65, 12), // "parcelNumber"
QT_MOC_LITERAL(7, 78, 13), // "sourceTableId"
QT_MOC_LITERAL(8, 92, 11), // "targetTable"
QT_MOC_LITERAL(9, 104, 15), // "deleteRequested"
QT_MOC_LITERAL(10, 120, 7), // "tableId"
QT_MOC_LITERAL(11, 128, 15), // "showContextMenu"
QT_MOC_LITERAL(12, 144, 3), // "pos"
QT_MOC_LITERAL(13, 148, 16), // "handleOpenAction"
QT_MOC_LITERAL(14, 165, 16), // "handleEditAction"
QT_MOC_LITERAL(15, 182, 16), // "handleMoveAction"
QT_MOC_LITERAL(16, 199, 15) // "handleAddAction"

    },
    "ParcelCard\0openRequested\0\0parcelData\0"
    "editRequested\0moveRequested\0parcelNumber\0"
    "sourceTableId\0targetTable\0deleteRequested\0"
    "tableId\0showContextMenu\0pos\0"
    "handleOpenAction\0handleEditAction\0"
    "handleMoveAction\0handleAddAction"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParcelCard[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       5,    3,   65,    2, 0x06 /* Public */,
       9,    2,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   77,    2, 0x08 /* Private */,
      13,    0,   80,    2, 0x08 /* Private */,
      14,    0,   81,    2, 0x08 /* Private */,
      15,    0,   82,    2, 0x08 /* Private */,
      16,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ParcelCard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ParcelCard *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openRequested((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        case 1: _t->editRequested((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        case 2: _t->moveRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->deleteRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->handleOpenAction(); break;
        case 6: _t->handleEditAction(); break;
        case 7: _t->handleMoveAction(); break;
        case 8: _t->handleAddAction(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ParcelCard::*)(const QVariantMap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParcelCard::openRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ParcelCard::*)(const QVariantMap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParcelCard::editRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ParcelCard::*)(const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParcelCard::moveRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ParcelCard::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ParcelCard::deleteRequested)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ParcelCard::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_meta_stringdata_ParcelCard.data,
    qt_meta_data_ParcelCard,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ParcelCard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParcelCard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParcelCard.stringdata0))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int ParcelCard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void ParcelCard::openRequested(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ParcelCard::editRequested(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ParcelCard::moveRequested(const QString & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ParcelCard::deleteRequested(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

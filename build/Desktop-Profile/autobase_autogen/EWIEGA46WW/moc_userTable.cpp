/****************************************************************************
** Meta object code from reading C++ file 'userTable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../userTable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userTable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserTable_t {
    QByteArrayData data[21];
    char stringdata0[271];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserTable_t qt_meta_stringdata_UserTable = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UserTable"
QT_MOC_LITERAL(1, 10, 12), // "tableRenamed"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "newName"
QT_MOC_LITERAL(4, 32, 12), // "tableDeleted"
QT_MOC_LITERAL(5, 45, 9), // "tableName"
QT_MOC_LITERAL(6, 55, 13), // "tableExported"
QT_MOC_LITERAL(7, 69, 8), // "filePath"
QT_MOC_LITERAL(8, 78, 6), // "format"
QT_MOC_LITERAL(9, 85, 11), // "tableViewed"
QT_MOC_LITERAL(10, 97, 7), // "tableId"
QT_MOC_LITERAL(11, 105, 11), // "tableOpened"
QT_MOC_LITERAL(12, 117, 18), // "addParcelRequested"
QT_MOC_LITERAL(13, 136, 15), // "showContextMenu"
QT_MOC_LITERAL(14, 152, 3), // "pos"
QT_MOC_LITERAL(15, 156, 18), // "handleViewedAction"
QT_MOC_LITERAL(16, 175, 16), // "handleOpenAction"
QT_MOC_LITERAL(17, 192, 18), // "handleRenameAction"
QT_MOC_LITERAL(18, 211, 18), // "handleExportAction"
QT_MOC_LITERAL(19, 230, 18), // "handleDeleteAction"
QT_MOC_LITERAL(20, 249, 21) // "handleAddParcelAction"

    },
    "UserTable\0tableRenamed\0\0newName\0"
    "tableDeleted\0tableName\0tableExported\0"
    "filePath\0format\0tableViewed\0tableId\0"
    "tableOpened\0addParcelRequested\0"
    "showContextMenu\0pos\0handleViewedAction\0"
    "handleOpenAction\0handleRenameAction\0"
    "handleExportAction\0handleDeleteAction\0"
    "handleAddParcelAction"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserTable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    3,   85,    2, 0x06 /* Public */,
       9,    2,   92,    2, 0x06 /* Public */,
      11,    2,   97,    2, 0x06 /* Public */,
      12,    1,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,  105,    2, 0x08 /* Private */,
      15,    0,  108,    2, 0x08 /* Private */,
      16,    0,  109,    2, 0x08 /* Private */,
      17,    0,  110,    2, 0x08 /* Private */,
      18,    0,  111,    2, 0x08 /* Private */,
      19,    0,  112,    2, 0x08 /* Private */,
      20,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    5,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,   10,
    QMetaType::Void, QMetaType::Int,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserTable *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tableRenamed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->tableDeleted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->tableExported((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->tableViewed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->tableOpened((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->addParcelRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->handleViewedAction(); break;
        case 8: _t->handleOpenAction(); break;
        case 9: _t->handleRenameAction(); break;
        case 10: _t->handleExportAction(); break;
        case 11: _t->handleDeleteAction(); break;
        case 12: _t->handleAddParcelAction(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserTable::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::tableRenamed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserTable::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::tableDeleted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserTable::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::tableExported)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserTable::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::tableViewed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UserTable::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::tableOpened)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UserTable::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserTable::addParcelRequested)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserTable::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_meta_stringdata_UserTable.data,
    qt_meta_data_UserTable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserTable.stringdata0))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int UserTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void UserTable::tableRenamed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserTable::tableDeleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserTable::tableExported(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UserTable::tableViewed(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UserTable::tableOpened(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void UserTable::addParcelRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

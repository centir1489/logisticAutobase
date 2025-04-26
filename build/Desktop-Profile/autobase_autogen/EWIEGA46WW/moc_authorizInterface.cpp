/****************************************************************************
** Meta object code from reading C++ file 'authorizInterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../authorizInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'authorizInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AuthorizInterface_t {
    QByteArrayData data[15];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuthorizInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuthorizInterface_t qt_meta_stringdata_AuthorizInterface = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AuthorizInterface"
QT_MOC_LITERAL(1, 18, 13), // "authenticated"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "username"
QT_MOC_LITERAL(4, 42, 21), // "registrationCompleted"
QT_MOC_LITERAL(5, 64, 16), // "onProceedClicked"
QT_MOC_LITERAL(6, 81, 14), // "onLoginClicked"
QT_MOC_LITERAL(7, 96, 17), // "onRegisterClicked"
QT_MOC_LITERAL(8, 114, 17), // "showPasswordInput"
QT_MOC_LITERAL(9, 132, 21), // "showRegistrationInput"
QT_MOC_LITERAL(10, 154, 17), // "handleAuthSuccess"
QT_MOC_LITERAL(11, 172, 16), // "handleAuthFailed"
QT_MOC_LITERAL(12, 189, 6), // "reason"
QT_MOC_LITERAL(13, 196, 25), // "handleRegistrationSuccess"
QT_MOC_LITERAL(14, 222, 24) // "handleRegistrationFailed"

    },
    "AuthorizInterface\0authenticated\0\0"
    "username\0registrationCompleted\0"
    "onProceedClicked\0onLoginClicked\0"
    "onRegisterClicked\0showPasswordInput\0"
    "showRegistrationInput\0handleAuthSuccess\0"
    "handleAuthFailed\0reason\0"
    "handleRegistrationSuccess\0"
    "handleRegistrationFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuthorizInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   75,    2, 0x08 /* Private */,
       6,    0,   76,    2, 0x08 /* Private */,
       7,    0,   77,    2, 0x08 /* Private */,
       8,    0,   78,    2, 0x08 /* Private */,
       9,    0,   79,    2, 0x08 /* Private */,
      10,    1,   80,    2, 0x08 /* Private */,
      11,    1,   83,    2, 0x08 /* Private */,
      13,    1,   86,    2, 0x08 /* Private */,
      14,    1,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void AuthorizInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AuthorizInterface *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->authenticated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->registrationCompleted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onProceedClicked(); break;
        case 3: _t->onLoginClicked(); break;
        case 4: _t->onRegisterClicked(); break;
        case 5: _t->showPasswordInput(); break;
        case 6: _t->showRegistrationInput(); break;
        case 7: _t->handleAuthSuccess((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->handleAuthFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->handleRegistrationSuccess((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->handleRegistrationFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AuthorizInterface::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthorizInterface::authenticated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AuthorizInterface::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AuthorizInterface::registrationCompleted)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AuthorizInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_AuthorizInterface.data,
    qt_meta_data_AuthorizInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AuthorizInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuthorizInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuthorizInterface.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AuthorizInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AuthorizInterface::authenticated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AuthorizInterface::registrationCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

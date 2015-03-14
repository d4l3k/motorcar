/****************************************************************************
** Meta object code from reading C++ file 'qtwaylandmotorcarcompositor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/compositor/qt/qtwaylandmotorcarcompositor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtwaylandmotorcarcompositor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor_t {
    QByteArrayData data[11];
    char stringdata[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor_t qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor = {
    {
QT_MOC_LITERAL(0, 0, 39),
QT_MOC_LITERAL(1, 40, 16),
QT_MOC_LITERAL(2, 57, 0),
QT_MOC_LITERAL(3, 58, 6),
QT_MOC_LITERAL(4, 65, 13),
QT_MOC_LITERAL(5, 79, 15),
QT_MOC_LITERAL(6, 95, 14),
QT_MOC_LITERAL(7, 110, 17),
QT_MOC_LITERAL(8, 128, 6),
QT_MOC_LITERAL(9, 135, 10),
QT_MOC_LITERAL(10, 146, 12)
    },
    "qtmotorcar::QtWaylandMotorcarCompositor\0"
    "surfaceDestroyed\0\0object\0surfaceMapped\0"
    "surfaceUnmapped\0surfaceDamaged\0"
    "surfacePosChanged\0render\0sendExpose\0"
    "updateCursor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qtmotorcar__QtWaylandMotorcarCompositor[] = {

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
       4,    0,   57,    2, 0x08 /* Private */,
       5,    0,   58,    2, 0x08 /* Private */,
       6,    0,   59,    2, 0x08 /* Private */,
       7,    0,   60,    2, 0x08 /* Private */,
       8,    0,   61,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QObjectStar,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void qtmotorcar::QtWaylandMotorcarCompositor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtWaylandMotorcarCompositor *_t = static_cast<QtWaylandMotorcarCompositor *>(_o);
        switch (_id) {
        case 0: _t->surfaceDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->surfaceMapped(); break;
        case 2: _t->surfaceUnmapped(); break;
        case 3: _t->surfaceDamaged(); break;
        case 4: _t->surfacePosChanged(); break;
        case 5: _t->render(); break;
        case 6: _t->sendExpose(); break;
        case 7: _t->updateCursor(); break;
        default: ;
        }
    }
}

const QMetaObject qtmotorcar::QtWaylandMotorcarCompositor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor.data,
      qt_meta_data_qtmotorcar__QtWaylandMotorcarCompositor,  qt_static_metacall, 0, 0}
};


const QMetaObject *qtmotorcar::QtWaylandMotorcarCompositor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qtmotorcar::QtWaylandMotorcarCompositor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qtmotorcar__QtWaylandMotorcarCompositor.stringdata))
        return static_cast<void*>(const_cast< QtWaylandMotorcarCompositor*>(this));
    if (!strcmp(_clname, "QWaylandCompositor"))
        return static_cast< QWaylandCompositor*>(const_cast< QtWaylandMotorcarCompositor*>(this));
    if (!strcmp(_clname, "motorcar::Compositor"))
        return static_cast< motorcar::Compositor*>(const_cast< QtWaylandMotorcarCompositor*>(this));
    return QObject::qt_metacast(_clname);
}

int qtmotorcar::QtWaylandMotorcarCompositor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE

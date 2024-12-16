/****************************************************************************
** Meta object code from reading C++ file 'FenetrePrincipale.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../FenetrePrincipale.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FenetrePrincipale.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSFenetrePrincipaleENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSFenetrePrincipaleENDCLASS = QtMocHelpers::stringData(
    "FenetrePrincipale",
    "commencerPartieContreIA",
    "",
    "commencerPartieDeuxJoueurs",
    "ouvrirFileDialog",
    "chargerPartieSauvegarde",
    "afficherPlateauDebut",
    "afficherPiochesDebut",
    "dessinerPanneauJoueur",
    "x",
    "y",
    "largeur",
    "hauteur",
    "couleur",
    "opacite",
    "dessinerPionsPiochesJoueurs",
    "onMiseAJourPlateau",
    "onPartieTerminee",
    "message",
    "onPionClique",
    "VuePion*",
    "pion"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSFenetrePrincipaleENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    0,   83,    2, 0x08,    4 /* Private */,
       6,    0,   84,    2, 0x08,    5 /* Private */,
       7,    0,   85,    2, 0x08,    6 /* Private */,
       8,    6,   86,    2, 0x08,    7 /* Private */,
      15,    0,   99,    2, 0x08,   14 /* Private */,
      16,    0,  100,    2, 0x08,   15 /* Private */,
      17,    1,  101,    2, 0x08,   16 /* Private */,
      19,    1,  104,    2, 0x08,   18 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QColor, QMetaType::Double,    9,   10,   11,   12,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

Q_CONSTINIT const QMetaObject FenetrePrincipale::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSFenetrePrincipaleENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSFenetrePrincipaleENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSFenetrePrincipaleENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FenetrePrincipale, std::true_type>,
        // method 'commencerPartieContreIA'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'commencerPartieDeuxJoueurs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'ouvrirFileDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chargerPartieSauvegarde'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherPlateauDebut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherPiochesDebut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'dessinerPanneauJoueur'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QColor, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'dessinerPionsPiochesJoueurs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onMiseAJourPlateau'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPartieTerminee'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onPionClique'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<VuePion *, std::false_type>
    >,
    nullptr
} };

void FenetrePrincipale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FenetrePrincipale *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->commencerPartieContreIA(); break;
        case 1: _t->commencerPartieDeuxJoueurs(); break;
        case 2: _t->ouvrirFileDialog(); break;
        case 3: _t->chargerPartieSauvegarde(); break;
        case 4: _t->afficherPlateauDebut(); break;
        case 5: _t->afficherPiochesDebut(); break;
        case 6: _t->dessinerPanneauJoueur((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6]))); break;
        case 7: _t->dessinerPionsPiochesJoueurs(); break;
        case 8: _t->onMiseAJourPlateau(); break;
        case 9: _t->onPartieTerminee((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onPionClique((*reinterpret_cast< std::add_pointer_t<VuePion*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< VuePion* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *FenetrePrincipale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FenetrePrincipale::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSFenetrePrincipaleENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FenetrePrincipale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP

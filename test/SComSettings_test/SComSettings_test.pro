QT += core
QT += testlib
QT += widgets
QT += sql xml
QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..
DEFINES += QS_HAS_JSON
DEFINES += QS_HAS_XML
DEFINES += S_TEST

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../tdebugstuff.cpp \
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssettingsbase.cpp \
    ../../models/scomsettings.cpp \
    ../../widgets/scheckcombobox.cpp \
    ../../widgets/scombobox.cpp \
    ../../widgets/slineedit.cpp \
    ../../squerylog.cpp \
    global.cpp \
    slocalsettings.cpp \
    tclasstest.cpp

HEADERS += \
    ../tdebugstuff.h \
    ../../3rdparty/QSerializer/src/qserializer.h \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/slogrecordmodel.h \
    ../../models/ssinglerowmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssettingsbase.h \
    ../../models/scomsettingstypes.h \
    ../../models/scomsettings.h \
    ../../widgets/scheckcombobox.h \
    ../../widgets/scombobox.h \
    ../../widgets/slineedit.h \
    ../../squerylog.h \
    global.h \
    slocalsettings.h

QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

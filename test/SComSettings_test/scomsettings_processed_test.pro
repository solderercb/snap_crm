QT += core
QT += testlib
QT += widgets
QT += sql xml
QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += PREPROCESSING_EVALUATION_BUILD
DEFINES += _PWD=$${PWD}/..
DEFINES += QS_HAS_JSON
DEFINES += QS_HAS_XML
DEFINES += S_TEST

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    ../../widgets/scheckcombobox.cpp \
    ../../widgets/scombobox.cpp \
    ../../widgets/slineedit.cpp \
    ../../squerylog.cpp \
    global.cpp \
    slocalsettings.cpp \
    ../tdebugstuff.cpp \
    scomsettings_processed.cpp \
    tclasstest.cpp

HEADERS += \
    ../../3rdparty/QSerializer/src/qserializer.h \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    ../../models/scomsettingstypes.h \
    ../../widgets/scheckcombobox.h \
    ../../widgets/scombobox.h \
    ../../widgets/slineedit.h \
    ../../squerylog.h \
    global.h \
    slocalsettings.h \
    ../tdebugstuff.h \
    scomsettings_processed.h

QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

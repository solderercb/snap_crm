QT += core testlib sql
QT += widgets
#QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..

INCLUDEPATH += $${PWD}/include

SOURCES += \
    ../tdebugstuff.cpp \
    ../slocalsettings.cpp \
    global.cpp \
    ../../squerylog.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    srepair.cpp \
    tclasstest.cpp

HEADERS += \
    ../tdebugstuff.h \
    ../slocalsettings.h \
    global.h \
    ../../squerylog.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/slogrecordmodel.h \
    srepair.h

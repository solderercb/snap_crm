QT += core
QT += testlib
QT += widgets
QT += sql
QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    global.cpp \
    ../tdebugstuff.cpp \
    ../slocalsettings.cpp \
    ../../squerylog.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/sstoreitemmodel.cpp \
    sclientmodel.cpp \
    tclasstest.cpp

HEADERS += \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    com_sql_queries.h \
    global.h \
    ../tdebugstuff.h \
    ../slocalsettings.h \
    ../../squerylog.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/sstoreitemmodel.h \
    sclientmodel.h

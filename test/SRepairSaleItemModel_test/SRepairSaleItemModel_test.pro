QT += core testlib widgets sql
#QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../threadconnections.cpp \
    ../../squerylog.cpp \
    ../../models/sabstractitemmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/srepairsaleitemmodel.cpp \
    ../tdebugstuff.cpp \
    ../slocalsettings.cpp \
    global.cpp \
    tclasstest.cpp

HEADERS += \
    ../../threadconnections.h \
    ../../squerylog.h \
    ../../models/sabstractitemmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/srepairsaleitemmodel.h \
    ssqlquerymodel.h \
    sstandarditemmodel.h \
    com_sql_queries.h \
    ssaletablemodel.h \
    sstoreitemmodel.h \
    ../tdebugstuff.h \
    ../slocalsettings.h \
    global.h

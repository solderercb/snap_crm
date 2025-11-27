QT += core testlib widgets sql
#QT -= gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../squerylog.cpp \
    ../../models/sabstractitemmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/sstoresaleitemmodel.cpp \
    applog.cpp \
    ../tdebugstuff.cpp \
    ../slocalsettings.cpp \
    global.cpp \
    tclasstest.cpp

HEADERS += \
    ../../squerylog.h \
    ../../models/sabstractitemmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/sstoresaleitemmodel.h \
    applog.h \
    ssqlquerymodel.h \
    com_sql_queries.h \
    ssaletablemodel.h \
    sstoreitemmodel.h \
    ../tdebugstuff.h \
    ../slocalsettings.h \
    ../shortlivednotification.h \
    global.h

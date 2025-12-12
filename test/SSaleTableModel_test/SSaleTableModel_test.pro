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
DEFINES += S_TEST

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../models/stablemodelscommonmethods.cpp \
    ../slocalsettings.cpp \
    ../tdebugstuff.cpp \
    ../shortlivednotification.cpp \
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    ../../models/ssaletablemodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/sstoresaleitemmodel.cpp \
    ../../models/srepairsaleitemmodel.cpp \
    ../../models/sworkmodel.cpp \
    ../../threadconnections.cpp \
    ../../squerylog.cpp \
    global.cpp \
    applog.cpp \
    tclasstest.cpp

HEADERS += \
    ../../models/stablemodelscommonmethods.h \
    ../slocalsettings.h \
    ../tdebugstuff.h \
    ../shortlivednotification.h \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    ../../models/ssaletablemodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/slogrecordmodel.h \
    ../../models/ssinglerowmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/sstoresaleitemmodel.h \
    ../../models/srepairsaleitemmodel.h \
    ../../models/sworkmodel.h \
    ../../threadconnections.h \
    ../../squerylog.h \
    ../../com_sql_queries.h \
    ssqlquerymodel.h \
    sstoreitemmodel.h \
    scartridgecardmodel.h \
    global.h \
    applog.h

#QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

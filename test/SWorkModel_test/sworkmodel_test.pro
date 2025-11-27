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
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/sworkmodel.cpp \
    ../../squerylog.cpp \
    ../slocalsettings.cpp \
    ../tdebugstuff.cpp \
    global.cpp \
    applog.cpp \
    tclasstest.cpp

HEADERS += \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/sworkmodel.h \
    ../../squerylog.h \
    ../slocalsettings.h \
    ../tdebugstuff.h \
    ssqlquerymodel.h \
    com_sql_queries.h \
    global.h \
    applog.h \

QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

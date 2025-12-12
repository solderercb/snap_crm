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
    ../../models/sfieldmodel.cpp \
    ../../threadconnections.cpp \
    ../../squerylog.cpp \
    ../../widgets/scombobox.cpp \
    ../../widgets/sdateedit.cpp \
    ../../widgets/slineedit.cpp \
    global.cpp \
    ../tdebugstuff.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/sfieldsmodel.cpp \
    ../../models/sfieldvaluemodel.cpp \
    applog.cpp \
    ../slocalsettings.cpp \
    tclasstest.cpp

HEADERS += \
    ../../models/sfieldmodel.h \
    ../../threadconnections.h \
    ../../squerylog.h \
    ../../widgets/scombobox.h \
    ../../widgets/sdateedit.h \
    ../../widgets/slineedit.h \
    com_sql_queries.h \
    global.h \
    ../tdebugstuff.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/sfieldsmodel.h \
    ../../models/sfieldvaluemodel.h \
    applog.h \
    ../slocalsettings.h \
    ssqlquerymodel.h \

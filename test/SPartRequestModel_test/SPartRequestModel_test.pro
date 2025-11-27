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
    ../../amountToWords/amountToWords.cpp \
    ../../models/scartridgerepairmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/ssqlfetchingmodel.cpp \
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    ../../models/seditablebasemodel.cpp \
    ../../models/stablemodelscommonmethods.cpp \
    ../../modules/purchasemanager/srequest.cpp \
    ../../squerylog.cpp \
    ../slocalsettings.cpp \
    ../tdebugstuff.cpp \
    global.cpp \
    applog.cpp \
    snotification.cpp \
    tclasstest.cpp

HEADERS += \
    ../../amountToWords/amountToWords.h \
    ../../models/scartridgerepairmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/slogrecordmodel.h \
    ../../models/ssqlfetchingmodel.h \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    ../../models/seditablebasemodel.h \
    ../../models/stablemodelscommonmethods.h \
    ../../modules/purchasemanager/srequest.h \
    ../../squerylog.h \
    ../slocalsettings.h \
    ../tdebugstuff.h \
    scomsettings.h \
    snotification.h \
    ssqlquerymodel.h \
    com_sql_queries.h \
    global.h \
    applog.h \

QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

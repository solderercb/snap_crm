QT += core
QT += testlib
QT += widgets
QT += sql
QT += gui
QT += xml

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _PWD=$${PWD}/..

INCLUDEPATH += $${PWD}/include

SOURCES +=  \
    ../../clients4test.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../tabcommon.cpp \
    ../../tabsale.cpp \
    ../../squerylog.cpp \
    ../tdebugstuff.cpp \
    ../slocalsettings.cpp \
    global.cpp \
    applog.cpp \
    tclasstest.cpp

HEADERS += \
    ../../clients4test.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/ssinglerowmodel.h \
    ../../tabcommon.h \
    ../../tabsale.h \
    ../../squerylog.h \
    ../tdebugstuff.h \
    ../slocalsettings.h \
    com_sql_queries.h \
    global.h \
    applog.h \
    sdocumentmodel.h \
    ssaletablemodel.h \
    scashregistermodel.h \
    mainwindow.h
    
FORMS += \
    ../../tabsale.ui

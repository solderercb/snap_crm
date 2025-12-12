QT += core
QT += testlib
QT += widgets
QT += sql
QT += network
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
    ../../models/sabstractitemmodel.cpp \
    ../../models/sstandarditemmodel.cpp \
    ../../models/ssinglerowmodelbase.cpp \
    ../../models/slogrecordmodel.cpp \
    ../../models/ssinglerowmodel.cpp \
    ../../models/ssinglerowjmodel.cpp \
    ../../models/scommentmodel.cpp \
    ../../models/scommentsmodel.cpp \
    ../../models/suseractivitymodel.cpp \
    ../../widgets/swidgetcommonmethods.cpp \
    ../../widgets/swidget.cpp\
    ../../widgets/scomtextedit.cpp \
    ../../widgets/splaintextedit.cpp \
    ../../widgets/stextedit.cpp \
    ../../widgets/scomments.cpp \
    ../../threadconnections.cpp \
    ../../squerylog.cpp \
    global.cpp \
    applog.cpp \
    tclasstest.cpp

HEADERS += \
    ../../models/stablemodelscommonmethods.h \
    ../slocalsettings.h \
    ../tdebugstuff.h \
    ../../models/sabstractitemmodel.h \
    ../../models/sstandarditemmodel.h \
    ../../models/ssinglerowmodelbase.h \
    ../../models/slogrecordmodel.h \
    ../../models/ssinglerowmodel.h \
    ../../models/ssinglerowjmodel.h \
    ../../models/scommentmodel.h \
    ../../models/scommentsmodel.h \
    ../../models/suseractivitymodel.h \
    ../../widgets/swidgetcommonmethods.h \
    ../../widgets/swidget.h \
    ../../widgets/scomtextedit.h \
    ../../widgets/splaintextedit.h \
    ../../widgets/stextedit.h \
    ../../widgets/scomments.h \
    ../../threadconnections.h \
    ../../squerylog.h \
    com_sql_queries.h \
    global.h \
    applog.h \
    tabcommon.h \
    mainwindow.h \
    ssqlquerymodel.h

FORMS += \
    ../../widgets/scomments.ui

#QMAKE_POST_LINK += chcp 65001 $$escape_expand(\\n\\t)

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T01:19:21
#
#-------------------------------------------------

QT	+= core gui
QT	+= sql
QT      += core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    SSetComboBox/ssetcombobox.cpp \
    bottoolbarwidget.cpp \
				mainwindow.cpp \
    loginwindow.cpp \
    mylineedit.cpp \
    repairstablemodel.cpp \
    tabclients.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    windowsdispatcher.cpp

HEADERS  += mainwindow.h \
    SSetComboBox/ssetcombobox.h \
    bottoolbarwidget.h \
    com_sql_queries.h \
    loginwindow.h \
    mylineedit.h \
    repairstablemodel.h \
    tabclients.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    windowsdispatcher.h

FORMS    += mainwindow.ui \
    bottoolbarwidget.ui \
    loginwindow.ui \
    tabclients.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui

DISTFILES +=

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
    bottoolbarwidget.cpp \
    dbconnection.cpp \
				mainwindow.cpp \
    loginwindow.cpp \
    my_widget.cpp \
    mylineedit.cpp \
    repairstablemodel.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp

HEADERS  += mainwindow.h \
    bottoolbarwidget.h \
    dbconnection.h \
    loginwindow.h \
    my_widget.h \
    mylineedit.h \
    repairstablemodel.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h

FORMS    += mainwindow.ui \
    bottoolbarwidget.ui \
    loginwindow.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui

DISTFILES +=

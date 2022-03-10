#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T01:19:21
#
#-------------------------------------------------

QT	+= core gui
QT	+= sql
QT      += core5compat
QT      += printsupport
QT      += xml
QT      += qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snap
TEMPLATE = app

RC_FILE = resources.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp\
    SSetComboBox/ssetcombobox.cpp \
    bottoolbarwidget.cpp \
    chooseofficewindow.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    mylineedit.cpp \
    repairstablemodel.cpp \
    tabclients.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    stable.h \
    mainwindow.h \
    SSetComboBox/ssetcombobox.h \
    appver.h \
    bottoolbarwidget.h \
    chooseofficewindow.h \
    com_sql_queries.h \
    loginwindow.h \
    mylineedit.h \
    repairstablemodel.h \
    tabclients.h \
    tabcommon.h \
    tabprintdialog.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    windowsdispatcher.h

FORMS    += mainwindow.ui \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    loginwindow.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui

LIB_DIR = $$PWD/lib6

CONFIG(release, debug|release): LIBS += -L$$LIB_DIR -llimereport -lKernel32
else:CONFIG(debug, debug|release): LIBS += -L$$LIB_DIR -llimereportd -lKernel32

INCLUDEPATH += $$LIB_DIR/include
DEPENDPATH += $$LIB_DIR/include

BIN_DIR ~= s,/,\\,g
LIB_DIR ~= s,/,\\,g
QMAKE_PRE_LINK += $$QMAKE_COPY \"$${LIB_DIR}\\*.dll\" \"$${BIN_DIR}\"  $$escape_expand(\\n\\t)

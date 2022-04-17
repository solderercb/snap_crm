#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T01:19:21
#
#-------------------------------------------------

QT	+= core gui
QT      += widgets
QT	+= sql
QT      += core5compat
QT      += printsupport
QT      += xml
QT      += qml

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
    amountToWords.cpp \
    bottoolbarwidget.cpp \
    chooseofficewindow.cpp \
    clients4test.cpp \
    getOutDialog.cpp \
    global.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    mcombobox.cpp \
    models/repairtablefiltermenu.cpp \
    models/saletable/saletableitemdelegates.cpp \
    models/saletable/saletablemodel.cpp \
    querylog.cpp \
    repairstablemodel.cpp \
    shortlivednotification.cpp \
    ssqlquerymodel.cpp \
    sstandarditemmodel.cpp \
    tabclients.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    tabsale.cpp \
    widgets/sgroupboxeventfilter.cpp \
    widgets/slineedit.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    amountToWords.h \
    clients4test.h \
    getOutDialog.h \
    global.h \
    mcombobox.h \
    models/repairtablefiltermenu.h \
    models/saletable/saletableitemdelegates.h \
    models/saletable/saletablemodel.h \
    querylog.h \
    shortlivednotification.h \
    ssqlquerymodel.h \
    sstandarditemmodel.h \
    stable.h \
    mainwindow.h \
    SSetComboBox/ssetcombobox.h \
    appver.h \
    bottoolbarwidget.h \
    chooseofficewindow.h \
    com_sql_queries.h \
    loginwindow.h \
    repairstablemodel.h \
    tabclients.h \
    tabcommon.h \
    tabprintdialog.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    tabsale.h \
    widgets/sgroupboxeventfilter.h \
    widgets/slineedit.h \
    windowsdispatcher.h

FORMS    += mainwindow.ui \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    getOutDialog.ui \
    loginwindow.ui \
    models/repairtablefiltermenu.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui

LIB_DIR = $$PWD/lib6
INCLUDEPATH += $$LIB_DIR/include
DEPENDPATH += $$LIB_DIR/include
PROGRAM_FILES_DIR = C:\Program Files\SNAP CRM

CONFIG(release, debug|release) {
    BUILD_TYPE = release
    LIBS += -L$$LIB_DIR -llimereport -lKernel32
}else{
    BUILD_TYPE = debug
    LIBS += -L$$LIB_DIR -llimereportd -lKernel32
}

BIN_DIR ~= s,/,\\,g
LIB_DIR ~= s,/,\\,g
QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
QMAKE_PRE_LINK += $$QMAKE_COPY \"$${LIB_DIR}\\*.dll\" \"$${BIN_DIR}\"  $$escape_expand(\\n\\t)
CONFIG(release, debug|release): QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWD}\\$${BUILD_TYPE}\\$${TARGET}.exe\" \"$${PROGRAM_FILES_DIR}\\$${TARGET}.exe\" $$escape_expand(\\n\\t)

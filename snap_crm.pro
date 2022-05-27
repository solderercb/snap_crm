#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT	+= core gui
QT      += widgets
QT	+= sql
QT      += printsupport
QT      += xml
QT      += qml
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = snap
TEMPLATE = app

RC_FILE = resources.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += LIMEREPORT_IMPORTS

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
    models/repairtablefiltermenu.cpp \
    models/saletable/saletableitemdelegates.cpp \
    models/saletable/saletablemodel.cpp \
    models/ssloptionsdialog.cpp \
    models/tabsalesettingsmenu.cpp \
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
    widgets/scombobox.cpp \
    widgets/sgroupboxeventfilter.cpp \
    widgets/slineedit.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    amountToWords.h \
    clients4test.h \
    getOutDialog.h \
    global.h \
    models/repairtablefiltermenu.h \
    models/saletable/saletableitemdelegates.h \
    models/saletable/saletablemodel.h \
    models/ssloptionsdialog.h \
    models/tabsalesettingsmenu.h \
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
    widgets/scombobox.h \
    widgets/sgroupboxeventfilter.h \
    widgets/slineedit.h \
    windowsdispatcher.h

FORMS    += mainwindow.ui \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    getOutDialog.ui \
    loginwindow.ui \
    models/repairtablefiltermenu.ui \
    models/ssloptionsdialog.ui \
    models/tabsalesettingsmenu.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui

RESOURCES += styles/styles.qrc

CONFIG(release, debug|release) {
    BUILD_TYPE = release
}else{
    BUILD_TYPE = debug
}

MOC_DIR        = $${OUT_PWD}/$${BUILD_TYPE}/moc
OBJECTS_DIR    = $${OUT_PWD}/$${BUILD_TYPE}/obj
UI_DIR         = $${OUT_PWD}/ui
UI_HEADERS_DIR = $${OUT_PWD}/ui
UI_SOURCES_DIR = $${OUT_PWD}/ui
RCC_DIR        = $${OUT_PWD}/$${BUILD_TYPE}/rcc

equals(QT_MAJOR_VERSION, 6){
    LIB_DIR = $$PWD/lib6
}
else {
    LIB_DIR = $$PWD/lib5
}
INCLUDEPATH += $$LIB_DIR/include
DEPENDPATH += $$LIB_DIR/include
PROGRAM_FILES_DIR = C:\Program Files\SNAP CRM

CONFIG(release, debug|release) {
    LIBS += -L$$LIB_DIR -llimereport
}else{
    LIBS += -L$$LIB_DIR -llimereportd
}
LIBS += -lwinspool -lKernel32

BIN_DIR ~= s,/,\\,g
LIB_DIR ~= s,/,\\,g
QMAKE_PRE_LINK  += chcp 1250 >nul 2>&1 $$escape_expand(\\n\\t)
QMAKE_PRE_LINK  += rcc -binary $${PWD}\\schema-updates\\schema-updates.qrc -o $${OUT_PWD}\\$${BUILD_TYPE}\\schema-updates.rcc $$escape_expand(\\n\\t)
QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
#QMAKE_PRE_LINK += $$QMAKE_COPY \"$${LIB_DIR}\\*.dll\" \"$${BIN_DIR}\"  $$escape_expand(\\n\\t)
equals(QT_MAJOR_VERSION, 6){
    CONFIG(release, debug|release): QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWD}\\$${OUT_PWD}\\$${BUILD_TYPE}\\$${TARGET}.exe\" \"$${PROGRAM_FILES_DIR}\\$${TARGET}.exe\" $$escape_expand(\\n\\t)
}

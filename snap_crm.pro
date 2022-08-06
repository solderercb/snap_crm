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

SOURCES += \
    main.cpp\
    amountToWords.cpp \
    bottoolbarwidget.cpp \
    chooseofficewindow.cpp \
    clients4test.cpp \
    global.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    models/repairstablemodel.cpp \
    models/repairtablefiltermenu.cpp \
    models/saletablemodel.cpp \
    models/ssloptionsdialog.cpp \
    models/ssqlquerymodel.cpp \
    models/sstandarditemmodel.cpp \
    models/tabsalesettingsmenu.cpp \
    models/scashregistermodel.cpp \
    models/sclientmodel.cpp \
    models/scomrecord.cpp \
    models/sdatabaserecord.cpp \
    models/sdocumentmodel.cpp \
    models/slogrecordmodel.cpp \
    models/sphonemodel.cpp \
    models/ssaleitemmodel.cpp \
    querylog.cpp \
    tabclients.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    tabsale.cpp \
    widgets/getoutdialog.cpp \
    widgets/saletableitemdelegates.cpp \
    widgets/scombobox.cpp \
    widgets/sgroupboxeventfilter.cpp \
    widgets/shortlivednotification.cpp \
    widgets/slineedit.cpp \
    widgets/ssetcombobox.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    amountToWords.h \
    appver.h \
    bottoolbarwidget.h \
    chooseofficewindow.h \
    clients4test.h \
    com_sql_queries.h \
    global.h \
    loginwindow.h \
    mainwindow.h \
    models/repairstablemodel.h \
    models/repairtablefiltermenu.h \
    models/saletablemodel.h \
    models/ssloptionsdialog.h \
    models/ssqlquerymodel.h \
    models/sstandarditemmodel.h \
    models/tabsalesettingsmenu.h \
    models/scashregistermodel.h \
    models/sclientmodel.h \
    models/scomrecord.h \
    models/sdatabaserecord.h \
    models/sdocumentmodel.h \
    models/slogrecordmodel.h \
    models/sphonemodel.h \
    models/ssaleitemmodel.h \
    querylog.h \
    stable.h \
    tabclients.h \
    tabcommon.h \
    tabprintdialog.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    tabsale.h \
    widgets/getoutdialog.h \
    widgets/saletableitemdelegates.h \
    widgets/scombobox.h \
    widgets/sgroupboxeventfilter.h \
    widgets/shortlivednotification.h \
    widgets/slineedit.h \
    widgets/ssetcombobox.h \
    windowsdispatcher.h

FORMS += \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    models/repairtablefiltermenu.ui \
    models/ssloptionsdialog.ui \
    models/tabsalesettingsmenu.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui \
    widgets/getoutdialog.ui

RESOURCES += styles/styles.qrc

QMAKE_MKLINK = mklink /H

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
equals(QT_MAJOR_VERSION, 5){
    CONFIG(release, debug|release) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${OUT_PWD}\\$${BUILD_TYPE}\\$${TARGET}.exe\" \"$${PROGRAM_FILES_DIR}\\$${TARGET}.exe\" $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${OUT_PWD}\\$${BUILD_TYPE}\\schema-updates.rcc\" \"$${PROGRAM_FILES_DIR}\\\" $$escape_expand(\\n\\t)
    }
}

# Qt Creator v5/v6 при сохранении разрушает жёсткую связь файлов. Для того, чтобы в отдельном субрепозитории (субпроекте,
# предназначенном для разработки отдельного класса и позволяющем чуть-чуть сэкономить на времени компиляции) при изменении
# исходного кода в основном проекте также появились изменения, после линковки пересоздаём жесткую ссылку. У этого метода,
# есть, конечно, недостатки; например, если файл был изменён в соновном проекте, сохранён, а компиляция не производилась, то
# в случае внесения изменений в файл в субрепозитории и запуска компиляции, изменения в файле в основном репозитории могут
# быть утрачены.

SHARED_FILE_SRC_PATH = $${PWD}\\widgets
SHARED_FILE_TRG_PATH = $${PWD}\\standalone-dev\\SSetComboBox
SHARED_FILES = ssetcombobox.cpp
SHARED_FILES += ssetcombobox.h
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}
SHARED_FILE_TRG_PATH = $${PWD}\\standalone-dev\\saletable
SHARED_FILES = amountToWords.cpp
SHARED_FILES += amountToWords.h
SHARED_FILES += clients4test.cpp
SHARED_FILES += clients4test.h
SHARED_FILES += com_sql_queries.h
SHARED_FILES += global.cpp
SHARED_FILES += global.h
SHARED_FILES += querylog.cpp
SHARED_FILES += querylog.h
SHARED_FILES += tabcommon.cpp
SHARED_FILES += tabcommon.h
SHARED_FILES += tabsale.cpp
SHARED_FILES += tabsale.h
SHARED_FILES += tabsale.ui
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}\\widgets
SHARED_FILE_TRG_PATH = $${PWD}\\standalone-dev\\saletable\\widgets
SHARED_FILES = saletableitemdelegates.cpp
SHARED_FILES += saletableitemdelegates.h
SHARED_FILES += shortlivednotification.cpp
SHARED_FILES += shortlivednotification.h
SHARED_FILES += tabsalesettingsmenu.cpp
SHARED_FILES += tabsalesettingsmenu.h
SHARED_FILES += tabsalesettingsmenu.ui
SHARED_FILES += sgroupboxeventfilter.cpp
SHARED_FILES += sgroupboxeventfilter.h
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}\\models
SHARED_FILE_TRG_PATH = $${PWD}\\standalone-dev\\saletable\\models
SHARED_FILES = saletablemodel.cpp
SHARED_FILES += saletablemodel.h
SHARED_FILES += ssqlquerymodel.cpp
SHARED_FILES += ssqlquerymodel.h
SHARED_FILES += sstandarditemmodel.cpp
SHARED_FILES += sstandarditemmodel.h
SHARED_FILES += scashregistermodel.cpp
SHARED_FILES += scashregistermodel.h
SHARED_FILES += sclientmodel.cpp
SHARED_FILES += sclientmodel.h
SHARED_FILES += scomrecord.cpp
SHARED_FILES += scomrecord.h
SHARED_FILES += scomrecordmodel.h
SHARED_FILES += sdatabaserecord.cpp
SHARED_FILES += sdatabaserecord.h
SHARED_FILES += sdocumentmodel.cpp
SHARED_FILES += sdocumentmodel.h
SHARED_FILES += slogrecordmodel.cpp
SHARED_FILES += slogrecordmodel.h
SHARED_FILES += sphonemodel.cpp
SHARED_FILES += sphonemodel.h
SHARED_FILES += ssaleitemmodel.cpp
SHARED_FILES += ssaleitemmodel.h
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}
SHARED_FILE_TRG_PATH = $${PWD}\\LimeReport\\demo_snap
SHARED_FILES = clients4test.cpp
SHARED_FILES += clients4test.h
SHARED_FILES += com_sql_queries.h
SHARED_FILES += global.cpp
SHARED_FILES += global.h
SHARED_FILES += tabcommon.cpp
SHARED_FILES += tabcommon.h
SHARED_FILES += tabprintdialog.cpp
SHARED_FILES += tabprintdialog.h
SHARED_FILES += tabprintdialog.ui
SHARED_FILES += tabreportdesigner.cpp
SHARED_FILES += tabreportdesigner.h
SHARED_FILES += tabreportdesigner.ui
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}\\models
SHARED_FILE_TRG_PATH = $${PWD}\\LimeReport\\demo_snap\\models
SHARED_FILES = ssqlquerymodel.cpp
SHARED_FILES += ssqlquerymodel.h
SHARED_FILES += sstandarditemmodel.cpp
SHARED_FILES += sstandarditemmodel.h
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

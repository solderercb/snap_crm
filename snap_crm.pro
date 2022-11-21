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
    applog.cpp \
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
    models/sabstractitemmodel.cpp \
    models/scommentmodel.cpp \
    models/scommentsmodel.cpp \
    models/sdevicemakermodel.cpp \
    models/sdevicemakersmodel.cpp \
    models/sdevicemodel.cpp \
    models/sdevicesmodel.cpp \
    models/sdevmdlmodel.cpp \
    models/sdevmdlsmodel.cpp \
    models/sfieldsmodel.cpp \
    models/sfieldvaluemodel.cpp \
    models/sinvoicemodel.cpp \
    models/spaymenttypemodel.cpp \
    models/spaymenttypesmodel.cpp \
    models/srepairmodel.cpp \
    models/srepairstatuslog.cpp \
    models/ssaletablemodel.cpp \
    models/ssloptionsdialog.cpp \
    models/ssortfilterproxymodel.cpp \
    models/ssqlquerymodel.cpp \
    models/sstandarditemmodel.cpp \
    models/suseractivitymodel.cpp \
    models/sworkshopissuedmodel.cpp \
    models/tabsalesettingsmenu.cpp \
    models/scashregistermodel.cpp \
    models/sclientmodel.cpp \
    models/scomrecord.cpp \
    models/sdatabaserecord.cpp \
    models/sdocumentmodel.cpp \
    models/slogrecordmodel.cpp \
    models/sphonemodel.cpp \
    models/sphonesmodel.cpp \
    models/ssaleitemmodel.cpp \
    squerylog.cpp \
    tabcashmoveexch.cpp \
    tabcashoperation.cpp \
    tabclient.cpp \
    tabclients.cpp \
    tabclientsummary.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    tabsale.cpp \
    widgets/getoutdialog.cpp \
    widgets/saletableitemdelegates.cpp \
    widgets/sclientmatch.cpp \
    widgets/scombobox.cpp \
    widgets/sdevicematch.cpp \
    widgets/sgroupboxeventfilter.cpp \
    widgets/shortlivednotification.cpp \
    widgets/slineedit.cpp \
    widgets/sphone.cpp \
    widgets/sphones.cpp \
    widgets/ssetcombobox.cpp \
    widgets/stextedit.cpp \
    widgets/stoolboxswitchpanel.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    amountToWords.h \
    applog.h \
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
    models/sabstractitemmodel.h \
    models/scommentmodel.h \
    models/scommentsmodel.h \
    models/sdevicemakermodel.h \
    models/sdevicemakersmodel.h \
    models/sdevicemodel.h \
    models/sdevicesmodel.h \
    models/sdevmdlmodel.h \
    models/sdevmdlsmodel.h \
    models/sfieldsmodel.h \
    models/sfieldvaluemodel.h \
    models/sinvoicemodel.h \
    models/spaymenttypemodel.h \
    models/spaymenttypesmodel.h \
    models/srepairmodel.h \
    models/srepairstatuslog.h \
    models/ssaletablemodel.h \
    models/ssloptionsdialog.h \
    models/ssortfilterproxymodel.h \
    models/ssqlquerymodel.h \
    models/sstandarditemmodel.h \
    models/suseractivitymodel.h \
    models/sworkshopissuedmodel.h \
    models/tabsalesettingsmenu.h \
    models/scashregistermodel.h \
    models/sclientmodel.h \
    models/scomrecord.h \
    models/sdatabaserecord.h \
    models/sdocumentmodel.h \
    models/slogrecordmodel.h \
    models/sphonemodel.h \
    models/sphonesmodel.h \
    models/ssaleitemmodel.h \
    squerylog.h \
    stable.h \
    tabcashmoveexch.h \
    tabcashoperation.h \
    tabclient.h \
    tabclients.h \
    tabclientsummary.h \
    tabcommon.h \
    tabprintdialog.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    tabsale.h \
    widgets/getoutdialog.h \
    widgets/saletableitemdelegates.h \
    widgets/sclientmatch.h \
    widgets/scombobox.h \
    widgets/sdevicematch.h \
    widgets/sgroupboxeventfilter.h \
    widgets/shortlivednotification.h \
    widgets/slineedit.h \
    widgets/sphone.h \
    widgets/sphones.h \
    widgets/ssetcombobox.h \
    widgets/stextedit.h \
    widgets/stoolboxswitchpanel.h \
    windowsdispatcher.h

FORMS += \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    models/repairtablefiltermenu.ui \
    models/ssloptionsdialog.ui \
    models/tabsalesettingsmenu.ui \
    tabcashmoveexch.ui \
    tabcashoperation.ui \
    tabclient.ui \
    tabclients.ui \
    tabclientsummary.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui \
    widgets/getoutdialog.ui \
    widgets/sclientmatch.ui \
    widgets/sdevicematch.ui \
    widgets/sphone.ui \
    widgets/sphones.ui \
    widgets/stoolboxswitchpanel.ui

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
QMAKE_PRE_LINK  += rcc -binary $${PWD}\\schema-updates\\schema-updates.qrc -o $${OUT_PWD}\\$${BUILD_TYPE}\\schema-updates.rcc $$escape_expand(\\n\\t)
#QMAKE_PRE_LINK += $$QMAKE_COPY \"$${LIB_DIR}\\*.dll\" \"$${BIN_DIR}\"  $$escape_expand(\\n\\t)
equals(QT_MAJOR_VERSION, 5){
    CONFIG(release, debug|release) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${OUT_PWD}\\$${BUILD_TYPE}\\$${TARGET}.exe\" \"$${PROGRAM_FILES_DIR}\\$${TARGET}.exe\" $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${OUT_PWD}\\$${BUILD_TYPE}\\schema-updates.rcc\" \"$${PROGRAM_FILES_DIR}\\\" $$escape_expand(\\n\\t)
    }
}

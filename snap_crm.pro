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
CONFIG(debug, debug|release){
    QT      += testlib
}

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
DEFINES += QS_HAS_JSON
DEFINES += QS_HAS_XML

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
    models/salarytabmodels/sadditionalpaymentmodel.cpp \
    models/salarytabmodels/ssalarymodel.cpp \
    models/salarytabmodels/stablesalaryextramodel.cpp \
    models/salarytabmodels/stablesalaryitemsmodel.cpp \
    models/salarytabmodels/stablesalarypaymentsmodel.cpp \
    models/salarytabmodels/stablesalaryrepairpartsmodel.cpp \
    models/salarytabmodels/stablesalaryrepairworksmodel.cpp \
    models/salarytabmodels/stablesalaryrepairsmodel.cpp \
    models/salarytabmodels/stablesalaryreceptedissued.cpp \
    models/salarytabmodels/stablesalarysalepartsmodel.cpp \
    models/salarytabmodels/stablesalarysalesmodel.cpp \
    models/seditablebasemodel.cpp \
    models/stablemodelscommonmethods.cpp \
    models/stablerepairsmodel.cpp \
    models/repairtablefiltermenu.cpp \
    models/stablebasemodel.cpp \
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
    models/slocalsettings.cpp \
    models/spaymenttypemodel.cpp \
    models/spaymenttypesmodel.cpp \
    models/srepairmodel.cpp \
    models/srepairsaleitemmodel.cpp \
    models/srepairstatuslog.cpp \
    models/ssaletablemodel.cpp \
    models/ssloptionsdialog.cpp \
    models/ssortfilterproxymodel.cpp \
    models/ssqlquerymodel.cpp \
    models/sstandarditemmodel.cpp \
    models/sstoreitemmodel.cpp \
    models/sstoresaleitemmodel.cpp \
    models/suseractivitymodel.cpp \
    models/susermodel.cpp \
    models/sworkmodel.cpp \
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
    tabsalary.cpp \
    tabsettings.cpp \
    widgets/salarytabwidgets/spagesalaryextra.cpp \
    widgets/salarytabwidgets/spagesalaryitems.cpp \
    widgets/salarytabwidgets/spagesalarypayments.cpp \
    widgets/salarytabwidgets/spagesalarysales.cpp \
    widgets/salarytabwidgets/stableviewsalaryextra.cpp \
    widgets/salarytabwidgets/stableviewsalaryitems.cpp \
    widgets/salarytabwidgets/stableviewsalarypayments.cpp \
    widgets/salarytabwidgets/stableviewsalaryrepairparts.cpp \
    widgets/salarytabwidgets/stableviewsalaryrepairworks.cpp \
    widgets/salarytabwidgets/spagesalarybase.cpp \
    widgets/salarytabwidgets/spagesalaryreceptedissued.cpp \
    widgets/salarytabwidgets/spagesalaryrepairs.cpp \
    widgets/salarytabwidgets/spagesalarysummary.cpp \
    widgets/salarytabwidgets/stableviewsalaryreceptedissued.cpp \
    widgets/salarytabwidgets/stableviewsalaryrepairs.cpp \
    widgets/salarytabwidgets/stableviewsalarysaleparts.cpp \
    widgets/salarytabwidgets/stableviewsalarysales.cpp \
    widgets/sdoublespinbox.cpp \
    widgets/spageclientsummary.cpp \
    squerylog.cpp \
    tabcashmoveexch.cpp \
    tabcashoperation.cpp \
    tabclient.cpp \
    tabclients.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabrepair.cpp \
    tabrepairnew.cpp \
    tabrepairs.cpp \
    tabsale.cpp \
    widgets/spagedinterface.cpp \
    widgets/spagesettingsdoctempateeditor.cpp \
    widgets/sreportscommonfunctions.cpp \
    widgets/stableviewbase.cpp \
    widgets/stableviewbaseitemdelegates.cpp \
    widgets/stableviewclients.cpp \
    widgets/stableviewclientsitemdelegates.cpp \
    widgets/stableviewrepairsitemdelegates.cpp \
    widgets/stableviewrepairs.cpp \
    widgets/getoutdialog.cpp \
    widgets/saletableitemdelegates.cpp \
    widgets/sclientmatch.cpp \
    widgets/scombobox.cpp \
    widgets/scomments.cpp \
    widgets/scomtextedit.cpp \
    widgets/sdevicematch.cpp \
    widgets/sgroupboxeventfilter.cpp \
    widgets/shortlivednotification.cpp \
    widgets/slineedit.cpp \
    widgets/sphone.cpp \
    widgets/sphones.cpp \
    widgets/splaintextedit.cpp \
    widgets/ssetcombobox.cpp \
    widgets/stabpage.cpp \
    widgets/stextedit.cpp \
    widgets/stoolboxswitchpanel.cpp \
    widgets/swidget.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    3rdparty/qserializer.h \
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
    models/salarytabmodels/sadditionalpaymentmodel.h \
    models/salarytabmodels/ssalarymodel.h \
    models/salarytabmodels/stablesalaryextramodel.h \
    models/salarytabmodels/stablesalaryitemsmodel.h \
    models/salarytabmodels/stablesalarypaymentsmodel.h \
    models/salarytabmodels/stablesalaryrepairpartsmodel.h \
    models/salarytabmodels/stablesalaryrepairworksmodel.h \
    models/salarytabmodels/stablesalaryreceptedissued.h \
    models/salarytabmodels/stablesalaryrepairsmodel.h \
    models/salarytabmodels/stablesalarysalepartsmodel.h \
    models/salarytabmodels/stablesalarysalesmodel.h \
    models/seditablebasemodel.h \
    models/stablemodelscommonmethods.h \
    models/stablerepairsmodel.h \
    models/repairtablefiltermenu.h \
    models/stablebasemodel.h \
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
    models/slocalsettings.h \
    models/slocalsettingsstructs.h \
    models/spaymenttypemodel.h \
    models/spaymenttypesmodel.h \
    models/srepairmodel.h \
    models/srepairsaleitemmodel.h \
    models/srepairstatuslog.h \
    models/ssaletablemodel.h \
    models/ssloptionsdialog.h \
    models/ssortfilterproxymodel.h \
    models/ssqlquerymodel.h \
    models/sstandarditemmodel.h \
    models/sstoreitemmodel.h \
    models/sstoresaleitemmodel.h \
    models/suseractivitymodel.h \
    models/susermodel.h \
    models/sworkmodel.h \
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
    tabsalary.h \
    tabsettings.h \
    widgets/salarytabwidgets/spagesalaryextra.h \
    widgets/salarytabwidgets/spagesalaryitems.h \
    widgets/salarytabwidgets/spagesalarypayments.h \
    widgets/salarytabwidgets/spagesalarysales.h \
    widgets/salarytabwidgets/stableviewsalaryextra.h \
    widgets/salarytabwidgets/stableviewsalaryitems.h \
    widgets/salarytabwidgets/stableviewsalarypayments.h \
    widgets/salarytabwidgets/stableviewsalaryrepairparts.h \
    widgets/salarytabwidgets/stableviewsalaryrepairworks.h \
    widgets/salarytabwidgets/spagesalarybase.h \
    widgets/salarytabwidgets/spagesalaryreceptedissued.h \
    widgets/salarytabwidgets/spagesalaryrepairs.h \
    widgets/salarytabwidgets/spagesalarysummary.h \
    widgets/salarytabwidgets/stableviewsalaryreceptedissued.h \
    widgets/salarytabwidgets/stableviewsalaryrepairs.h \
    widgets/salarytabwidgets/stableviewsalarysaleparts.h \
    widgets/salarytabwidgets/stableviewsalarysales.h \
    widgets/sdoublespinbox.h \
    widgets/spageclientsummary.h \
    squerylog.h \
    stable.h \
    tabcashmoveexch.h \
    tabcashoperation.h \
    tabclient.h \
    tabclients.h \
    tabcommon.h \
    tabprintdialog.h \
    tabrepair.h \
    tabrepairnew.h \
    tabrepairs.h \
    tabsale.h \
    widgets/spagedinterface.h \
    widgets/spagesettingsdoctempateeditor.h \
    widgets/sreportscommonfunctions.h \
    widgets/stableviewbaseitemdelegates.h \
    widgets/stableviewclients.h \
    widgets/stableviewclientsitemdelegates.h \
    widgets/stableviewrepairsitemdelegates.h \
    widgets/stableviewgridlayout.h \
    widgets/stableviewbase.h \
    widgets/stableviewrepairs.h \
    widgets/getoutdialog.h \
    widgets/saletableitemdelegates.h \
    widgets/sclientmatch.h \
    widgets/scombobox.h \
    widgets/scomments.h \
    widgets/scomtextedit.h \
    widgets/sdevicematch.h \
    widgets/sgroupboxeventfilter.h \
    widgets/shortlivednotification.h \
    widgets/slineedit.h \
    widgets/sphone.h \
    widgets/sphones.h \
    widgets/splaintextedit.h \
    widgets/ssetcombobox.h \
    widgets/stabpage.h \
    widgets/stextedit.h \
    widgets/stoolboxswitchpanel.h \
    widgets/swidget.h \
    windowsdispatcher.h

FORMS += \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    models/repairtablefiltermenu.ui \
    models/ssloptionsdialog.ui \
    models/tabsalesettingsmenu.ui \
    tabsalary.ui \
    tabsettings.ui \
    widgets/salarytabwidgets/spagesalaryextra.ui \
    widgets/salarytabwidgets/spagesalaryitems.ui \
    widgets/salarytabwidgets/spagesalarypayments.ui \
    widgets/salarytabwidgets/spagesalaryreceptedissued.ui \
    widgets/salarytabwidgets/spagesalaryrepairs.ui \
    widgets/salarytabwidgets/spagesalarysales.ui \
    widgets/salarytabwidgets/spagesalarysummary.ui \
    widgets/spageclientsummary.ui \
    tabcashmoveexch.ui \
    tabcashoperation.ui \
    tabclient.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui \
    widgets/getoutdialog.ui \
    widgets/sclientmatch.ui \
    widgets/scomments.ui \
    widgets/sdevicematch.ui \
    widgets/spagesettingsdoctempateeditor.ui \
    widgets/sphone.ui \
    widgets/sphones.ui \
    widgets/stoolboxswitchpanel.ui

RESOURCES += styles/styles.qrc
RESOURCES += help/help.qrc

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
    LIB_DIR = $$PWD/libQt6
}
else {
    LIB_DIR = $$PWD/libQt5
}
INCLUDEPATH += $$LIB_DIR/include
DEPENDPATH += $$LIB_DIR/include
PROGRAM_FILES_DIR = C:\Program Files\SNAP CRM

LIBS += -lwinspool -lKernel32
LIBS += -L$$LIB_DIR -llimereport

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

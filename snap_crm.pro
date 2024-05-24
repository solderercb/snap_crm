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

greaterThan(QT_MAJOR_VERSION, 5){
    QT += core5compat
}

CONFIG += c++17

TARGET = snap
TEMPLATE = app

RC_FILE = resources.rc

system($${PWD}/appver-update.bat >nul 2>&1)

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
    amountToWords/amountToWords.cpp \
    bottoolbarwidget.cpp \
    chooseofficewindow.cpp \
    clients4test.cpp \
    global.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    models/propstruct.cpp \
    models/salarytabmodels/sadditionalpaymentmodel.cpp \
    models/salarytabmodels/ssalarymodel.cpp \
    models/salarytabmodels/ssalaryrepairsmodel.cpp \
    models/salarytabmodels/stablesalaryextramodel.cpp \
    models/salarytabmodels/stablesalaryitemsmodel.cpp \
    models/salarytabmodels/stablesalarypaymentsmodel.cpp \
    models/salarytabmodels/stablesalaryrepairpartsmodel.cpp \
    models/salarytabmodels/stablesalaryrepairworksmodel.cpp \
    models/salarytabmodels/stablesalaryrepairsmodel.cpp \
    models/salarytabmodels/stablesalaryreceptedissued.cpp \
    models/salarytabmodels/stablesalarysalepartsmodel.cpp \
    models/salarytabmodels/stablesalarysalesmodel.cpp \
    models/scartridgecardmodel.cpp \
    models/scartridgematerialmodel.cpp \
    models/scartridgematerialsmodel.cpp \
    models/scartridgerepairmodel.cpp \
    models/scomsettings.cpp \
    models/sdatabaseauxiliary.cpp \
    models/seditablebasemodel.cpp \
    models/sofficemodel.cpp \
    models/ssqlfetchingmodel.cpp \
    models/stablemodelscommonmethods.cpp \
    models/stablerepairsmodel.cpp \
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
    models/stabletechreportsmodel.cpp \
    models/stechreportmodel.cpp \
    models/suseractivitymodel.cpp \
    models/susermodel.cpp \
    models/susersettings.cpp \
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
    reports/sprintposreport.cpp \
    sapplication.cpp \
    tabreceptcartridge.cpp \
    tabrepaircartridges.cpp \
    tabsalary.cpp \
    tabsettings.cpp \
    tabtechreports.cpp \
    widgets/qtooltipper.cpp \
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
    widgets/scartridgeform.cpp \
    widgets/scartridgematerialstableitemdelegates.cpp \
    widgets/scartridgecard.cpp \
    widgets/sclientinputform.cpp \
    widgets/sdateedit.cpp \
    widgets/sdoublespinbox.cpp \
    widgets/settingstabwidgets/ssettingspagebase.cpp \
    widgets/settingstabwidgets/ssettingspageglobal.cpp \
    widgets/settingstabwidgets/ssettingspagerolesandpermissions.cpp \
    widgets/settingstabwidgets/ssettingspageuser.cpp \
    widgets/settingstabwidgets/stableviewpermissions.cpp \
    widgets/smodalwidget.cpp \
    widgets/spageclientsummary.cpp \
    widgets/srepairstablefiltermenu.cpp \
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
    widgets/speriodselector.cpp \
    widgets/sreportscommonfunctions.cpp \
    widgets/stableviewbase.cpp \
    widgets/stableviewbaseitemdelegates.cpp \
    widgets/stableviewclients.cpp \
    widgets/stableviewclientsitemdelegates.cpp \
    widgets/stableviewrepairsitemdelegates.cpp \
    widgets/stableviewrepairs.cpp \
    widgets/sdialogissuerepair.cpp \
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
    widgets/stableviewsummarybase.cpp \
    widgets/stableviewtechreports.cpp \
    widgets/stabpage.cpp \
    widgets/stextedit.cpp \
    widgets/stoolboxswitchpanel.cpp \
    widgets/swidget.cpp \
    widgets/sworksandparts.cpp \
    windowsdispatcher.cpp

HEADERS  += \
    3rdparty/QSerializer/src/qserializer.h \
    amountToWords/amountToWords.h \
    amountToWords/amountToWordsBg.h \
    amountToWords/amountToWordsBy.h \
    amountToWords/amountToWordsCn.h \
    amountToWords/amountToWordsIl.h \
    amountToWords/amountToWordsKz.h \
    amountToWords/amountToWordsMd.h \
    amountToWords/amountToWordsPl.h \
    amountToWords/amountToWordsRu.h \
    amountToWords/amountToWordsUa.h \
    amountToWords/amountToWordsUs.h \
    amountToWords/amountToWordsUz.h \
    applog.h \
    appver.h \
    bottoolbarwidget.h \
    chooseofficewindow.h \
    clients4test.h \
    com_sql_queries.h \
    global.h \
    loginwindow.h \
    mainwindow.h \
    models/propstruct.h \
    models/salarytabmodels/sadditionalpaymentmodel.h \
    models/salarytabmodels/ssalarymodel.h \
    models/salarytabmodels/ssalaryrepairsmodel.h \
    models/salarytabmodels/stablesalaryextramodel.h \
    models/salarytabmodels/stablesalaryitemsmodel.h \
    models/salarytabmodels/stablesalarypaymentsmodel.h \
    models/salarytabmodels/stablesalaryrepairpartsmodel.h \
    models/salarytabmodels/stablesalaryrepairworksmodel.h \
    models/salarytabmodels/stablesalaryreceptedissued.h \
    models/salarytabmodels/stablesalaryrepairsmodel.h \
    models/salarytabmodels/stablesalarysalepartsmodel.h \
    models/salarytabmodels/stablesalarysalesmodel.h \
    models/scartridgecardmodel.h \
    models/scartridgematerialmodel.h \
    models/scartridgematerialsmodel.h \
    models/scartridgerepairmodel.h \
    models/scomsettings.h \
    models/scomsettingstypes.h \
    models/sdatabaseauxiliary.h \
    models/seditablebasemodel.h \
    models/sofficemodel.h \
    models/spermissions.h \
    models/ssqlfetchingmodel.h \
    models/stablemodelscommonmethods.h \
    models/stablerepairsmodel.h \
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
    models/stabletechreportsmodel.h \
    models/stechreportmodel.h \
    models/suseractivitymodel.h \
    models/susermodel.h \
    models/susersettings.h \
    models/susersettingstypes.h \
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
    reports/sprintposreport.h \
    sapplication.h \
    tabreceptcartridge.h \
    tabrepaircartridges.h \
    tabsalary.h \
    tabsettings.h \
    tabtechreports.h \
    widgets/qtooltipper.h \
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
    widgets/scartridgeform.h \
    widgets/scartridgematerialstableitemdelegates.h \
    widgets/scartridgecard.h \
    widgets/sclientinputform.h \
    widgets/sdateedit.h \
    widgets/sdoublespinbox.h \
    widgets/settingstabwidgets/ssettingspagebase.h \
    widgets/settingstabwidgets/ssettingspageglobal.h \
    widgets/settingstabwidgets/ssettingspagerolesandpermissions.h \
    widgets/settingstabwidgets/ssettingspageuser.h \
    widgets/settingstabwidgets/stableviewpermissions.h \
    widgets/smodalwidget.h \
    widgets/spageclientsummary.h \
    widgets/srepairstablefiltermenu.h \
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
    widgets/speriodselector.h \
    widgets/sreportscommonfunctions.h \
    widgets/stableviewbaseitemdelegates.h \
    widgets/stableviewclients.h \
    widgets/stableviewclientsitemdelegates.h \
    widgets/stableviewrepairsitemdelegates.h \
    widgets/stableviewgridlayout.h \
    widgets/stableviewbase.h \
    widgets/stableviewrepairs.h \
    widgets/sdialogissuerepair.h \
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
    widgets/stableviewsummarybase.h \
    widgets/stableviewtechreports.h \
    widgets/stabpage.h \
    widgets/stextedit.h \
    widgets/stoolboxswitchpanel.h \
    widgets/swidget.h \
    widgets/sworksandparts.h \
    windowsdispatcher.h

FORMS += \
    bottoolbarwidget.ui \
    chooseofficewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    models/ssloptionsdialog.ui \
    models/tabsalesettingsmenu.ui \
    tabreceptcartridge.ui \
    tabrepaircartridges.ui \
    tabsalary.ui \
    tabsettings.ui \
    tabtechreports.ui \
    widgets/salarytabwidgets/spagesalaryextra.ui \
    widgets/salarytabwidgets/spagesalaryitems.ui \
    widgets/salarytabwidgets/spagesalarypayments.ui \
    widgets/salarytabwidgets/spagesalaryreceptedissued.ui \
    widgets/salarytabwidgets/spagesalaryrepairs.ui \
    widgets/salarytabwidgets/spagesalarysales.ui \
    widgets/salarytabwidgets/spagesalarysummary.ui \
    widgets/scartridgeform.ui \
    widgets/scartridgecard.ui \
    widgets/sclientinputform.ui \
    widgets/settingstabwidgets/ssettingspageglobal.ui \
    widgets/settingstabwidgets/ssettingspagerolesandpermissions.ui \
    widgets/settingstabwidgets/ssettingspageuser.ui \
    widgets/spageclientsummary.ui \
    widgets/srepairstablefiltermenu.ui \
    tabcashmoveexch.ui \
    tabcashoperation.ui \
    tabclient.ui \
    tabclients.ui \
    tabprintdialog.ui \
    tabrepair.ui \
    tabrepairnew.ui \
    tabrepairs.ui \
    tabsale.ui \
    widgets/sdialogissuerepair.ui \
    widgets/sclientmatch.ui \
    widgets/scomments.ui \
    widgets/sdevicematch.ui \
    widgets/spagesettingsdoctempateeditor.ui \
    widgets/speriodselector.ui \
    widgets/sphone.ui \
    widgets/sphones.ui \
    widgets/stoolboxswitchpanel.ui \
    widgets/sworksandparts.ui \

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

!exists(3rdparty/outpwd.tmp){
    system("copy NUL 3rdparty\\outpwd.tmp")    # пустышка на случай отсутствия файла
}

include(3rdparty/3rdparty-common.pri)

# копирование плагинов в папку с исполняемым файлом; копирование производится только если файлы изменены
PLUGINS_SRC = $${EXPORT_LIBS}/plugins
PLUGINS_DST = $${OUT_PWD}/$${BUILD_TYPE}/plugins
UPDATE_REQ = 1
exists($$PLUGINS_DST/updaters/qtifw.dll) {
    UPDATE_REQ = 0
    SRC_FILE_MD5 = $$system("@echo off & for /f \"SKIP=1 TOKENS=*\" %i in ('certutil -hashfile $$PLUGINS_SRC/updaters/qtifw.dll MD5') do (echo %i & exit)")
    DST_FILE_MD5 = $$system("@echo off & for /f \"SKIP=1 TOKENS=*\" %i in ('certutil -hashfile $$PLUGINS_DST/updaters/qtifw.dll MD5') do (echo %i & exit)")
    !equals(SRC_FILE_MD5, $$DST_FILE_MD5) {
        UPDATE_REQ = 1
    }
}

equals(UPDATE_REQ, 1) {
    PLUGINS_SRC ~= s,/,\\,g
    PLUGINS_DST ~= s,/,\\,g
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$${PLUGINS_SRC}\" \"$${PLUGINS_DST}\" $$escape_expand(\\n\\t)
}

INCLUDEPATH += $$EXPORT_LIBS/include
DEPENDPATH += $$EXPORT_LIBS/include

LIBS += -lwinspool -lKernel32
CONFIG(release, debug|release) {
    LIBS += -L$$EXPORT_LIBS -llimereport
}else{
    LIBS += -L$$EXPORT_LIBS -llimereportd
}
LIBS += -lQt5AutoUpdaterCore -lQt5AutoUpdaterWidgets
LIBS += -lEscPosQt


win32:BIN_DIR ~= s,/,\\,g
win32:EXPORT_LIBS ~= s,/,\\,g
QMAKE_PRE_LINK  += rcc -binary $${PWD}\\schema-updates\\schema-updates.qrc -o $${OUT_PWD}\\$${BUILD_TYPE}\\schema-updates.rcc $$escape_expand(\\n\\t)
#QMAKE_PRE_LINK += $$QMAKE_COPY \"$${EXPORT_LIBS}\\*.dll\" \"$${BIN_DIR}\"  $$escape_expand(\\n\\t)

TRANSLATIONS_PATH = $$PWD/lang
LANGUAGES = ru_RU #uk_UA be_BY kk_KZ uz_UZ en_US en_GB az_AZ bg_BG cs_CZ de_AT de_DE et_EE fi_FI hu_HU hy_AM ka_GE ku_TR lt_LT lv_LV pl_PL ro_RO tr_TR

defineReplace(prependAll) {
    for(a,$$1):result += $$2$${a}$$3
    return($$result)
}

TRANSLATIONS = $$prependAll(LANGUAGES, \"$$TRANSLATIONS_PATH/$${TARGET}_,.ts\")

qtPrepareTool(LUPDATE, lupdate)
ts.commands = $$LUPDATE \"$$PWD/snap_crm.pro\" -ts $$TRANSLATIONS
TRANSLATIONS_FILES =
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    qmfile = $$tsfile
    qmfile ~= s,".ts\"$",".qm\"",
    qm.commands += $$LRELEASE -removeidentical $$tsfile -qm $$qmfile $$escape_expand(\\n\\t)
    TRANSLATIONS_FILES += $$qmfile
}
qm.depends = ts
OTHER_FILES += $$TRANSLATIONS
QMAKE_EXTRA_TARGETS += qm ts
POST_TARGETDEPS +=  qm

DEST = $$OUT_PWD/$${BUILD_TYPE}/lang
win32:TRANSLATIONS_PATH ~= s,/,\\,g
win32:DEST ~= s,/,\\,g

QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
!exists($${DEST}){
   system("mkdir $$DEST")
}
for(qmfile, TRANSLATIONS_FILES) {
    win32:qmfile ~= s,/,\\,g
    QMAKE_POST_LINK += $$QMAKE_COPY $$qmfile \"$$DEST\" $$escape_expand(\\n\\t)
}

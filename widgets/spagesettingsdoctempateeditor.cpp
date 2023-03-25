#include "spagesettingsdoctempateeditor.h"
#include "ui_spagesettingsdoctempateeditor.h"
#include "global.h"
#include "com_sql_queries.h"

SPageSettingsDocTempateEditor::SPageSettingsDocTempateEditor(QWidget *parent) :
    STabPage(parent),
    ui(new Ui::SPageSettingsDocTempateEditor)
{
    ui->setupUi(this);
    ui->pushButtonImport->hide();
    ui->pushButtonExport->hide();
    ui->lineEdit->setButtons("Apply");

    QDir path;
    m_templatesDir = QApplication::applicationDirPath() + "/reports/";
    path.setPath(m_templatesDir);
    if (!path.exists())
        path.mkpath(m_templatesDir);

    docTemplatesList = new SSqlQueryModel();
    docTemplatesList->setQuery(QUERY_SEL_DOC_TEMPLATES_LIST, QSqlDatabase::database("connMain"));
    docTemplatesList->setObjectName("docTemplates");
    ui->comboBox->blockSignals(true);
    ui->comboBox->setModel(docTemplatesList);
    ui->comboBox->setCurrentIndex(-1);
    ui->comboBox->blockSignals(false);
//    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &SPageSettingsDocTempateEditor::selectTemplate); // такой формат здесь почему-то не подходит (ошибка)
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectTemplate(int)));

    m_report = new LimeReport::ReportEngine();
    reportDesigner = m_report->getDesignerWindow();

//    connect(m_report, SIGNAL(saveFinished()), this, SLOT(reportSaved()));
    connect(m_report, SIGNAL(onSave(bool&)), this, SLOT(reportOnSave(bool&)));

    ui->scrollArea->setWidget(reportDesigner);
    test_scheduler->start(200);
}

SPageSettingsDocTempateEditor::~SPageSettingsDocTempateEditor()
{
    delete ui;
    delete reportDesigner;
    delete m_report;
}

bool SPageSettingsDocTempateEditor::pageCloseRequest()
{
    return reportDesigner->checkNeedToSave();
}

void SPageSettingsDocTempateEditor::selectTemplate(int index)
{
    m_reportName = docTemplatesList->index(index, 1).data().toString();
    m_reportType = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).keyToValue(m_reportName.toLocal8Bit());

    CurrentFile.setFileName(m_templatesDir + m_reportName + ".lrxml");

    initDataSources();
    loadTemplateFromFile();
}

/* Загрузка ранее кэшированного файла отчета
 * Возвращает 1 в случае успеха
 * TODO: это почти копипаста из класса tabPrintDialog. Сам знаешь, что нужно сделать...
 */
bool SPageSettingsDocTempateEditor::loadTemplateFromFile()
{
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));

    if(!CurrentFile.exists())
    {
        if(!loadTemplateFromDB())
            return 0;
    }
    else    // если файл отчета кэширован ранее, нужно сравнить контрольные суммы
    {
        query.exec(QUERY_SEL_DOC_TEMPL_CHECKSUM(m_reportName));
        query.first();
        if(!query.isValid())
            return 0;

        QByteArray *buffer = readFile(CurrentFile);
        if(hash(buffer).toUpper() != query.value(0).toString())
        {
            if(!loadTemplateFromDB())
                return 0;
        }
        else
        {
            m_report->loadFromByteArray(buffer);
        }
        delete buffer;
    }

    return 1;
}

bool SPageSettingsDocTempateEditor::saveTemplateToFile()
{
    QByteArray *buffer = readFile(tmpFile);

    writeFile(CurrentFile, buffer);

    delete buffer;

    return 1;
}

/* Загрузка отчета из БД
 * Возвращает 1 в случае успеха
 * TODO: это почти копипаста из класса tabPrintDialog. Сам знаешь, что нужно сделать...
 */
bool SPageSettingsDocTempateEditor::loadTemplateFromDB()
{
    QByteArray buffer;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));

    query.exec(QUERY_SEL_DOC_TEMPL_DATA(m_reportName));
    query.first();
    if(!query.isValid())
        return 0;

    buffer = query.value(0).toByteArray();
    writeFile(CurrentFile, &buffer);
    m_report->loadFromByteArray(&buffer);   // При открытии пустого файла падает

    return 1;
}

bool SPageSettingsDocTempateEditor::saveTemplateToDB()
{
    tmpFile.setFileName(m_templatesDir + m_reportName + ".lrxml." + QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));         // временный файл для контроля записанных данных
    QByteArray buffer = m_report->saveToByteArray();
    if(!writeFile(tmpFile, &buffer))
        return 0;

    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));
    QByteArray vrfyBuffer;
    QString hashsum = hash(&buffer);

    query.exec(QUERY_SEL_DOC_TEMPL_CHECKSUM(m_reportName));
    query.first();
    if(query.isValid())
        if(hashsum.toUpper() == query.value(0).toString())    // вызов этого метода происходит по сигналу onSave и т. к. сигнал генерируется дважды нет смысла второй раз писать данные
            return 1;

    query.exec("BEGIN;");
    query.exec(QUERY_UPD_DOC_TEMPL_DATA(m_reportName, QString(buffer.toHex()), hashsum));
    if(query.lastError().isValid())
    {
        qDebug().nospace() << "[" << this << "] saveTemplateToDB() | query error";
        ; // error msg
        return 0;
    }
    query.exec(QUERY_SEL_DOC_TEMPL_DATA(m_reportName));
    query.first();
    if(!query.isValid())
        return 0;

    vrfyBuffer = query.value(0).toByteArray();
    if(hashsum != hash(&vrfyBuffer))
    {
        qDebug().nospace() << "[" << this << "] saveTemplateToDB() | hashes mismatch";
        ; // error msg
        query.exec("ROLLBACK;");
        return 0;
    }
    query.exec("COMMIT;");
    tmpFile.remove();
    if(!writeFile(CurrentFile, &buffer))
        return 0;

    return 1;
}

QByteArray *SPageSettingsDocTempateEditor::readFile(QFile &file)
{
    QByteArray *retBuffer = new QByteArray();

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't open file " << file.fileName();
        appLog->appendRecord("Report designer: read file from disk failed");
        return retBuffer;
    }
    *retBuffer = file.readAll();
    file.close();

    return retBuffer;
}

bool SPageSettingsDocTempateEditor::writeFile(QFile &file, QByteArray *data)
{
    if(!data)
    {
        qDebug().nospace() << "[" << this << "] readFile() | Нет данных, нечего записывать";
        appLog->appendRecord("Report designer: empty data, nothing to write");
        return 0;
    }

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "Не удалось сохранить файл бланка отчета на диск" << file.fileName();
        appLog->appendRecord("Report designer: save file to disk failed");
        return 0;
    }

    file.resize(data->size());
    file.write(*data);
    file.close();

    return 1;

}

QString SPageSettingsDocTempateEditor::hash(QByteArray *data)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(*data);
    return hash.result().toHex();
}

void SPageSettingsDocTempateEditor::reportSaved()
{   // Этот сигнал посылается когда файл отчета уже сохранён (перезаписан) (контролировал путём сравнения контрольной суммы с оригинальным)
//    qDebug().nospace() << "[" << this << "] reportSaved()";
}

void SPageSettingsDocTempateEditor::reportOnSave(bool &saved)
{   // Этот сигнал посылается при нажатии кнопки "Сохранить", но до того как файл будет перезаписан (контролировал путём сравнения контрольной суммы с оригинальным)
    // оставлю для истории
//    qDebug().nospace() << "[" << this << "] reportOnSave() | saved = " << saved;

    // здесь хитро получилось...  поскольку дизайнер загружает шаблон из QByteArray, то при сохранении сигнал OnSave генерируется дважды.
    // А т. к. после первого раза я сохранил на диске копию, то диалог сохранения файла не отображается.
    saveTemplateToDB();
}

QStandardItemModel* SPageSettingsDocTempateEditor::initDemoModel(const QStringList &demoHeaders, const QList<QVariant> &demoValues)
{
    QStandardItemModel *model = new QStandardItemModel();

    model->setHorizontalHeaderLabels(demoHeaders);
    foreach(QString item, demoHeaders)
    {
        QStandardItem *it = new QStandardItem(demoValues.at(demoHeaders.indexOf(item)).toString());
        model->setItem(0, demoHeaders.indexOf(item), it);
    }

    return model;
}

void SPageSettingsDocTempateEditor::initDataSources()
{
    switch (m_reportType)
    {
        case Global::Reports::new_rep: initRepairDataSources(); break;
//        case Global::Reports::pko: ; break;
//        case Global::Reports::reject: ; break;
//        case Global::Reports::rko: ; break;
        case Global::Reports::sticker1: initItemStickerDataSources(); break;
//        case Global::Reports::sticker2: ; break;
//        case Global::Reports::sticker3: ; break;
        case Global::Reports::rep_label: initRepairStickerDataSources(); break;
//        case Global::Reports::warranty: ; break;
//        case Global::Reports::works: ; break;
//        case Global::Reports::diag: ; break;
//        case Global::Reports::pn: ; break;
//        case Global::Reports::rn: ; break;
//        case Global::Reports::lost: ; break;
//        case Global::Reports::price1: ; break;
//        case Global::Reports::invoice0: ; break;
//        case Global::Reports::invoice1: ; break;
//        case Global::Reports::invoice2: ; break;
//        case Global::Reports::invoice3: ; break;
//        case Global::Reports::vatinvoice0: ; break;
//        case Global::Reports::p_list0: ; break;
//        case Global::Reports::w_list0: ; break;
//        case Global::Reports::new_cartridge: ; break;
//        case Global::Reports::sticker_cartridge: ; break;
//        case Global::Reports::issue_cartridge: ; break;
//        case Global::Reports::slip: ; break;
//        case Global::Reports::move: ; break;
//        case Global::Reports::buyout: ; break;
        default: qDebug().nospace() << "[" << this << "] initDataSources() | not implemented report type"; return;
    }
}

void SPageSettingsDocTempateEditor::initRepairDataSources()
{
    // Дэмо значения для бланка приёмки такие же, как и для стикера + доп. поля
    initRepairStickerDataSources();

    QStringList demoHeaders = QStringList{"name", "value", "comment"};
    QList<QVariant> demoValues = {"IMEI", "123456789ABCDEF", ""};
    m_report->dataManager()->addModel("additionalFields", initDemoModel(demoHeaders, demoValues), true);
    demoValues = {"Сохранность данных", "все данные можно удалить", ""};
    m_report->dataManager()->addModel("additionalFields", initDemoModel(demoHeaders, demoValues), true);

}

void SPageSettingsDocTempateEditor::initRepairStickerDataSources()
{
    // Дэмо значения для стикера
    QStringList demoHeaders = {"id", "Title", "client", "serial_number", "company", "office", "start_office", "manager", "master", "in_date", "express_repair", "is_warranty", "is_repeat", "box", "box_name", "barcode", "early", "ext_early", "fault", "complect", "look", "ext_notes"};
    QList<QVariant> demoValues = {12345, "Моноблок (All-in-One PC) Apple iMac12,1  Mid 2011  A1311 (EMC 2428)", 6325, "C02POIWERUJD", 1, 1, 1, 33, 33, "2022-01-25 10:26:32", 0, 0, 0, "NULL", "NULL", "012000123452", "NULL", "NULL", "не включается", "компьютер, клавиатура+мышь, кабель питания", "хорошее состояние", ""};
    m_report->dataManager()->addModel("repair", initDemoModel(demoHeaders, demoValues), true);

    demoHeaders = QStringList{"id", "name", "surname", "patronymic", "FioOrUrName", "address", "phone", "type", "is_regular", "is_dealer", "web_password"};
    demoValues = {6325, "Андрей", "Андреев", "Андреевич", "Андреев Андрей Андреевич", "пр. Дружбы народов, 6", "123-45-67", 0, 0, 0, "3YFG7"};
    m_report->dataManager()->addModel("customer", initDemoModel(demoHeaders, demoValues), true);

    // Преобразование QMap<QString, QVariant> в QStandardItemModel
    m_report->dataManager()->addModel("user", initDemoModel(userDbData->keys(), userDbData->values()), true);

    m_report->dataManager()->addModel("company", companiesModel, false);

    m_report->dataManager()->addModel("office", officesModel, false);

    // Преобразование QMap<QString, QVariant> в QStandardItemModel
    m_report->dataManager()->addModel("config", initDemoModel(comSettings->keys(), comSettings->values()), true);
}

void SPageSettingsDocTempateEditor::initItemStickerDataSources()
{
    QStringList demoHeaders = {"UID", "id", "Hidden", "articul", "dealer", "is_realization", "dealer_lock", "name", "state", "category", "store", "created", "updated", "count", "reserved", "units", "box", "box_name", "price_option", "custom_price_option", "currency_rate", "in_price", "in_price_base", "price", "price_base", "price2", "price2_base", "price3", "price3_base", "price4", "price4_base", "price5", "price5_base", "document", "part_request", "shop_title", "shop_description", "SN", "PN", "description", "shop_enable", "int_barcode", "ext_barcode", "in_count", "in_summ", "notes", "img1", "img2", "img3", "img4", "img5", "minimum_in_stock", "sold", "return_percent", "warranty", "warranty_dealer", "not_for_sale", "st_state", "st_notes", "ge_highlight", "last_stocktaking_date"};
    QList<QVariant> demoValues = {"016466-003790", 16466, 0, 3790, 1, 0, 0, "Гнездо micro USB, 2.0, на плату, Арт. 3790", 1, 82, 1, "2018-06-28 09:23:06", NULL, 1, 0, 0, NULL, NULL, 1, 2, 26.19, 10.7629, 9.95, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 2535, NULL, "", "", "", "Бахчев: тип 45", "Прежняя ПН: 2535\r\n", NULL, "011000164662", NULL, 2, 20, "Похож на Бахчев P/N тип 47\r\nарт. 3392 (без \"юбки\").\r\nПохож на Бахчев P/N тип 54 (без \"юбки\").\r\nПохож на Бахчев P/N тип 106\r\nарт. 4922 (без \"юбки\").\r\nПохож на Бахчев P/N тип 124 (без \"юбки\").\r\nВозможна замена на:", NULL, NULL, NULL, NULL, NULL, 0, 1, 0, 0, 0, 0, 0, NULL, 0, NULL};
    m_report->dataManager()->addModel("items", initDemoModel(demoHeaders, demoValues), true);
}

#ifdef QT_DEBUG
void SPageSettingsDocTempateEditor::randomFill()
{

}

void SPageSettingsDocTempateEditor::test_scheduler_handler()
{
    ui->comboBox->setCurrentIndex(0);
}

void SPageSettingsDocTempateEditor::test_scheduler2_handler()
{

}
#endif


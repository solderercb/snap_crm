#include "sreportscommonfunctions.h"

SReportsCommonFunctions::SReportsCommonFunctions()
{
    QDir path;
    m_templatesDir = SLocalSettings::appSettingsPath() + "/reports/";
    path.setPath(m_templatesDir);
    if (!path.exists())
        path.mkpath(m_templatesDir);

}

SReportsCommonFunctions::~SReportsCommonFunctions()
{
    if(m_repairsDemoModel)
        delete m_repairsDemoModel;
    if(i_listSourceDataCallbackHandler)
        delete i_listSourceDataCallbackHandler;
}

QString SReportsCommonFunctions::hash(QByteArray *data)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(*data);
    return hash.result().toHex();
}

void SReportsCommonFunctions::addDataModel(void *p)
{
    m_extDataModels.append(p);
}

void SReportsCommonFunctions::setTemplateName(const QString &reportName)
{
    CurrentFile.setFileName(m_templatesDir + reportName + ".lrxml");
}

QByteArray *SReportsCommonFunctions::readFile(QFile &file)
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

bool SReportsCommonFunctions::writeFile(QFile &file, QByteArray *data)
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

/* Загрузка ранее кэшированного файла отчета
 * Возвращает 1 в случае успеха
 */
bool SReportsCommonFunctions::loadTemplateFromFile()
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

/* Загрузка отчета из БД
 * Возвращает 1 в случае успеха
 */
bool SReportsCommonFunctions::loadTemplateFromDB()
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

QStandardItemModel *SReportsCommonFunctions::initDemoModel(const QStringList &demoHeaders, const QList<QVariant> &demoValues)
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

void SReportsCommonFunctions::initDataSources()
{
    // Преобразование QMap<QString, QVariant> в QStandardItemModel
    m_reportDatasouces << "user";
    LimeReport::ICallbackDatasource *userDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(userDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), userDbData, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(userDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), userDbData, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    m_report->dataManager()->addModel("company", companiesModel, false);
//    m_reportDatasouces << "company";
//    LimeReport::ICallbackDatasource *userDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
//    QObject::connect(companyDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), companiesModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
//    QObject::connect(companyDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), companiesModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    m_report->dataManager()->addModel("office", officesModel, false);
//    m_reportDatasouces << "office";
//    LimeReport::ICallbackDatasource *userDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
//    QObject::connect(officeDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), officesModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
//    QObject::connect(officeDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), officesModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    m_reportDatasouces << "config";
    LimeReport::ICallbackDatasource *configDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(configDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), comSettings, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(configDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), comSettings, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

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
        case Global::Reports::works: initWorksDataSources(); break;
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
        case Global::Reports::tech_report: initTechReportDataSources(); break;
        case Global::Reports::repairs:
        case Global::Reports::cartridges: initRepairsDataSources(); break;
        default: notImplementedReport(); return;
    }
}

void SReportsCommonFunctions::initCustomerDataSources(const int id)
{
    m_reportDatasouces << "customer";
    SClientModel *clientModel = new SClientModel();
    if(m_reportVars.isEmpty() && id == 0)
        clientModel->initDemo();
    else
        clientModel->load(id);
    LimeReport::ICallbackDatasource *customerDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(customerDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), clientModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(customerDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), clientModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    m_reportDatasouces << "customerPhones";
    SPhonesModel *clientPhonesModel = new SPhonesModel();
    if(m_reportVars.isEmpty() && id == 0)
        clientPhonesModel->initDemo();
    else
        clientPhonesModel->load(clientModel->id());
    LimeReport::ICallbackDatasource *customerPhonesDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(customerPhonesDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), clientPhonesModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(customerPhonesDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), clientPhonesModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));
}

void SReportsCommonFunctions::initRepairDataSources()
{
    // Источники данных для бланка приёмки такие же, как и для стикера + доп. поля
    initRepairStickerDataSources();

    m_reportDatasouces << "additionalFields";
    SFieldsModel *fieldsModel = new SFieldsModel(SFieldsModel::Repair);
    if(m_reportVars.isEmpty())
        fieldsModel->initDemo();
    else
        fieldsModel->load(m_reportVars.value("repair_id").toInt());
    LimeReport::ICallbackDatasource *fieldsDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(fieldsDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), fieldsModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(fieldsDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), fieldsModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));
}

void SReportsCommonFunctions::initRepairStickerDataSources()
{
    m_reportDatasouces << "repair";
    SRepairModel *repairModel = new SRepairModel();
    if(m_reportVars.isEmpty())
        repairModel->initDemo();
    else
        repairModel->load(m_reportVars.value("repair_id").toInt());
    LimeReport::ICallbackDatasource *repairDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(repairDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), repairModel, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(repairDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), repairModel, SLOT(reportCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    initCustomerDataSources(repairModel->clientId());
}

void SReportsCommonFunctions::initWorksDataSources()
{
    if(!i_listSourceDataCallbackHandler->isRepairsListSet())
    {
        i_clientId = 0;
        i_listSourceDataCallbackHandler->initDemoRepairsList();
    }

    initCustomerDataSources(i_clientId);

    m_reportDatasouces << "repair";
    LimeReport::ICallbackDatasource *repairDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(repairDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), i_listSourceDataCallbackHandler, SLOT(repairsListCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(repairDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), i_listSourceDataCallbackHandler, SLOT(repairsListCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));

    m_reportDatasouces << "works";
    LimeReport::ICallbackDatasource *worksDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(worksDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), i_listSourceDataCallbackHandler, SLOT(repairWorksListCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(worksDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), i_listSourceDataCallbackHandler, SLOT(repairWorksListCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));
}

void SReportsCommonFunctions::initItemStickerDataSources()
{
    QSqlQueryModel *itemsModel;

    m_reportDatasouces << "demo";
    QStringList demoHeaders = {"UID", "id", "Hidden", "articul", "dealer", "is_realization", "dealer_lock", "name", "state", "category", "store", "created", "updated", "count", "reserved", "units", "box", "box_name", "price_option", "custom_price_option", "currency_rate", "in_price", "in_price_base", "price", "price_base", "price2", "price2_base", "price3", "price3_base", "price4", "price4_base", "price5", "price5_base", "document", "part_request", "shop_title", "shop_description", "SN", "PN", "description", "shop_enable", "int_barcode", "ext_barcode", "in_count", "in_summ", "notes", "img1", "img2", "img3", "img4", "img5", "minimum_in_stock", "sold", "return_percent", "warranty", "warranty_dealer", "not_for_sale", "st_state", "st_notes", "ge_highlight", "last_stocktaking_date"};
    QList<QVariant> demoValues = {"016466-003790", 16466, 0, 3790, 1, 0, 0, "Гнездо micro USB, 2.0, на плату, Арт. 3790", 1, 82, 1, "2018-06-28 09:23:06", QVariant(), 1, 0, 0, "<box>", "<box_name>", 1, 2, 26.19, 10.7629, 9.95, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 2535, QVariant(), "", "", "", "Бахчев: тип 45", "Прежняя ПН: 2535\r\n", QVariant(), "011000164662", QVariant(), 2, 20, "Похож на Бахчев P/N тип 47\r\nарт. 3392 (без \"юбки\").\r\nПохож на Бахчев P/N тип 54 (без \"юбки\").\r\nПохож на Бахчев P/N тип 106\r\nарт. 4922 (без \"юбки\").\r\nПохож на Бахчев P/N тип 124 (без \"юбки\").\r\nВозможна замена на:", QVariant(), QVariant(), QVariant(), QVariant(), QVariant(), 0, 1, 0, 0, 0, 0, 0, QVariant(), 0, QVariant()};
    m_report->dataManager()->addModel(m_reportDatasouces.last(), initDemoModel(demoHeaders, demoValues), true);

    m_reportDatasouces << "demoList";
    itemsModel = new QSqlQueryModel();
    itemsModel->setQuery("SELECT CONCAT(LPAD(store_items.`id`, 6, '0'), '-', LPAD(store_items.`articul`, 6, '0')) AS 'UID', store_items.* FROM store_items LIMIT 2;", QSqlDatabase::database("connMain"));
    m_report->dataManager()->addModel(m_reportDatasouces.last(), itemsModel, true);
}

void SReportsCommonFunctions::initPKODataSources()
{
}

/* Список ремонтов (устройства и картриджи) */
void SReportsCommonFunctions::initRepairsDataSources()
{
    STableRepairsModel *model = nullptr;
    QList<void*>::const_iterator i = m_extDataModels.constBegin();
    while(i != m_extDataModels.constEnd())
    {
        model = reinterpret_cast<STableRepairsModel*>(*i++);
        if(model)
        {
            while(model->canFetchMore())
                model->fetchMore(1000);
            break;
        }
    }

    if(!model)
    {
        m_repairsDemoModel = new STableRepairsModel();
        model = m_repairsDemoModel;
        model->initDemo();
    }

    m_reportDatasouces << "repairs";
    LimeReport::ICallbackDatasource *repairsDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(repairsDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), model, SLOT(reportCallbackData(LimeReport::CallbackInfo,QVariant&)));
}

void SReportsCommonFunctions::initTechReportDataSources()
{
    if(!i_listSourceDataCallbackHandler->isTechReportsListSet())
    {
        i_listSourceDataCallbackHandler->initDemoTechReportsList();
    }

    m_reportDatasouces << "report";
    LimeReport::ICallbackDatasource *techReportDS = m_report->dataManager()->createCallbackDatasource(m_reportDatasouces.last());
    QObject::connect(techReportDS, SIGNAL(getCallbackData(LimeReport::CallbackInfo,QVariant&)), i_listSourceDataCallbackHandler, SLOT(techReportListCallbackData(LimeReport::CallbackInfo,QVariant&)));
    QObject::connect(techReportDS, SIGNAL(changePos(LimeReport::CallbackInfo::ChangePosType,bool&)), i_listSourceDataCallbackHandler, SLOT(techReportListCallbackDataChangePos(LimeReport::CallbackInfo::ChangePosType,bool&)));
}

/***************************************************************************************************************************************************
*/
SListSourceDataCallbackHandler::SListSourceDataCallbackHandler()
{

}

SListSourceDataCallbackHandler::~SListSourceDataCallbackHandler()
{
    if(m_repairsListOwned)
        while(!m_repairsList.isEmpty())
        {
            SRepairModel *m = m_repairsList.takeLast();
            delete m;
        }
}

void SListSourceDataCallbackHandler::setRepairsList(const QList<SRepairModel *> &list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    if(takeOwn)
    {
        for(auto *repair : list)
        {
            repair->setParent(this);
            repair->BOQModel()->setParent(this);
        }
    }

    m_repairsListSet = 1;
    m_repairsList = list;
    m_repairsListOwned = takeOwn;
    m_repairsListItem = m_repairsList.constBegin();
}

void SListSourceDataCallbackHandler::setTechReportsList(const QList<STechReportModel *> &list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    if(takeOwn)
    {
        for(auto *repair : list)
        {
            repair->setParent(this);
        }
    }

    m_techReportsListSet = 1;
    m_techReportsList = list;
    m_techReportsListOwned = takeOwn;
    m_techReportsListItem = m_techReportsList.constBegin();

}

void SListSourceDataCallbackHandler::initDemoRepairsList()
{
    SRepairModel *repair;
    SSaleTableModel *works;
    repair = new SRepairModel(this);
    works = new SSaleTableModel(this);
    repair->initDemo();
    works->initDemo();
    repair->setBOQModel(works);
    m_repairsList.append(repair);
    m_repairsListSet = 1;
    m_repairsListOwned = 1;
    m_repairsListItem = m_repairsList.constBegin();
}

bool SListSourceDataCallbackHandler::isRepairsListSet() const
{
    return m_repairsListSet;
}

bool SListSourceDataCallbackHandler::isTechReportsListSet() const
{
    return m_techReportsListSet;
}

void SListSourceDataCallbackHandler::initDemoTechReportsList()
{
    STechReportModel *report;
    report = new STechReportModel();
    report->initDemo();
    m_techReportsList.append(report);

    report = new STechReportModel();
    report->initDemo();
    m_techReportsList.append(report);

    m_techReportsListSet = 1;
    m_techReportsListOwned = 1;
    m_techReportsListItem = m_techReportsList.constBegin();
}

void SListSourceDataCallbackHandler::repairsListCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = (*m_repairsListItem)->id()?0:1; break;
        case LimeReport::CallbackInfo::HasNext: data = (m_repairsListItem != m_repairsList.constEnd()); break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = (*m_repairsListItem)->metaObject()->property(info.index + (*m_repairsListItem)->metaObject()->propertyOffset()).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = (*m_repairsListItem)->metaObject()->property((*m_repairsListItem)->metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(*m_repairsListItem); break;
        case LimeReport::CallbackInfo::ColumnCount: data = (*m_repairsListItem)->metaObject()->propertyCount() - (*m_repairsListItem)->metaObject()->propertyOffset(); break;
        case LimeReport::CallbackInfo::RowCount: data = m_repairsList.count(); break;
    }
    return;
}

void SListSourceDataCallbackHandler::repairsListCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    if(type == LimeReport::CallbackInfo::Next)
    {
        m_repairsListItem++;
        if(m_repairsListItem == m_repairsList.constEnd())
        {
            m_repairsListItem--;
            result = 0;
            return;
        }
    }
    else
    {
        m_repairsListItem = m_repairsList.constBegin();
    }
    result = 1;
    return;
}

void SListSourceDataCallbackHandler::repairWorksListCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    SSaleTableModel *BOQModel = (*m_repairsListItem)->BOQModel();
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = BOQModel->rowCount()?0:1; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = BOQModel->metaObject()->property(info.index + BOQModel->metaObject()->propertyOffset()).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = BOQModel->metaObject()->property(BOQModel->metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(BOQModel); break;
        case LimeReport::CallbackInfo::ColumnCount: data = BOQModel->metaObject()->propertyCount() - BOQModel->metaObject()->propertyOffset(); break;
        case LimeReport::CallbackInfo::RowCount: data = BOQModel->rowCount(); break;
    }
    return;
}

void SListSourceDataCallbackHandler::repairWorksListCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    (*m_repairsListItem)->BOQModel()->reportCallbackDataChangePos(type, result);
    return;
}

void SListSourceDataCallbackHandler::techReportListCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = (*m_techReportsListItem)->id()?0:1; break;
        case LimeReport::CallbackInfo::HasNext: data = (m_techReportsListItem != m_techReportsList.constEnd()); break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = (*m_techReportsListItem)->metaObject()->property(info.index + (*m_techReportsListItem)->metaObject()->propertyOffset()).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = (*m_techReportsListItem)->metaObject()->property((*m_techReportsListItem)->metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(*m_techReportsListItem); break;
        case LimeReport::CallbackInfo::ColumnCount: data = (*m_techReportsListItem)->metaObject()->propertyCount() - (*m_techReportsListItem)->metaObject()->propertyOffset(); break;
        case LimeReport::CallbackInfo::RowCount: data = m_techReportsList.count(); break;
    }
    return;

}

void SListSourceDataCallbackHandler::techReportListCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    if(type == LimeReport::CallbackInfo::Next)
    {
        m_techReportsListItem++;
        if(m_techReportsListItem == m_techReportsList.constEnd())
        {
            m_techReportsListItem--;
            result = 0;
            return;
        }
    }
    else
    {
        m_techReportsListItem = m_techReportsList.constBegin();
    }
    result = 1;
    return;
}

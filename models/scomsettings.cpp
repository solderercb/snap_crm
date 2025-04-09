#include "scomsettings.h"
#include "global.h"

static QString errMsg(QObject::tr("Не удалось сохранить основные настройки"));

SComSettings::SComSettings() :
    SPropertyCollection()
{
}

void SComSettings::initWidgets()
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("init__widget").insert(5, metaObject()->property(i).name()));


    // Модели данных виджетов (ComboBox) должны быть заданы до загрузки данных, иначе будет падать.
    setComboBoxModel("currencyId", currencyListModel);
    setComboBoxModel("defaultWorksWarranty", warrantyTermsModel);
    setComboBoxModel("defaultNewItemsWarranty", warrantyTermsModel);
    setComboBoxModel("defaultUsedItemsWarranty", warrantyTermsModel);
    setComboBoxModel("defaultRefItemsWarranty", warrantyTermsModel);
    setComboBoxModel("defaultDisasmItemsWarranty", warrantyTermsModel);
    setComboBoxModel("defaultOtherItemsWarranty", warrantyTermsModel);

    load();

/**************************************************************************************/
/******************* Виджеты, работа с которыми еще не реализована ********************/
/******************** см. метод SComSettings::prepareUpdateList() *********************/
/**************************************************************************************/
    disableWidget("repairDispatcherUsers");
    disableWidget("repairDispatcherDayLimit");
    disableWidget("repairDispatcherStatuses");
    disableWidget("ascPhoneMask1");
    disableWidget("ascPhoneMask2");
    disableWidget("timeZoneId");
    disableWidget("voipId");
    disableWidget("onlineStoreId");
    disableWidget("notifyNewCommentColor");
    disableWidget("notifyRepairStatusUpdateColor");
    disableWidget("notifyIncomingSMSColor");
    disableWidget("notifyOutOfTermAlarmColor");
    disableWidget("notifyDeviceMatchColor");
    disableWidget("notifyCustomTaskColor");
    disableWidget("notifyItemRequestColor");
    disableWidget("notifyOrderFromOnlineStoreColor");
    disableWidget("notifyIncomingCallColor");
    disableWidget("notifyItemPurchaseRequestColor");
}

void SComSettings::setComboBoxModel(const QString propertyName, SStandardItemModel *model)
{
    QComboBox *cb;

    cb = static_cast<QComboBox*>(i_editorWidgets.value(propertyName));
    Q_ASSERT_X(cb, metaObject()->className(), QString("comboBox %1 wasn't found").arg(propertyName).toLocal8Bit());

    cb->setModel(model);
}

void SComSettings::disableWidget(const QString propertyName)
{
    i_editorWidgets.value(propertyName)->setEnabled(false);
}

void SComSettings::load()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    for(i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("register__db_field").insert(9, metaObject()->property(i).name()), Qt::DirectConnection);

    // "переворот" таблицы config
    query->exec("SET @smth := NULL");
    query->exec("SELECT @smth := CONCAT(IF(@smth IS NOT NULL, CONCAT(@smth, '\\\nUNION ALL\\\n'), ''), 'SELECT \\\'', `COLUMN_NAME`, '\\\', `', `COLUMN_NAME`, '` FROM `config`') FROM information_schema.`COLUMNS` WHERE `TABLE_SCHEMA` = SCHEMA() AND `TABLE_NAME` = 'config';");

    query->last();
    query->exec(query->value(0).toString());
    while (query->next())
    {
        i = fieldToPropertyId(query->value(0).toString());
        if(i < 0)
            continue;

        metaObject()->property(i).write(this, query->value(1));
    }

    loadFromTableSettings();

    loadFromJson();

    delete query;
}

void SComSettings::loadFromTableSettings()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    query->exec("SELECT `name`, `value` FROM `settings`;");

    while(query->next())
    {
        i = fieldToPropertyId("settings." + query->value(0).toString());
        if(i < 0)
            continue;

        metaObject()->property(i).write(this, query->value(1));
    }

    delete query;
}

void SComSettings::loadFromJson()
{
    int i = 0;

    i = metaObject()->indexOfProperty("repairDispatcherUsers");
    metaObject()->property(i++).write(this, repairDispatcherUsersJson.Users.join(','));
    metaObject()->property(i++).write(this, repairDispatcherAssignCriteriaJson.DayLimit);
    metaObject()->property(i++).write(this, repairDispatcherAssignCriteriaJson.StatusList.join(','));

    i = metaObject()->indexOfProperty("emailServer");
    metaObject()->property(i++).write(this, emailConfigJson.Host);
    metaObject()->property(i++).write(this, emailConfigJson.Port);
    metaObject()->property(i++).write(this, emailConfigJson.Login);
    metaObject()->property(i++).write(this, emailConfigJson.Password);
    metaObject()->property(i++).write(this, emailConfigJson.Timeout);
    metaObject()->property(i++).write(this, emailConfigJson.EnableSsl);
    metaObject()->property(i++).write(this, emailConfigJson.EnableImplicitSsl);
    metaObject()->property(i++).write(this, emailConfigJson.Template);

    i = metaObject()->indexOfProperty("smsProvider");
    metaObject()->property(i++).write(this, smsConfigJson.Provider);
    metaObject()->property(i++).write(this, smsConfigJson.AuthType);
    metaObject()->property(i++).write(this, smsConfigJson.ApiId);
    metaObject()->property(i++).write(this, smsConfigJson.Login);
    metaObject()->property(i++).write(this, smsConfigJson.Password);
    metaObject()->property(i++).write(this, smsConfigJson.Sender);
}

// распределение изменённых значений по группам по названию таблиц
void SComSettings::prepareUpdateList(Table table)
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        QString propName = metaObject()->property(i).name();
        QString field = i_fieldNames.value(propName);
        if(field.startsWith("settings") != table)
            continue;

        if(table)   // Table::Settings
            field = field.replace("settings.", "");

        QVariant val = i_fieldModified.value(propName);
//        if(!val.isValid())                                // также временно отключено
//            val = i_jsonFieldModified.value(propName);

/******* TEMPORARY *******/
        // Данные отключенных виджетов не записывать в БД
        QWidget *w = i_editorWidgets.value(propName);
        if(w && !w->isEnabled())
            continue;
/**************/

        if(!val.isValid())
            continue;

        i_valuesMap.insert(field, val);
    }
}

void SComSettings::save()
{
    bool nErr = 1;
    QSqlQuery query(QSqlDatabase::database("connThird"));
    QString q;

    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        metaObject()->property(i).read(this);
    }

    updateJson();

    i_valuesMap.clear();
    prepareUpdateList(Table::Config);

    fieldsUpdFormatter();
    if(!i_valuesMap.isEmpty())
    {
        q = QString("UPDATE\n  `config`\nSET\n  %2\nWHERE `id` = 1;").arg(fields.join(",\n  "));
        QUERY_EXEC_TH(&query, nErr,q);
    }

    if(!nErr)
        Global::throwError(query.lastError(), errMsg);

    saveToTableSettings();

    i_fieldModified.clear();
    i_jsonFieldModified.clear();
}

void SComSettings::saveToTableSettings()
{
    bool nErr = 1;
    QSqlQuery query(QSqlDatabase::database("connThird"));

    i_valuesMap.clear();
    prepareUpdateList(Table::Settings);

    query.prepare(QString("UPDATE `settings` SET `value` = :value WHERE `name` = :name"));
    QMap<QString, QVariant>::const_iterator i = i_valuesMap.constBegin();
    while(i != i_valuesMap.constEnd())
    {
        query.bindValue(":name", i.key());
        query.bindValue(":value", i.value());
        nErr = query.exec();
        if(!nErr)
            Global::throwError(query.lastError(), errMsg);

        i++;
    }
}

// json объекты нужно обработать отдельно.
// И АСЦ и QSerializer нормально обрабатывают список значений из строк (каждое в кавычках) и из чисел (без кавычек)
void SComSettings::updateJson()
{
    QString dbValue = repairDispatcherUsersJson.Users.join(',');   // старое значение
    if(dbValue.compare(repairDispatcherUsers) != 0) // сравнение старого значения и нового из виджета
    {
        repairDispatcherUsersJson.Users = repairDispatcherUsers.split(',');
        i_jsonFieldModified.insert("repairDispatcherUsersJson", QString("[%1]").arg(repairDispatcherUsers));
    }

    dbValue = repairDispatcherAssignCriteriaJson.StatusList.join(',');   // старое значение
    if(dbValue.compare(repairDispatcherStatuses) != 0 || repairDispatcherAssignCriteriaJson.DayLimit != repairDispatcherDayLimit) // сравнение старого значения и нового из виджета
    {
        repairDispatcherAssignCriteriaJson.StatusList = repairDispatcherStatuses.split(',');
        repairDispatcherAssignCriteriaJson.DayLimit = repairDispatcherDayLimit;
        i_jsonFieldModified.insert("repairDispatcherAssignCriteriaJson", repairDispatcherAssignCriteriaJson.toRawJson());
    }

    if( emailConfigJson.Host != emailServer || emailConfigJson.Port != emailPort ||
        emailConfigJson.Login != emailLogin || emailConfigJson.Password != emailPassword ||
        emailConfigJson.Timeout != emailTimeout || emailConfigJson.EnableSsl != emailEnableSsl ||
        emailConfigJson.EnableImplicitSsl != emailEnableImplicitSsl || emailConfigJson.Template != emailTemplate )
    {
        emailConfigJson.Host = emailServer;
        emailConfigJson.Port = emailPort;
        emailConfigJson.Login = emailLogin;
        emailConfigJson.Password = emailPassword;
        emailConfigJson.Timeout = emailTimeout;
        emailConfigJson.EnableSsl = emailEnableSsl;
        emailConfigJson.EnableImplicitSsl = emailEnableImplicitSsl;
        emailConfigJson.Template = emailTemplate;
        i_jsonFieldModified.insert("emailConfigJson", emailConfigJson.toRawJson());
    }

    if( smsConfigJson.Provider != smsProvider || smsConfigJson.AuthType != smsAuthType ||
        smsConfigJson.ApiId != smsApiId || smsConfigJson.Login != smsLogin ||
        smsConfigJson.Password != smsPassword || smsConfigJson.Sender != smsSender )
    {
        smsConfigJson.Provider = smsProvider;
        smsConfigJson.AuthType = smsAuthType;
        smsConfigJson.ApiId = smsApiId;
        smsConfigJson.Login = smsLogin;
        smsConfigJson.Password = smsPassword;
        smsConfigJson.Sender = smsSender;
        i_jsonFieldModified.insert("smsConfigJson", smsConfigJson.toRawJson());
    }
}

void SComSettings::translate()
{
    tr("currencyId");
    tr("classicKassa");
    tr("timeZoneId");
    tr("ascPhoneMask1");
    tr("ascPhoneMask2");
    tr("updateChannel");
    tr("isPriceColOptVisible");
    tr("isPriceColOpt2Visible");
    tr("isPriceColOpt3Visible");
    tr("isPriceColRoznVisible");
    tr("isPriceColServiceVisible");
    tr("voipId");
    tr("isRealizatorEnable");
    tr("onlineStoreId");
    tr("isCartridgeRepairEnabled");
    tr("isEngineerRequiredOnDeviceRecept");
    tr("isVisitSourceRequired");
    tr("isSerialNumberRequired");
    tr("isCartridgeSerialNumberRequired");
    tr("isSearchOnlyBySerial");
    tr("isPhotoOnReceptRequired");
    tr("isPhotoOnIssueRequired");
    tr("isDiagRequired");
    tr("isVendorAddingAllowedOnRecept");
    tr("isRepairSummSetByManager");
    tr("isPaySalaryForRepairsIssuedInDebt");
    tr("isAutoSetCompanyOnRepairRecept");
    tr("isVendorWarrantyEnabled");
    tr("useSimplifiedCartridgeRepair");
    tr("autoCloseRepairTabTimeout");
    tr("repairImagesLimit");
    tr("repairDispatcherUsers");
    tr("repairDispatcherDayLimit");
    tr("repairDispatcherStatuses");
    tr("isClientPatronymicRequired");
    tr("isClientEmailRequired");
    tr("isClientAddressRequired");
    tr("isClientPhoneRequired");
    tr("isAnyClientDealer");
    tr("timeoutForItemsRequestsHandling");
    tr("timeoutForDiagnosisConfirmation");
    tr("defaultItemReserveTime");
    tr("itemImagesLimit");
    tr("isReasonForItemIncomeRequired");
    tr("defaultWorksWarranty");
    tr("defaultNewItemsWarranty");
    tr("defaultUsedItemsWarranty");
    tr("defaultRefItemsWarranty");
    tr("defaultDisasmItemsWarranty");
    tr("defaultOtherItemsWarranty");
    tr("notifyNewComment");
    tr("notifyRepairStatusUpdateColor");
    tr("notifyIncomingSMS");
    tr("notifyOutOfTermAlarmColor");
    tr("notifyDeviceMatch");
    tr("notifyCustomTask");
    tr("notifyItemRequest");
    tr("notifyOrderFromOnlineStore");
    tr("notifyIncomingCall");
    tr("notifyItemPurchaseRequest");
    tr("printPKO");
    tr("printOutInvoice");
    tr("printInInvoice");
    tr("printRKO");
    tr("printWarrantyDoc");
    tr("printWorksList");
    tr("printDiagResult");
    tr("printRepairRejectDoc");
    tr("printRKOOnItemIncome");
    tr("printCheck");
    tr("printRepairReceptDoc");
    tr("printRepairStickers");
    tr("printCartridgeReceptDoc");
    tr("printCartridgeStickers");
    tr("defaultRepairStickersQty");
    tr("autoSwitchKeyboardLayout");
    tr("voipAsteriskHost");
    tr("voipAsteriskPort");
    tr("voipAsteriskLogin");
    tr("voipAsteriskPassword");
    tr("voipAsteriskWebPort");
    tr("voipPrefix");
    tr("voipEndpoint");
    tr("voipKey");
    tr("voipSecret");
    tr("onlineStoreUrl");
    tr("onlineStoreKey");
    tr("salaryClassic");
    tr("salaryIncludeNotIssuedByDefault");
    tr("newClientSmsEnabled");
    tr("exchangeTypeAuto");
    tr("exchangeTypeManual");
    tr("backupEnable");
    tr("backupImages");
    tr("backupTime");
    tr("smsProvider");
    tr("smsAuthType");
    tr("smsApiId");
    tr("smsLogin");
    tr("smsPassword");
    tr("smsSender");
    tr("emailServer");
    tr("emailPort");
    tr("emailLogin");
    tr("emailPassword");
    tr("emailTimeout");
    tr("emailEnableSsl");
    tr("emailEnableImplicitSsl");
}

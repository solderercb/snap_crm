#include "scomsettings.h"
#include <QSqlQuery>
#include <ProjectGlobals>
#include <SCheckComboBox>
#include <SSqlQueryModel>

static QString errMsg(QObject::tr("Не удалось сохранить основные настройки"));
static SComSettingsTypesRegistrator comSettingsTypesRegistrator;

SComSettings::SComSettings() :
    SSettingsBase()
{
    m_record_settings = std::make_shared<QSqlRecord>();
    m_classDbMap = std::make_unique<QMap<int, int>>();  // создание объекта до вызова mapFields()
    mapFields();
    registerReportFields();
    connect(this, &SSettingsBase::beginDataChange, this, &SComSettings::setDataRework);
}

void SComSettings::insertNewField(const int index, const QString &name)
{
    QSqlField f;
    if(name.startsWith("settings."))
    {
        m_classDbMap->insert(index, (Table::Settings | m_record_settings->count()));
        f.setName(name.split('.').at(1));
        m_record_settings->append(f);
    }
    else
    {
        m_classDbMap->insert(index, m_record->count());
        f.setName(name);
        m_record->append(f);
    }
}

void SComSettings::load()
{
    SSettingsBase::load();    // загрузка из таблицы `config`, очистка кэша

    loadFromTableSettings();

    deserializeData();
}

void SComSettings::loadFromTableSettings()
{
    i_query->exec("SELECT `name`, `value` FROM `settings`;");

    while(i_query->next())
    {
        int index = m_record_settings->indexOf(i_query->value(0).toString());
        if(index < 0)
            continue;

        m_record_settings->setValue(index, i_query->value(1));
    }
}

/* Инициализация отдельных полей класса при загрузке:
 *  - обработка полей с JSON;
 *  - обработка полей, связанных с виджетами RadioButton
*/
void SComSettings::deserializeData()
{
    loadFromJson(C_repairDispatcherUsersJson, 1);
    loadFromJson(C_repairDispatcherAssignCriteriaJson, 1);
    loadFromJson(C_repairStatuses, 1);
    loadFromJson(C_emailConfigJson, 1);
    loadFromJson(C_smsConfigJson, 1);
}

/* Обработка полей таблиц, представленных в виде JSON и копирование значений в отдельные поля класса
 * (такие поля хранятся только в кэше)
*/
void SComSettings::loadFromJson(const int index, bool init)
{
    ModifiedField::State newState = init?ModifiedField::Executed:ModifiedField::Updated;
    switch (index)
    {
        case C_repairDispatcherUsersJson: {
            t_repairDispatcherUsers val = repairDispatcherUsersJsonVariantCopy();
            setCacheData(C_repairDispatcherUsersJson, QVariant::fromValue(val), newState);
            setCacheData(C_repairDispatcherUsers, val.Users.join(','), newState);
            break;}
        case C_repairDispatcherAssignCriteriaJson: {
            t_repairDispatcherAssignCriteria val = repairDispatcherAssignCriteriaJsonVariantCopy();
            setCacheData(C_repairDispatcherAssignCriteriaJson, QVariant::fromValue(val), newState);
            setCacheData(C_repairDispatcherDayLimit, val.DayLimit, newState);
            setCacheData(C_repairDispatcherStatuses, val.StatusList.join(','), newState);
            break;}
        case C_repairStatuses: {
            t_repairStatuses val = repairStatusesVariantCopy();
            setCacheData(C_repairStatuses, QVariant::fromValue(val), newState);
            break;}
        case C_emailConfigJson: {
            t_emailConfig val = emailConfigJsonVariantCopy();
            setCacheData(C_emailConfigJson, QVariant::fromValue(val), newState);
            setCacheData(C_emailServer, val.Host, newState);
            setCacheData(C_emailPort, val.Port, newState);
            setCacheData(C_emailLogin, val.Login, newState);
            setCacheData(C_emailPassword, val.Password, newState);
            setCacheData(C_emailTimeout, val.Timeout, newState);
            setCacheData(C_emailEnableSsl, val.EnableSsl, newState);
            setCacheData(C_emailEnableImplicitSsl, val.EnableImplicitSsl, newState);
            setCacheData(C_emailTemplate, val.Template, newState);
            break;}
        case C_smsConfigJson: {
            t_smsConfig val = smsConfigJsonVariantCopy();
            setCacheData(C_smsConfigJson, QVariant::fromValue(val), newState);
            setCacheData(C_smsProvider, val.Provider, newState);
            setCacheData(C_smsAuthType, val.AuthType, newState);
            setCacheData(C_smsApiId, val.ApiId, newState);
            setCacheData(C_smsLogin, val.Login, newState);
            setCacheData(C_smsPassword, val.Password, newState);
            setCacheData(C_smsSender, val.Sender, newState);
            break;}
//        case C_repairStatuses: ; break;
        default: break;
    }
}

void SComSettings::updateJson(const int index, const QVariant &value)
{
    auto updateRepairDispatcherAssignCriteriaJson = [=](const int index){
        auto val = repairDispatcherAssignCriteriaJsonVariantCopy();
        switch (index)
        {
            case C_repairDispatcherDayLimit: val.DayLimit = value.toInt(); break;
            case C_repairDispatcherStatuses: val.StatusList = value.toString().split(','); break;
            default: break;
        }
        set_repairDispatcherAssignCriteriaJson(val);
    };

    auto updateEmailJson = [=](const int index){
        auto val = emailConfigJsonVariantCopy();
        switch (index)
        {
            case C_emailServer: val.Host = value.toString(); break;
            case C_emailPort: val.Port = value.toInt(); break;
            case C_emailLogin: val.Login = value.toString(); break;
            case C_emailPassword: val.Password = value.toString(); break;
            case C_emailTimeout: val.Timeout = value.toInt(); break;
            case C_emailEnableSsl: val.EnableSsl = value.toBool(); break;
            case C_emailEnableImplicitSsl: val.EnableImplicitSsl = value.toBool(); break;
            case C_emailTemplate: val.Template = value.toString(); break;
            default: break;
        }
        set_emailConfigJson(val);
    };

    auto updateSmsJson = [=](const int index){
        auto val = smsConfigJsonVariantCopy();
        switch (index)
        {
            case C_smsProvider: val.Provider = value.toInt(); break;
            case C_smsAuthType: val.AuthType = value.toInt(); break;
            case C_smsApiId: val.ApiId = value.toString(); break;
            case C_smsLogin: val.Login = value.toString(); break;
            case C_smsPassword: val.Password = value.toString(); break;
            case C_smsSender: val.Sender = value.toString(); break;
            default: break;
        }
        set_smsConfigJson(val);
    };

    blockSignals(true);
    switch (index)
    {
        case C_repairDispatcherUsers: {
            auto val = repairDispatcherUsersJsonVariantCopy();
            val.Users = value.toString().split(',');
            set_repairDispatcherUsersJson(val);
            break;}

        case C_repairDispatcherDayLimit: updateRepairDispatcherAssignCriteriaJson(C_repairDispatcherDayLimit); break;
        case C_repairDispatcherStatuses: updateRepairDispatcherAssignCriteriaJson(C_repairDispatcherStatuses); break;

        case C_repairStatuses: {
            auto val = repairStatusesVariantCopy();
            set_repairStatuses(val);
            break;}

        case C_emailServer: updateEmailJson(C_emailServer); break;
        case C_emailPort: updateEmailJson(C_emailPort); break;
        case C_emailLogin: updateEmailJson(C_emailLogin); break;
        case C_emailPassword: updateEmailJson(C_emailPassword); break;
        case C_emailTimeout: updateEmailJson(C_emailTimeout); break;
        case C_emailEnableSsl: updateEmailJson(C_emailEnableSsl); break;
        case C_emailEnableImplicitSsl: updateEmailJson(C_emailEnableImplicitSsl); break;
        case C_emailTemplate: updateEmailJson(C_emailTemplate); break;

        case C_smsProvider: updateSmsJson(C_smsProvider); break;
        case C_smsAuthType: updateSmsJson(C_smsAuthType); break;
        case C_smsApiId: updateSmsJson(C_smsApiId); break;
        case C_smsLogin: updateSmsJson(C_smsLogin); break;
        case C_smsPassword: updateSmsJson(C_smsPassword); break;
        case C_smsSender: updateSmsJson(C_smsSender); break;
        default: break;
    }
    blockSignals(false);
}

int SComSettings::dbFieldIndex(const int classIndex) const
{
    return m_classDbMap->value(classIndex, -1);
}

QString SComSettings::dbFieldName(const int classIndex) const
{
    int dbi = dbFieldIndex(classIndex);
    if(dbi&Table::Settings)
        return m_record_settings->fieldName(dbi&(Table::Settings-1));

    return rec()->fieldName(dbi);
}

int SComSettings::targetTable(const int targetTableIndex)
{
    if(targetTableIndex&Table::Settings)
        return 1;

    return 0;
}

void SComSettings::setDataRework(const int index, QVariant &data)
{
    switch (index)
    {
        case C_onlineStoreUrl:
        case C_onlineStoreKey: if(data.toString().isEmpty()) data = QVariant(); break;
        default: break;
    }
}

void SComSettings::dataChangeHandler(const int index, const QVariant &data)
{
    loadFromJson(index);
    updateJson(index, data);
}

void SComSettings::save()
{
    SSettingsBase::save();
    saveToTableSettings();
}

void SComSettings::saveToTableSettings()
{
    if(!isDirty(1))
        return;

    bool nErr = 1;

    i_query->prepare(QString("UPDATE `settings` SET `value` = :value WHERE `name` = :name"));

    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(!((*i)->state() & (ModifiedField::Updated | ModifiedField::Failed)))
            continue;

        int index = i.key();
        if(dbFieldIndex(index) >= 0)
        {
            i_query->bindValue(":name", dbFieldName(index));
            i_query->bindValue(":value", fieldValueHandler((*i)->data()));
            nErr = i_query->exec();
        }
        if(!nErr)
            Global::throwError(i_query->lastError(), errMsg);

        setState(*i, ModifiedField::Executed);
    }
}

QString SComSettings::table()
{
    return "config";
}

QVariant SComSettings::loadedValue(const int dbTableIndex) const
{
    if(dbTableIndex & Table::Settings)
        return m_record_settings->value(dbTableIndex & (Table::Settings - 1));
    else
        return rec()->value(dbTableIndex);
}

/* Конфигурирование виджетов: установка моделей данных, деактивация не реализованных и прочие действия
*/
void SComSettings::configureWidgets()
{
    auto setComboBoxModel = [&](const int index, QAbstractItemModel *model){
        QComboBox *cb = dynamic_cast<QComboBox*>(i_editorWidgets[index]);
        if(cb)
            cb->setModel(model);
    };

    setComboBoxModel(C_currencyId, currencyListModel);
    setComboBoxModel(C_defaultWorksWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultNewItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultUsedItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultRefItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultDisasmItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultOtherItemsWarranty, warrantyTermsModel);

    SCheckComboBox *scb = static_cast<SCheckComboBox*>(i_editorWidgets[C_repairDispatcherUsers]);
    scb->setModel(engineersModel);
    scb->showSelectAllRow(true);

    scb = static_cast<SCheckComboBox*>(i_editorWidgets[C_repairDispatcherStatuses]);
#ifdef S_TEST
    scb->setModel(repairDispatcherStatesModel);
#endif
    scb->showSelectAllRow(false);

    /**************************************************************************************/
    /******************* Виджеты, работа с которыми еще не реализована ********************/
    /***************** данные таких виджетов не собираются при сохранении******************/
    /**************************************************************************************/
#ifndef S_TEST
    disableWidget(C_repairDispatcherUsers);
    disableWidget(C_repairDispatcherDayLimit);
    disableWidget(C_repairDispatcherStatuses);
#endif
    disableWidget(C_ascPhoneMask1);
    disableWidget(C_ascPhoneMask2);
    disableWidget(C_timeZoneId);
    disableWidget(C_voipId);
    disableWidget(C_onlineStoreId);
    disableWidget(C_notifyNewCommentColor);
    disableWidget(C_notifyRepairStatusUpdateColor);
    disableWidget(C_notifyIncomingSMSColor);
    disableWidget(C_notifyOutOfTermAlarmColor);
    disableWidget(C_notifyDeviceMatchColor);
    disableWidget(C_notifyCustomTaskColor);
    disableWidget(C_notifyItemRequestColor);
    disableWidget(C_notifyOrderFromOnlineStoreColor);
    disableWidget(C_notifyIncomingCallColor);
    disableWidget(C_notifyItemPurchaseRequestColor);
    disableWidget(C_exchangeSourceId);
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

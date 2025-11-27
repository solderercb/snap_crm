#include "srepairmodel.h"
#include <QSqlQuery>
#include <SSqlQueryModel>
#include <SAmountToWords>
#include <SUserSettings>
#include <ProjectQueries>
#include <SSaleTableModel>
#include <SRepairStatusLog>
#include <SCartridgeRepairModel>
#include <SCashRegisterModel>
#include <SLogRecordModel>

SRepairModel::SRepairModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "client" << "type" << "maker" << "office" << "manager" << "diagnostic_result" << "in_date" << "ext_notes" << "fault" << "reject_reason" << "company" << "start_office" << "current_manager" << "master";
    i_tableName = "workshop";
    setPrimaryKeyIndex(0);
    connect(this, &SSingleRowModel::beginDataChange, this, &SRepairModel::setDataRework);
    m_repairStatusLog = new SRepairStatusLog();
    i_logRecord->set_type(SLogRecordModel::Repair);
}

SRepairModel::~SRepairModel()
{
    delete m_repairStatusLog;
}

QString SRepairModel::constructSelectQuery()
{
    return QUERY_SEL_REPAIR_DATA(id());
}

void SRepairModel::convertTimestampFieldsToLocal()
{
    m_record->setValue(C_created, localDateTime(m_record->value(C_created).toDateTime()));
    m_record->setValue(C_issued, localDateTime(m_record->value(C_issued).toDateTime()));
    m_record->setValue(C_lockDateTime, localDateTime(m_record->value(C_lockDateTime).toDateTime()));
    m_record->setValue(C_lastSave, localDateTime(m_record->value(C_lastSave).toDateTime()));
    m_record->setValue(C_lastStatusChanged, localDateTime(m_record->value(C_lastStatusChanged).toDateTime()));
}

void SRepairModel::load()
{
    if(!isPrimaryKeyValid())
        loadError(Global::ThrowType::ConditionsError);

    beginUpdateModel();

    SSingleRowModelBase::load();

    if(m_record->value("is_cartridge").toBool())
    {
        if(m_cartridgeRepair == nullptr)
        {
            m_cartridgeRepair = new SCartridgeRepairModel(this);
            m_cartridgeRepair->setRepairId(id());
        }

        m_cartridgeRepair->load(cartridgeId());
    }

    endUpdateModel();
}

void SRepairModel::load(const int id)
{
    setPrimaryKey(id);
    load();
}

void SRepairModel::loadError(const int type)
{
    QString msg(tr("Не удалось загрузить SRepairModel; id ремонта: %1").arg(id()));
    Global::throwError(type, msg);
}

// демо-данные (например, для отчетов)
void SRepairModel::initDemo()
{
    m_record->setValue(C_id, 123456);
    m_record->setValue(C_isHidden, 0);
    m_record->setValue(C_title, "Моноблок (All-in-One PC) Apple iMac12,1  Mid 2011  A1311 (EMC 2428)");
    m_record->setValue(C_clientId, 0);
    m_record->setValue(C_classId, 12345);
    m_record->setValue(C_vendorId, 12345);
    m_record->setValue(C_deviceId, 12345);
    m_record->setValue(C_serialNumber, "C02POIWERUJD");
    m_record->setValue(C_companyId, 1);
    m_record->setValue(C_officeId, 1);
    m_record->setValue(C_startOfficeId, 1);
    m_record->setValue(C_managerId, managersModel->index(0, 1).data().toInt());
    m_record->setValue(C_currentManagerId, managersModel->index(0, 1).data().toInt());
    m_record->setValue(C_engineerId, engineersModel->index(0, 1).data().toInt());
    m_record->setValue(C_diagnosticResult, "Не включается");
    m_record->setValue(C_created, QDateTime::currentDateTimeUtc());
    m_record->setValue(C_issued,  QDateTime::currentDateTimeUtc());
    m_record->setValue(C_stateId, 0);
    m_record->setValue(C_newStateId, 0);
    m_record->setValue(C_isExpressRepair, 0);
    m_record->setValue(C_isQuickRepair, 0);
    m_record->setValue(C_isWarranty, 1);
    m_record->setValue(C_isRepeat, 1);
    m_record->setValue(C_paymentSystem, paymentSystemsModel->index(0, 1).data().toInt());
    m_record->setValue(C_isCardPayment, 0);
    m_record->setValue(C_canFormat, 0);
    m_record->setValue(C_printCheck, 0);
    m_record->setValue(C_boxId, repairBoxesModel->index(0, 1).data().toInt());
    m_record->setValue(C_warrantyLabel, "YHD092JG");
    m_record->setValue(C_extNotes, "текст примечания для клиента");
    m_record->setValue(C_isPrepaid, 1);
    m_record->setValue(C_prepaidTypeId, 0);
    m_record->setValue(C_prepaidSumm,  1000);
    m_record->setValue(C_prepaidOrderId, 12345);
    m_record->setValue(C_isPreAgreed, 1);
    m_record->setValue(C_isDebt, 0);
    m_record->setValue(C_preAgreedAmount,  2000);
    m_record->setValue(C_repairCost,  2000);
    m_record->setValue(C_realRepairCost,  2000);
    m_record->setValue(C_partsCost,  1500);
    m_record->setValue(C_fault, "не включается");
    m_record->setValue(C_complect, "компьютер-моноблок");
    m_record->setValue(C_look, "хорошее состояние");
    m_record->setValue(C_thirsPartySc, 0);
    m_record->setValue(C_warrantyDays, warrantyTermsModel->index(0, 1).data().toInt());
    m_record->setValue(C_barcode, "012000123452");
    m_record->setValue(C_rejectReason, "- причина отказа от ремонта -");
    m_record->setValue(C_informedStatusId, notifyStatusesModel->index(0, 1).data().toInt());
    m_record->setValue(C_color, "");
    m_record->setValue(C_early, 1234);
    m_record->setValue(C_extEarly, "oldDB 123");
    m_record->setValue(C_issuedMsg, "issuedMsg");
    m_record->setValue(C_smsInform, 0);
    m_record->setValue(C_invoiceId, 12345);
    m_record->setValue(C_cartridgeId, 0);
    m_record->setValue(C_warrantyVendorId, 0);
    m_record->setValue(C_termsControl, 0);
}

void SRepairModel::initCartridgeRepairModel(const int id)
{
    if(m_cartridgeRepair)
        qDebug().nospace() << "[" << this << "] initCartridgeRepairModel() | Warning! Model overwritten.";

    m_cartridgeRepair  = new SCartridgeRepairModel(this);
    m_cartridgeRepair->set_cardId(id);
}

bool SRepairModel::isNew()
{
    if(isPrimaryKeyValid())
        return false;

    return true;
}

/* Установка модели данных таблицы работ и деталей */
void SRepairModel::setBOQModel(WorkshopSaleModel *model)
{
    m_BOQModel = model;
}

WorkshopSaleModel *SRepairModel::BOQModel() const
{
    return m_BOQModel;
}

/* Период блокировки карточки ремонта в секундах
 * Время блокировки в поле `workshop`.`lock_datetime` должно обновляться с заданным периодом;
 * если по какой-то причине метка времени была установлена давно (больше периода), то карточка не считается заблокированной
*/
int SRepairModel::lockTimeout()
{
    return 30;
}

int SRepairModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(companyId());
}

void SRepairModel::setCompanyIndex(const int index)
{
    if(index == -1)
        return;
    set_companyId(companiesModel->databaseIDByRow(index));
}

int SRepairModel::officeIndex()
{
    return officesModel->rowByDatabaseID(officeId());
}

void SRepairModel::setOfficeIndex(const int index)
{
    if(index == -1)
        return;
    set_officeId(officesModel->databaseIDByRow(index));
}

int SRepairModel::startOfficeIndex()
{
    return officesModel->rowByDatabaseID(startOfficeId());
}

void SRepairModel::setStartOfficeIndex(const int index)
{
    if(index == -1)
        return;
    set_startOfficeId(officesModel->databaseIDByRow(index));
}

void SRepairModel::managerChanged(const int id)
{
    // Запись в журнал не производится, т. к. значение в поле manager записывается только при приёме в ремонт и в дальнейшем не изменяется
    m_repairStatusLog->set_manager(id);
}

int SRepairModel::managerIndex()
{
    return managersModel->rowByDatabaseID(managerId());
}

void SRepairModel::setManagerIndex(const int index)
{
    if(index == -1)
        return;
    set_managerId(managersModel->databaseIDByRow(index));
}

void SRepairModel::currentManagerChanged(const int id)
{
    appendLogText(tr("Менеджером ремонта назначен %1").arg(allUsersModel->value(id, "id", "username").toString()));
    m_repairStatusLog->set_manager(id);
}

int SRepairModel::currentManagerIndex()
{
    return managersModel->rowByDatabaseID(currentManagerId());
}

void SRepairModel::setCurrentManagerIndex(const int index)
{
    if(index == -1)
        return;
    set_currentManagerId(managersModel->databaseIDByRow(index));
}

void SRepairModel::engineerChanged(const int id)
{
    if(id)
    {
        appendLogText(tr("Инженером назначен %1").arg(allUsersModel->value(id, "id", "username").toString()));
    }
    m_repairStatusLog->set_engineer(id);
}

int SRepairModel::engineerIndex()
{
    return engineersModel->rowByDatabaseID(engineerId());
}

void SRepairModel::setEngineerIndex(const int index)
{
    if(index == -1)
    {
        set_engineerId(0);
        return;
    }
    set_engineerId(engineersModel->databaseIDByRow(index));
}

int SRepairModel::indexOfCreated()
{
    return C_created;
}

QVariant SRepairModel::inDateTime()
{
    return createdStr();
}

QVariant SRepairModel::inDate()
{
    return createdStr().left(10);  // dd.MM.yyyy
}

QVariant SRepairModel::outDate()
{
    return issued().toString("dd.MM.yyyy");
}

QVariant SRepairModel::outDateTime()
{
    return issued().toString("dd.MM.yyyy hh:mm:ss");
}

void SRepairModel::stateChanged(const int stateId)
{
    switch (stateId)
    {
        case Global::RepStateIds::GetIn: break;
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: set_issued(QDateTime::currentDateTime()); Q_FALLTHROUGH();
        default: appendLogText(tr("Статус заказа изменён на \"%1\"").arg(comSettings->repairStatusesVariantCopy()[stateId].Name));
    }

    m_repairStatusLog->set_repair(this->id());
    m_repairStatusLog->set_status(stateId);
    m_repairStatusLog->set_manager(currentManagerId()); // менеджер и мастер могут быть изменены между сменами статуса
    m_repairStatusLog->set_engineer(engineerId());
    updateLastSave();
    updateLastStatusChanged();
}

int SRepairModel::paymentSystemIndex()
{
    return paymentSystemsModel->rowByDatabaseID(paymentSystem());
}

void SRepairModel::setPaymentSystemIndex(const int index)
{
    set_paymentSystem(paymentSystemsModel->databaseIDByRow(index, "system_id"));
}

int SRepairModel::boxIndex()
{
    if(boxId())
        return repairBoxesModel->rowByDatabaseID(boxId());
    return -1;
}

void SRepairModel::setBoxIndex(const int index)
{
    set_boxId(repairBoxesModel->databaseIDByRow(index));
}

void SRepairModel::boxChanged(const int newId)
{
    auto old = commitedData(C_boxId).value_or(QVariant());

    if((newId == 0) && !old.isNull())
    {
        appendLogText(tr("Заказаз-наряд %1 изъят из ячейки \"%2\"").arg(id()).arg(repairBoxesModel->getDisplayRole(old.toInt())));
    }
    else if(newId > 0)
    {
        if(!old.isNull())
            appendLogText(tr("Заказаз-наряд %1 перемещён в ячейку \"%2\"").arg(id()).arg(repairBoxesModel->getDisplayRole(newId)));
        else
            appendLogText(tr("Заказаз-наряд %1 помещён в ячейку \"%2\"").arg(id()).arg(repairBoxesModel->getDisplayRole(newId)));
    }
}

/* Сумма платежей, связанных с ремонтом:
 *  - всех (вызов без параметров);
 *  - указанного типа (например, предоплат).
*/
double SRepairModel::paymentsAmount(const SCashRegisterModel::PaymentType type)
{
    double summ;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_PAYMENTS(id(), QString::number(type)));
    query->first();
    summ = query->value(0).toDouble();
    delete query;
    return summ;
}

/*  Запись данных о внесении или возврате предоплаты
 *  (возврат может использоваться при выдаче уст-ва без ремонта)
*/
void SRepairModel::addPrepay(double amount, QString reason)
{
    QString logText;
    double new_prepaidSumm = prepaidSumm() + (amount);
    set_prepaidSumm(new_prepaidSumm);
    if(amount > 0)
    {
        logText = tr("Предоплата в размере %1").arg(sysLocale.toCurrencyString(amount));
        if(!reason.isEmpty())
            logText.append(" (" + reason + ")");
    }
    else
        logText = tr("Возврат предоплаты в размере %1").arg(sysLocale.toCurrencyString(-amount));

    appendLogText(logText);
    set_isPrepaid( (new_prepaidSumm != 0) );
}

/*  Метод возвращает сумму всех записанных работ из таблицы works
*/
double SRepairModel::realWorksCost()
{
    double summ;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_WORKS(id()));
    query->first();
    summ = query->value(0).toDouble();
    delete query;
    return summ;
}

/*  Метод возвращает сумму всех добавленных в ремонт товаров из таблицы store_int_reserve
*/
double SRepairModel::realPartsCost()
{
    double summ;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_PARTS(id()));
    query->first();
    summ = query->value(0).toDouble();
    delete query;
    return summ;
}

void SRepairModel::updateLastSave()
{
    set_lastSave(QDateTime::currentDateTime());
}

void SRepairModel::updateLastStatusChanged()
{
    set_lastStatusChanged(QDateTime::currentDateTime());
}

void SRepairModel::genBarcode()
{
    // TODO: генерация правильного значения штрих-кода (в АСЦ неправильный)
    set_barcode("01200" + QString::number(id()).rightJustified(6, '0') + "2");
}

int SRepairModel::informedStatusIndex()
{
    return notifyStatusesModel->rowByDatabaseID(informedStatusId());
}

void SRepairModel::setInformedStatusIndex(const int index)
{
    set_informedStatusId(notifyStatusesModel->databaseIDByRow(index));
}

SCartridgeRepairModel *SRepairModel::cartridge()
{
    return m_cartridgeRepair;
}

void SRepairModel::setCartridge(SCartridgeRepairModel *model)
{
     m_cartridgeRepair = model;
}

bool SRepairModel::commitCartridge()
{
    if(!m_cartridgeRepair)
        return 1;

    bool nErr = 1;

    nErr = m_cartridgeRepair->commit();

    set_cartridgeId(m_cartridgeRepair->id());

    return nErr;
}

void SRepairModel::updateLogAssociatedRecId()
{
    i_logRecord->set_repair(id());
}

bool SRepairModel::commit()
{
    bool nErr = 1;
    if(isPrimaryKeyValid())
    {
        nErr &= update();
    }
    else
    {
        nErr &= commitCartridge();

        blockSignals(true);
        initMandatoryField(C_diagnosticResult, QVariant());
        initMandatoryField(C_rejectReason, "");
        initMandatoryField(C_created, QDateTime::currentDateTime());
        initMandatoryField(C_stateId, Global::RepStateIds::GetIn);
        initMandatoryField(C_companyId, userDbData->company());
        initMandatoryField(C_officeId, userDbData->currentOffice());
        initMandatoryField(C_startOfficeId, userDbData->currentOffice());
        initMandatoryField(C_engineerId, QVariant());
        blockSignals(false);
        nErr &= insert();
        setFieldsExecuted();
        appendLogText(tr("Устройство принято в ремонт №%1").arg(id()), "!");
        m_repairStatusLog->set_status(Global::RepStateIds::GetIn);
        m_repairStatusLog->set_repair(id());

        genBarcode();
        nErr &= update();
    }
    setFieldsExecuted();
    nErr &= m_repairStatusLog->commit();
    nErr &= commitLogs();

    return nErr;
}

bool SRepairModel::lock(bool state)
{
    bool nErr = 1;
    if(state)
    {
        setCacheData(C_userLock, userDbData->id());
        setCacheData(C_lockDateTime, QDateTime::currentDateTime());
    }
    else
    {
        setCacheData(C_userLock, QVariant());
        setCacheData(C_lockDateTime, QVariant());
    }

    auto pair = [&](const int index){return QString("  `%1` = %2").arg(dbFieldName(index)).arg(fieldValueHandler(data(index)));};
    auto upd_states = [&](ModifiedField::State state){setState(C_userLock, state); setState(C_lockDateTime, state);};
    auto fieldsPairs = QString("  %1,\n"\
                               "  %2")\
                               .arg(pair(C_userLock))
                               .arg(pair(C_lockDateTime));

    blockSignals(true); // в данном случае отправлять сигнал об обновлении модели не нужно
    try
    {
        QUERY_EXEC_TH(i_query, 1, QUERY_BEGIN);
        QUERY_EXEC_TH(i_query, 1, wrapUpdateQueryPairs(fieldsPairs));
        upd_states(ModifiedField::Executed);
        QUERY_EXEC_TH(i_query, 1, QUERY_COMMIT);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type != Global::ThrowType::ConnLost)
        {
            upd_states(ModifiedField::Failed);
            QUERY_EXEC_TH(i_query, 1, QUERY_ROLLBACK);
        }
    }
    blockSignals(false);
    return nErr;
}

bool SRepairModel::isLock()
{
    bool nErr = 1;
    QUERY_EXEC(i_query, nErr)(QUERY_SEL_REPAIR_LOCK(QSqlDatabase::database("connMain").databaseName(), id()));
    i_query->first();
    if(i_query->value(0).toInt() == userDbData->id()) // имеется запись о блокировке, но пользователь совпадает
        return 0;

    if(!i_query->value(1).toInt()) // пользователь не совпадает, но он оффлайн
        return 0;

    if(localDateTime(i_query->value(2).toDateTime()).secsTo(QDateTime::currentDateTime()) > (lockTimeout() + 3))   // время блокировки плюс некоторый запас на отправку запроса и получение данных
        return 0;

    return 1;
}

QVariant SRepairModel::devClass()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QVariant SRepairModel::vendor()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QVariant SRepairModel::device()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QVariant SRepairModel::company()
{
    return companiesModel->getDisplayRole(companyId());
}

QVariant SRepairModel::office()
{
    return officesModel->getDisplayRole(officeId());
}

QVariant SRepairModel::startOffice()
{
    return officesModel->getDisplayRole(startOfficeId());
}

QVariant SRepairModel::manager()
{
    return "TODO: implement this";  // нужна модель данных
}

QVariant SRepairModel::currentManager()
{
    return "TODO: implement this";  // нужна модель данных
}

QVariant SRepairModel::engineer()
{
    return "TODO: implement this";  // нужна модель данных
}

QVariant SRepairModel::box()
{
    return repairBoxesModel->getDisplayRole(boxId());
}

QVariant SRepairModel::prepaidType()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QVariant SRepairModel::prepaidSummStr()
{
    return sysLocale.toCurrencyString(prepaidSumm());
}

QVariant SRepairModel::realPrepaidSummStr()
{
    return sysLocale.toCurrencyString(paymentsAmount());
}

QVariant SRepairModel::preAgreedAmountStr()
{
    return sysLocale.toCurrencyString(preAgreedAmount());
}

QVariant SRepairModel::repairCostStr()
{
    return sysLocale.toCurrencyString(repairCost());
}

QVariant SRepairModel::realRepairCostStr()
{
    return sysLocale.toCurrencyString(realRepairCost());
}

QVariant SRepairModel::realRepairCostWords()
{
    return amountToWords(realRepairCost(), comSettings->currencyId());
}

QVariant SRepairModel::partsCostStr()
{
    return sysLocale.toCurrencyString(partsCost());
}

QVariant SRepairModel::realWorksCostStr()
{
    return sysLocale.toCurrencyString(realWorksCost());
}

QVariant SRepairModel::realPartsCostStr()
{
    return sysLocale.toCurrencyString(realPartsCost());
}

QVariant SRepairModel::warranty()
{
    return warrantyTermsMap->value(warrantyDays());
}

void SRepairModel::setDataRework(const int index, QVariant &data)
{
    switch (index)
    {
        case C_boxId:
        case C_engineerId: if(!data.toInt()) data = QVariant(); break;
        default: break;
    }
}

void SRepairModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
//        case : ; break;
//        case : ; break;
//        case : ; break;
        case C_managerId: managerChanged(data.toInt()); break;
        case C_currentManagerId: currentManagerChanged(data.toInt()); break;
        case C_engineerId: engineerChanged(data.toInt()); break;
        case C_diagnosticResult: appendLogText(tr("Результат диагностики изменён на \"%1\"").arg(data.toString())); break;
        case C_stateId: stateChanged(data.toInt()); break;
        case C_boxId: boxChanged(data.toInt()); break;
        case C_preAgreedAmount: appendLogText(tr("Предварительная стоимость ремонта: %1").arg(sysLocale.toCurrencyString(data.toDouble()))); break;
        case C_repairCost: appendLogText(tr("Стоимость ремонта в результатах диагностики изменёна на %1").arg(sysLocale.toCurrencyString(data.toDouble()))); break;
        case C_informedStatusId: appendLogText(tr("Статус информирования клиента изменён на \"%1\"").arg(notifyStatusesModel->getDisplayRole(data.toInt()))); break;
        default: break;
    }
}

void SRepairModel::setFieldsFailed()
{
    SSingleRowJModel::setFieldsFailed();
    m_repairStatusLog->setFieldsFailed();
}

#include "srepairmodel.h"

SRepairModel::SRepairModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "client" << "type" << "maker" << "office" << "manager" << "diagnostic_result" << "in_date" << "ext_notes" << "fault" << "reject_reason" << "company" << "start_office" << "current_manager" << "master";
    i_tableName = "workshop";
    i_idColumnName = "id";
    m_clientModel = new SClientModel();
    m_repairStatusLog = new SRepairStatusLog(0);
    i_logRecord->setType(SLogRecordModel::Repair);
}

SRepairModel::~SRepairModel()
{
    delete m_clientModel;
    delete m_repairStatusLog;
}

int SRepairModel::id()
{
    return i_id;
}

void SRepairModel::setId(const int id)
{
    i_id = id;
}

void SRepairModel::load(const int id)
{
    i_id = id;

    if(!i_id)
        loadError(Global::ThrowType::ConditionsError);

    QSqlQuery repair(QSqlDatabase::database("connMain"));
    repair.exec(QUERY_SEL_REPAIR_DATA(i_id));
    if(!repair.first())
        loadError(Global::ThrowType::ResultError);

    m_isHidden = repair.value("Hidden").toBool();
    m_title = repair.value("Title").toString();
    m_clientId = repair.value("client").toInt();
    m_type = repair.value("type").toInt();
    m_maker = repair.value("maker").toInt();
    m_model = repair.value("model").toInt();
    m_serialNumber = repair.value("serial_number").toString();
    m_company = repair.value("company").toInt();
    m_office = repair.value("office").toInt();
    m_startOffice = repair.value("start_office").toInt();
    m_manager = repair.value("manager").toInt();
    m_currentManager = repair.value("current_manager").toInt();
    m_master = repair.value("master").toInt();
    m_diagnosticResult = repair.value("diagnostic_result").toString();
    i_createdUtc = repair.value("in_date").toDateTime();
    m_outDate = repair.value("out_date").toDateTime();
    m_state = repair.value("state").toInt();
    m_newState = repair.value("new_state").toInt();
    m_userLock = repair.value("user_lock").toInt();
    m_lockDatetime = repair.value("lock_datetime").toDateTime();
    m_expressRepair = repair.value("express_repair").toBool();
    m_quickRepair = repair.value("quick_repair").toBool();
    m_isWarranty = repair.value("is_warranty").toBool();
    m_isRepeat = repair.value("is_repeat").toBool();
    m_paymentSystem = repair.value("payment_system").toInt();
    m_isCardPayment = repair.value("is_card_payment").toBool();
    m_canFormat = repair.value("can_format").toBool();
    m_printCheck = repair.value("print_check").toBool();
    m_box = repair.value("box").toInt();
    m_warrantyLabel = repair.value("warranty_label").toString();
    m_extNotes = repair.value("ext_notes").toString();
    m_isPrepaid = repair.value("is_prepaid").toBool();
    m_prepaidType = repair.value("prepaid_type").toInt();
    m_prepaidSumm = repair.value("prepaid_summ").toDouble();
    m_prepaidOrder = repair.value("prepaid_order").toInt();
    m_isPreAgreed = repair.value("is_pre_agreed").toBool();
    m_isDebt = repair.value("is_debt").toBool();
    m_preAgreedAmount = repair.value("pre_agreed_amount").toDouble();
    m_repairCost = repair.value("repair_cost").toDouble();
    m_realRepairCost = repair.value("real_repair_cost").toDouble();
    m_partsCost = repair.value("parts_cost").toDouble();
    m_fault = repair.value("fault").toString();
    m_complect = repair.value("complect").toString();
    m_look = repair.value("look").toString();
    m_thirsPartySc = repair.value("thirs_party_sc").toBool();
    m_lastSave = repair.value("last_save").toDateTime();
    m_lastStatusChanged = repair.value("last_status_changed").toDateTime();
    m_warrantyDays = repair.value("warranty_days").toInt();
    m_barcode = repair.value("barcode").toString();
    m_rejectReason = repair.value("reject_reason").toString();
    m_informedStatus = repair.value("informed_status").toInt();
    m_imageIds = repair.value("image_ids").toString();
    m_color = repair.value("color").toString();
    m_orderMoving = repair.value("order_moving").toString();
    m_early = repair.value("early").toInt();
    m_extEarly = repair.value("ext_early").toString();
    m_issuedMsg = repair.value("issued_msg").toString();
    m_smsInform = repair.value("sms_inform").toBool();
    m_invoice = repair.value("invoice").toInt();
    m_cartridge = repair.value("cartridge").toInt();
    if(repair.value("is_cartridge").toBool())
    {
        if(m_cartridgeRepair == nullptr)
        {
            m_cartridgeRepair = new SCartridgeRepairModel(this);
            m_cartridgeRepair->setRepairId(i_id);
        }

        m_cartridgeRepair->load(m_cartridge);
    }
    m_vendorId = repair.value("vendor_id").toInt();
    m_termsControl = repair.value("termsControl").toBool();
    m_repairStatusLog->setRepair(i_id);

    emit modelUpdated();
}

void SRepairModel::loadError(const int type)
{
    QString msg(tr("Не удалось загрузить SRepairModel; id ремонта: %1").arg(i_id));
    Global::throwError(type, msg);
}

// демо-данные (например, для отчетов)
void SRepairModel::initDemo()
{
    i_id = 123456;
    m_isHidden = 0;
    m_title = "Моноблок (All-in-One PC) Apple iMac12,1  Mid 2011  A1311 (EMC 2428)";
    m_clientId = 0;
    m_type = 12345;
    m_maker = 12345;
    m_model = 12345;
    m_serialNumber = "C02POIWERUJD";
    m_company = 1;
    m_office = 1;
    m_startOffice = 1;
    m_manager = managersModel->index(0, 1).data().toInt();
    m_currentManager = managersModel->index(0, 1).data().toInt();
    m_master = engineersModel->index(0, 1).data().toInt();
    m_diagnosticResult = "Не включается";
    i_createdUtc = QDateTime::currentDateTimeUtc();
    m_outDate =  QDateTime::currentDateTimeUtc();
    m_state = 0;
    m_newState = 0;
    m_expressRepair = 0;
    m_quickRepair = 0;
    m_isWarranty = 1;
    m_isRepeat = 1;
    m_paymentSystem = paymentSystemsModel->index(0, 1).data().toInt();
    m_isCardPayment = 0;
    m_canFormat = 0;
    m_printCheck = 0;
    m_box = repairBoxesModel->index(0, 1).data().toInt();
    m_warrantyLabel = "YHD092JG";
    m_extNotes = "текст примечания для клиента";
    m_isPrepaid = 1;
    m_prepaidType = 0;
    m_prepaidSumm =  1000;
    m_prepaidOrder = 12345;
    m_isPreAgreed = 1;
    m_isDebt = 0;
    m_preAgreedAmount =  2000;
    m_repairCost =  2000;
    m_realRepairCost =  2000;
    m_partsCost =  1500;
    m_fault = "не включается";
    m_complect = "компьютер-моноблок";
    m_look = "хорошее состояние";
    m_thirsPartySc = 0;
    m_warrantyDays = warrantyTermsModel->index(0, 1).data().toInt();
    m_barcode = "012000123452";
    m_rejectReason = "- причина отказа от ремонта -";
    m_informedStatus = notifyStatusesModel->index(0, 1).data().toInt();
    m_color = "";
    m_early = 1234;
    m_extEarly = "oldDB 123";
    m_issuedMsg = "issuedMsg";
    m_smsInform = 0;
    m_invoice = 12345;
    m_cartridge = 0;
    m_vendorId = 0;
    m_termsControl = 0;
}

void SRepairModel::initCartridgeRepairModel(const int id)
{
    if(m_cartridgeRepair)
        qDebug().nospace() << "[" << this << "] initCartridgeRepairModel() | Warning! Model overwritten.";

    m_cartridgeRepair  = new SCartridgeRepairModel(this);
    m_cartridgeRepair->setCardId(id);
}

bool SRepairModel::isNew()
{
    if(i_id)
        return false;

    return true;
}

/* Установка модели данных таблицы работ и деталей */
void SRepairModel::setBOQModel(SSaleTableModel *model)
{
    m_BOQModel = model;
}

SSaleTableModel *SRepairModel::BOQModel() const
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

bool SRepairModel::isHidden()
{
    return m_isHidden;
}

void SRepairModel::setHidden(const bool state)
{
    i_valuesMap.insert("Hidden", state);
}

QString SRepairModel::title()
{
    return m_title;
}

void SRepairModel::setTitle(const QString str)
{
    m_title = str;
    i_valuesMap.insert("Title", str);
}

int SRepairModel::clientId()
{
    return m_clientId;
}

void SRepairModel::setClientId(const int id)
{
    m_clientId = id;
    i_valuesMap.insert("client", id);
}

SClientModel *SRepairModel::clientModel()
{
    return m_clientModel;
}

int SRepairModel::classId()
{
    return m_type;
}

void SRepairModel::setClassId(const int classId)
{
    i_valuesMap.insert("type", classId);
}

int SRepairModel::vendorId()
{
    return m_maker;
}

void SRepairModel::setVendorId(const int vendorId)
{
    i_valuesMap.insert("maker", vendorId);
}

int SRepairModel::deviceId()
{
    return m_model;
}

void SRepairModel::setDeviceId(const int deviceId)
{
    i_valuesMap.insert("model", deviceId);
}

QString SRepairModel::serialNumber()
{
    return m_serialNumber;
}

void SRepairModel::setSerialNumber(const QString str)
{
    m_serialNumber = str;
    i_valuesMap.insert("serial_number", str);
}

int SRepairModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(m_company);
}

void SRepairModel::setCompanyIndex(const int index)
{
    if(index == -1)
        return;
    i_valuesMap.insert("company", companiesModel->databaseIDByRow(index));
}

int SRepairModel::office()
{
    return m_office;
}

int SRepairModel::officeIndex()
{
    return officesModel->rowByDatabaseID(m_office);
}

void SRepairModel::setOffice(const int id)
{
    i_valuesMap.insert("office", id);
}

void SRepairModel::setOfficeIndex(const int index)
{
    if(index == -1)
        return;
    setOffice(officesModel->databaseIDByRow(index));
}

int SRepairModel::startOfficeIndex()
{
    return officesModel->rowByDatabaseID(m_startOffice);
}

void SRepairModel::setStartOffice(const int id)
{
    i_valuesMap.insert("start_office", id);
}

void SRepairModel::setStartOfficeIndex(const int index)
{
    if(index == -1)
        return;
    setStartOffice(officesModel->databaseIDByRow(index));
}

int SRepairModel::manager()
{
    return m_manager;
}

int SRepairModel::managerIndex()
{
    return managersModel->rowByDatabaseID(m_manager);
}

void SRepairModel::setManager(const int id)
{
    i_valuesMap.insert("manager", id);
    // Запись в журнал не производится, т. к. значение в поле manager записывается только при приёме в ремонт и в дальнейшем не изменяется
    m_repairStatusLog->setManager(id);
}

void SRepairModel::setManagerIndex(const int index)
{
    if(index == -1)
        return;
    setManager(managersModel->databaseIDByRow(index));
}

int SRepairModel::currentManager()
{
    return m_currentManager;
}

int SRepairModel::currentManagerIndex()
{
    return managersModel->rowByDatabaseID(m_currentManager);
}

void SRepairModel::setCurrentManager(const int id)
{
    i_valuesMap.insert("current_manager", id);
    appendLogText(tr("Менеджером ремонта назначен %1").arg(allUsersModel->value(id, "id", "username").toString()));
    m_repairStatusLog->setManager(id);
}

void SRepairModel::setCurrentManagerIndex(const int index)
{
    if(index == -1)
        return;
    setCurrentManager(managersModel->databaseIDByRow(index));
}

int SRepairModel::engineer()
{
    return m_master;
}

int SRepairModel::engineerIndex()
{
    return engineersModel->rowByDatabaseID(m_master);
}

void SRepairModel::setEngineer(const int id)
{
    m_master = id;
    if(id)
    {
        i_valuesMap.insert("master", m_master);
        appendLogText(tr("Инженером назначен %1").arg(allUsersModel->value(m_master, "id", "username").toString()));
    }
    else
        i_valuesMap.insert("master", QVariant());
    m_repairStatusLog->setEngineer(m_master);
}

void SRepairModel::setEngineerIndex(const int index)
{
    if(index == -1)
    {
        setEngineer(0);
        return;
    }
    setEngineer(engineersModel->databaseIDByRow(index));
}

QString SRepairModel::diagnosticResult()
{
    return m_diagnosticResult;
}

void SRepairModel::setDiagnosticResult(const QVariant value)
{
    i_valuesMap.insert("diagnostic_result", value);
}

void SRepairModel::setDiagnosticResult(const QString str)
{
    m_diagnosticResult = str;
    setDiagnosticResult(QVariant(m_diagnosticResult));
    appendLogText(tr("Результат диагностики изменён на \"%1\"").arg(m_diagnosticResult));
}

QString SRepairModel::inDate()
{
    return created().left(10);  // dd.MM.yyyy
}

void SRepairModel::setInDate(const QDateTime timestamp)
{
    i_createdUtc = timestamp.toUTC();
    i_valuesMap.insert("in_date", timestamp);
}

QString SRepairModel::outDate()
{
    return localDateTime(m_outDate).toString("dd.MM.yyyy");
}

QString SRepairModel::outDateTime()
{
    return localDateTime(m_outDate).toString("dd.MM.yyyy hh:mm:ss");
}

void SRepairModel::setOutDate(const QDateTime timestamp)
{
    i_valuesMap.insert("out_date", timestamp);
}

int SRepairModel::state()
{
    return m_state;
}

void SRepairModel::setState(const int id)
{
    if(m_state == id)
        return;

    m_state = id;
    i_valuesMap.insert("state", m_state);
    switch (id)
    {
        case Global::RepStateIds::GetIn: break;
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: setOutDate(QDateTime::currentDateTime()); Q_FALLTHROUGH();
        default: appendLogText(tr("Статус заказа изменён на \"%1\"").arg(comSettings->repairStatuses[m_state].Name));
    }

    m_repairStatusLog->setStatus(m_state);
    m_repairStatusLog->setManager(m_currentManager); // менеджер и мастер могут быть изменены между сменами статуса
    m_repairStatusLog->setEngineer(m_master);
    updateLastSave();
    updateLastStatusChanged();
}

int SRepairModel::newState()
{
    return m_newState;
}

void SRepairModel::setNewState(const int id)
{
    i_valuesMap.insert("new_state", id);
}

int SRepairModel::userLock()
{
    return m_userLock;
}

void SRepairModel::setUserLock(const QVariant id)
{
    i_valuesMap.insert("user_lock", id);
}

QDateTime SRepairModel::lockDatetime()
{
    return m_lockDatetime;
}

void SRepairModel::setLockDatetime(const QVariant timestamp)
{
    i_valuesMap.insert("lock_datetime", timestamp);
}

bool SRepairModel::expressRepair()
{
    return m_expressRepair;
}

void SRepairModel::setExpressRepair(const bool state)
{
    m_expressRepair = state;
    i_valuesMap.insert("express_repair", state);
}

bool SRepairModel::quickRepair()
{
    return m_quickRepair;
}

void SRepairModel::setQuickRepair(const bool state)
{
    m_quickRepair = state;
    i_valuesMap.insert("quick_repair", state);
}

bool SRepairModel::isWarranty()
{
    return m_isWarranty;
}

void SRepairModel::setIsWarranty(const bool state)
{
    m_isWarranty = state;
    i_valuesMap.insert("is_warranty", state);
}

bool SRepairModel::isRepeat()
{
    return m_isRepeat;
}

void SRepairModel::setIsRepeat(const bool state)
{
    m_isRepeat = state;
    i_valuesMap.insert("is_repeat", state);
}

int SRepairModel::paymentSystem()
{
    return m_paymentSystem;
}

int SRepairModel::paymentSystemIndex()
{
    return paymentSystemsModel->rowByDatabaseID(m_paymentSystem);
}

void SRepairModel::setPaymentSystem(const int id)
{
    i_valuesMap.insert("payment_system", id);
}

void SRepairModel::setPaymentSystemIndex(const int index)
{
    i_valuesMap.insert("payment_system", paymentSystemsModel->databaseIDByRow(index, "system_id"));
}

bool SRepairModel::isCardPayment()
{
    return m_isCardPayment;
}

void SRepairModel::setIsCardPayment(const bool state)
{
    i_valuesMap.insert("is_card_payment", state);
}

bool SRepairModel::canFormat()
{
    return m_canFormat;
}

void SRepairModel::setCanFormat(const bool state)
{
    i_valuesMap.insert("can_format", state);
}

bool SRepairModel::printCheck()
{
    return m_printCheck;
}

void SRepairModel::setPrintCheck(const bool state)
{
    i_valuesMap.insert("print_check", state);
}

int SRepairModel::boxIndex()
{
    if(m_box)
        return repairBoxesModel->rowByDatabaseID(m_box);
    return -1;
}

void SRepairModel::setBoxIndex(const int index)
{
    int current_box = m_box;
    int new_box = repairBoxesModel->databaseIDByRow(index);

    if(current_box == new_box)
        return;

    if(index < 0 && current_box)
    {
        appendLogText(tr("Заказаз-наряд %1 изъят из ячейки \"%2\"").arg(i_id).arg(repairBoxesModel->getDisplayRole(current_box)));
        i_valuesMap.insert("box", QVariant());
        m_box = 0;
    }
    else if(index >= 0)
    {
        m_box = new_box;
        if(current_box)
            appendLogText(tr("Заказаз-наряд %1 перемещён в ячейку \"%2\"").arg(i_id).arg(repairBoxesModel->getDisplayRole(m_box)));
        else
            appendLogText(tr("Заказаз-наряд %1 помещён в ячейку \"%2\"").arg(i_id).arg(repairBoxesModel->getDisplayRole(m_box)));
        i_valuesMap.insert("box", repairBoxesModel->databaseIDByRow(index));
    }
}

QString SRepairModel::warrantyLabel()
{
    return m_warrantyLabel;
}

void SRepairModel::setWarrantyLabel(const QString str)
{
    i_valuesMap.insert("warranty_label", str);
}

QString SRepairModel::extNotes()
{
    return m_extNotes;
}

void SRepairModel::setExtNotes(const QString str)
{
    m_extNotes = str;
    i_valuesMap.insert("ext_notes", str);
}

bool SRepairModel::isPrepaid()
{
    return m_isPrepaid;
}

void SRepairModel::setIsPrepaid(const bool state)
{
    i_valuesMap.insert("is_prepaid", state);
}

int SRepairModel::prepaidType()
{
    return m_prepaidType;
}

void SRepairModel::setPrepaidType(const int id)
{
    i_valuesMap.insert("prepaid_type", id);
}

double SRepairModel::prepaidSumm()
{
    return m_prepaidSumm;
}

void SRepairModel::setPrepaidSumm(const double summ)
{
    m_prepaidSumm = summ;
    i_valuesMap.insert("prepaid_summ", summ);
}

double SRepairModel::paymentsAmount(const SCashRegisterModel::PaymentType type)
{
    double summ;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_PAYMENTS(i_id, QString::number(type)));    // если тип не указан, то будут выбраны все записи, относящиеся к данному ремонту
    query->first();
    summ = query->value(0).toDouble();
    delete query;
    return summ;
}

int SRepairModel::prepaidOrder()
{
    return m_prepaidOrder;
}

void SRepairModel::setPrepaidOrder(const int id)
{
    i_valuesMap.insert("prepaid_order", id);
}

/*  Запись данных о внесении или возврате предоплаты
 *  (возврат может использоваться при выдаче уст-ва без ремонта)
*/
void SRepairModel::addPrepay(double amount, QString reason)
{
    QString logText;
    double new_prepaidSumm = m_prepaidSumm + (amount);
    setPrepaidSumm(new_prepaidSumm);
    if(amount > 0)
    {
        logText = tr("Предоплата в размере %1").arg(sysLocale.toCurrencyString(amount));
        if(!reason.isEmpty())
            logText.append(" (" + reason + ")");
    }
    else
        logText = tr("Возврат предоплаты в размере %1").arg(sysLocale.toCurrencyString(-amount));

    appendLogText(logText);
    setIsPrepaid( (new_prepaidSumm != 0) );
}

bool SRepairModel::isPreAgreed()
{
    return m_isPreAgreed;
}

void SRepairModel::setIsPreAgreed(const bool state)
{
    i_valuesMap.insert("is_pre_agreed", state);
}

bool SRepairModel::isDebt()
{
    return m_isDebt;
}

void SRepairModel::setIsDebt(const bool state)
{
    i_valuesMap.insert("is_debt", state);
}

double SRepairModel::preAgreedAmount()
{
    return m_preAgreedAmount;
}

void SRepairModel::setPreAgreedAmount(const double summ)
{
    m_preAgreedAmount = summ;
    i_valuesMap.insert("pre_agreed_amount", summ);
    appendLogText(tr("Предварительная стоимость ремонта: %1").arg(sysLocale.toCurrencyString(summ)));
}

double SRepairModel::repairCost()
{
    return m_repairCost;
}

void SRepairModel::setRepairCost(const double amount)
{
    m_repairCost = amount;
    i_valuesMap.insert("repair_cost", m_repairCost);
    appendLogText(tr("Стоимость ремонта в результатах диагностики изменёна на %1").arg(sysLocale.toCurrencyString(m_repairCost)));
}

double SRepairModel::realRepairCost()
{
    return m_realRepairCost;
}

void SRepairModel::setRealRepairCost(const double summ)
{
    m_realRepairCost = summ;
    i_valuesMap.insert("real_repair_cost", summ);
}

double SRepairModel::partsCost()
{
    return m_partsCost;
}

void SRepairModel::setPartsCost(const double summ)
{
    m_partsCost = summ;
    i_valuesMap.insert("parts_cost", summ);
}

/*  Метод возвращает сумму всех записанных работ из таблицы works
*/
double SRepairModel::realWorksCost()
{
    double summ;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    query->exec(QUERY_SEL_REPAIR_WORKS(i_id));
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
    query->exec(QUERY_SEL_REPAIR_PARTS(i_id));
    query->first();
    summ = query->value(0).toDouble();
    delete query;
    return summ;
}

QString SRepairModel::fault()
{
    return m_fault;
}

void SRepairModel::setFault(const QString str)
{
    i_valuesMap.insert("fault", str);
}

QString SRepairModel::complect()
{
    return m_complect;
}

void SRepairModel::setComplect(const QString str)
{
    i_valuesMap.insert("complect", str);
}

QString SRepairModel::look()
{
    return m_look;
}

void SRepairModel::setLook(const QString str)
{
    i_valuesMap.insert("look", str);
}

bool SRepairModel::thirsPartySc()
{
    return m_thirsPartySc;
}

void SRepairModel::setThirsPartySc(const bool state)
{
    i_valuesMap.insert("thirs_party_sc", state);
}

QDateTime SRepairModel::lastSave()
{
    return m_lastSave;
}

void SRepairModel::setLastSave(const QDateTime timestamp)
{
    i_valuesMap.insert("last_save", timestamp);
}

void SRepairModel::updateLastSave()
{
    setLastSave(QDateTime::currentDateTime());
}

QDateTime SRepairModel::lastStatusChanged()
{
    return m_lastStatusChanged;
}

void SRepairModel::setLastStatusChanged(const QDateTime timestamp)
{
    i_valuesMap.insert("last_status_changed", timestamp);
}

void SRepairModel::updateLastStatusChanged()
{
    setLastStatusChanged(QDateTime::currentDateTime());
}

int SRepairModel::warrantyDays()
{
    return m_warrantyDays;
}

void SRepairModel::setWarrantyDays(const int id)
{
    i_valuesMap.insert("warranty_days", id);
}

QString SRepairModel::barcode()
{
    return m_barcode;
}

void SRepairModel::setBarcode(const QString str)
{
    m_barcode = str;
    i_valuesMap.insert("barcode", str);
}

void SRepairModel::genBarcode()
{
    // TODO: генерация правильного значения штрих-кода (в АСЦ неправильный)
    setBarcode("01200" + QString::number(i_id).rightJustified(6, '0') + "2");
}

QString SRepairModel::rejectReason()
{
    return m_rejectReason;
}

void SRepairModel::setRejectReason(const QString str)
{
    i_valuesMap.insert("reject_reason", str);
}

int SRepairModel::informedStatusIndex()
{
    return notifyStatusesModel->rowByDatabaseID(m_informedStatus);
}

void SRepairModel::setInformedStatusIndex(const int index)
{
    m_informedStatus = index;
    i_valuesMap.insert("informed_status", m_informedStatus);
    appendLogText(tr("Статус информирования клиента изменён на \"%1\"").arg(notifyStatusesModel->getDisplayRole(m_informedStatus)));
}

QString SRepairModel::imageIds()
{
    return m_imageIds;
}

void SRepairModel::setImageIds(const QString str)
{
    i_valuesMap.insert("image_ids", str);
}

QString SRepairModel::color()
{
    return m_color;
}

void SRepairModel::setColor(const QColor color)
{
    i_valuesMap.insert("color", color);
}

QString SRepairModel::orderMoving()
{
    return m_orderMoving;
}

void SRepairModel::setOrderMoving(const QString str)
{
    i_valuesMap.insert("order_moving", str);
}

int SRepairModel::early()
{
    return m_early;
}

void SRepairModel::setEarly(const int id)
{
    i_valuesMap.insert("early", id);
}

QString SRepairModel::extEarly()
{
    return m_extEarly;
}

void SRepairModel::setExtEarly(const QString str)
{
    i_valuesMap.insert("ext_early", str);
}

QString SRepairModel::issuedMsg()
{
    return m_issuedMsg;
}

void SRepairModel::setIssuedMsg(const QString str)
{
    i_valuesMap.insert("issued_msg", str);
}

bool SRepairModel::smsInform()
{
    return m_smsInform;
}

void SRepairModel::setSmsInform(const bool state)
{
    i_valuesMap.insert("sms_inform", state);
}

int SRepairModel::invoice()
{
    return m_invoice;
}

void SRepairModel::setInvoice(const int id)
{
    i_valuesMap.insert("invoice", id);
}

SCartridgeRepairModel *SRepairModel::cartridge()
{
    return m_cartridgeRepair;
}

void SRepairModel::setCartridge(SCartridgeRepairModel *model)
{
     m_cartridgeRepair = model;
}

bool SRepairModel::termsControl()
{
    return m_termsControl;
}

void SRepairModel::setTermsControl(const bool state)
{
    i_valuesMap.insert("termsControl", state);
}

bool SRepairModel::commitCartridge()
{
    if(!m_cartridgeRepair)
        return 1;

    bool nErr = 1;

    nErr = m_cartridgeRepair->commit();

    i_valuesMap.insert("cartridge", m_cartridgeRepair->id());

    return nErr;
}

void SRepairModel::updateLogAssociatedRecId()
{
    i_logRecord->setRepairId(i_id);
}

bool SRepairModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        commitCartridge();

        setDiagnosticResult(QVariant());
        setRejectReason("");
        setInDate(QDateTime::currentDateTime());
        setState(Global::RepStateIds::GetIn);
        if(!i_valuesMap.contains("company"))
            i_valuesMap.insert("company", userDbData->company);
        if(!i_valuesMap.contains("office"))
            i_valuesMap.insert("office", userDbData->currentOffice);
        insert();
        appendLogText(tr("Устройство принято в ремонт №%1").arg(i_id), "!");
        m_repairStatusLog->setStatus(Global::RepStateIds::GetIn);
        m_repairStatusLog->setRepair(i_id);

        genBarcode();
        commit();
    }
    m_repairStatusLog->commit();

    commitLogs();

    return i_nErr;
}

bool SRepairModel::lock(bool state)
{
    blockSignals(true); // в данном случае отправлять сигнал об обновлении модели не нужно
    try
    {
        QUERY_EXEC_TH(i_query,1,QUERY_BEGIN);
        if(state)
        {
            setUserLock(userDbData->id);
            setLockDatetime(QDateTime::currentDateTime());
        }
        else
        {
            setUserLock(QVariant());
            setLockDatetime(QVariant());
        }
        i_nErr = commit();
        QUERY_COMMIT_ROLLBACK(i_query, i_nErr);
    }
    catch (Global::ThrowType type)
    {
        i_nErr = 0;
        if(type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(i_query, i_nErr);
        }
    }
    blockSignals(false);
    return i_nErr;
}

bool SRepairModel::isLock()
{
    QUERY_EXEC(i_query, i_nErr)(QUERY_SEL_REPAIR_LOCK(QSqlDatabase::database("connMain").databaseName(), i_id));
    i_query->first();
    if(i_query->value(0).toInt() == userDbData->id) // имеется запись о блокировке, но пользователь совпадает
        return 0;

    if(!i_query->value(1).toInt()) // пользователь не совпадает, но он оффлайн
        return 0;

    if(localDateTime(i_query->value(2).toDateTime()).secsTo(QDateTime::currentDateTime()) > (lockTimeout() + 3))   // время блокировки плюс некоторый запас на отправку запроса и получение данных
        return 0;

    return 1;
}

QString SRepairModel::devClass()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QString SRepairModel::vendor()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QString SRepairModel::device()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QString SRepairModel::company()
{
    return companiesModel->getDisplayRole(m_company);
}

QString SRepairModel::officeStr()
{
    return officesModel->getDisplayRole(m_office);
}

QString SRepairModel::startOfficeStr()
{
    return officesModel->getDisplayRole(m_startOffice);
}

QString SRepairModel::managerFioShort()
{
    return "TODO: implement this";  // нужна модель данных
}

QString SRepairModel::currentManagerFioShort()
{
    return "TODO: implement this";  // нужна модель данных
}

QString SRepairModel::engineerFioShort()
{
    return "TODO: implement this";  // нужна модель данных
}

QString SRepairModel::box()
{
    return repairBoxesModel->getDisplayRole(m_box);
}

QString SRepairModel::prepaidTypeStr()
{
    return "TODO: implement this";  // нужна модель данных; такая используется в классе tabRepairNew
}

QString SRepairModel::prepaidSummStr()
{
    return sysLocale.toCurrencyString(prepaidSumm());
}

QString SRepairModel::realPrepaidSummStr()
{
    return sysLocale.toCurrencyString(paymentsAmount());
}

QString SRepairModel::preAgreedAmountStr()
{
    return sysLocale.toCurrencyString(preAgreedAmount());
}

QString SRepairModel::repairCostStr()
{
    return sysLocale.toCurrencyString(repairCost());
}

QString SRepairModel::realRepairCostStr()
{
    return sysLocale.toCurrencyString(realRepairCost());
}

QString SRepairModel::realRepairCostInWords()
{
    return amountToWords(realRepairCost(), comSettings->currencyId);
}

QString SRepairModel::partsCostStr()
{
    return sysLocale.toCurrencyString(partsCost());
}

QString SRepairModel::realWorksCostStr()
{
    return sysLocale.toCurrencyString(realWorksCost());
}

QString SRepairModel::realPartsCostStr()
{
    return sysLocale.toCurrencyString(realPartsCost());
}

QString SRepairModel::warranty()
{
    return warrantyTermsMap->value(m_warrantyDays);
}


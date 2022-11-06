#include "srepairmodel.h"

SRepairModel::SRepairModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "client" << "type" << "maker" << "office" << "manager" << "diagnostic_result" << "in_date" << "fault" << "reject_reason" << "company" << "start_office" << "current_manager" << "master";
    i_tableName = "workshop";
    m_clientModel = new SClientModel();
}

SRepairModel::SRepairModel(const int repair, QObject *parent) : SRepairModel(parent)
{
    load(repair);
}

SRepairModel::~SRepairModel()
{
    delete m_clientModel;
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
    if(!id)
        return;

    QSqlQuery *repair = new QSqlQuery(QSqlDatabase::database("connMain"));
    repair->exec(QUERY_SEL_REPAIR_RPRT(id));
    if(!repair->first())
        return;

    i_id = id;

    m_isHidden = repair->value("Hidden").toBool();
    m_title = repair->value("Title").toString();
    m_clientId = repair->value("client").toInt();
    m_type = repair->value("type").toInt();
    m_maker = repair->value("maker").toInt();
    m_model = repair->value("model").toInt();
    m_serialNumber = repair->value("serial_number").toString();
    m_company = repair->value("company").toInt();
    m_office = repair->value("office").toInt();
    m_startOffice = repair->value("start_office").toInt();
    m_manager = repair->value("manager").toInt();
    m_currentManager = repair->value("current_manager").toInt();
    m_master = repair->value("master").toInt();
    m_diagnosticResult = repair->value("diagnostic_result").toString();
    i_createdUtc = repair->value("in_date").toDateTime();
    m_outDate = repair->value("out_date").toDateTime();
    m_state = repair->value("state").toInt();
    m_newState = repair->value("new_state").toInt();
    m_userLock = repair->value("user_lock").toInt();
    m_lockDatetime = repair->value("lock_datetime").toDateTime();
    m_expressRepair = repair->value("express_repair").toBool();
    m_quickRepair = repair->value("quick_repair").toBool();
    m_isWarranty = repair->value("is_warranty").toBool();
    m_isRepeat = repair->value("is_repeat").toBool();
    m_paymentSystem = repair->value("payment_system").toInt();
    m_isCardPayment = repair->value("is_card_payment").toBool();
    m_canFormat = repair->value("can_format").toBool();
    m_printCheck = repair->value("print_check").toBool();
    m_box = repair->value("box").toInt();
    m_warrantyLabel = repair->value("warranty_label").toString();
    m_extNotes = repair->value("ext_notes").toString();
    m_isPrepaid = repair->value("is_prepaid").toBool();
    m_prepaidType = repair->value("prepaid_type").toInt();
    m_prepaidSumm = repair->value("prepaid_summ").toFloat();
    m_prepaidOrder = repair->value("prepaid_order").toInt();
    m_isPreAgreed = repair->value("is_pre_agreed").toBool();
    m_isDebt = repair->value("is_debt").toBool();
    m_preAgreedAmount = repair->value("pre_agreed_amount").toFloat();
    m_repairCost = repair->value("repair_cost").toFloat();
    m_realRepairCost = repair->value("real_repair_cost").toFloat();
    m_partsCost = repair->value("parts_cost").toFloat();
    m_fault = repair->value("fault").toString();
    m_complect = repair->value("complect").toString();
    m_look = repair->value("look").toString();
    m_thirsPartySc = repair->value("thirs_party_sc").toBool();
    m_lastSave = repair->value("last_save").toDateTime();
    m_lastStatusChanged = repair->value("last_status_changed").toDateTime();
    m_warrantyDays = repair->value("warranty_days").toInt();
    m_barcode = repair->value("barcode").toString();
    m_rejectReason = repair->value("reject_reason").toString();
    m_informedStatus = repair->value("informed_status").toInt();
    m_imageIds = repair->value("image_ids").toString();
    m_color = repair->value("color").toString();
    m_orderMoving = repair->value("order_moving").toString();
    m_early = repair->value("early").toInt();
    m_extEarly = repair->value("ext_early").toString();
    m_issuedMsg = repair->value("issued_msg").toString();
    m_smsInform = repair->value("sms_inform").toBool();
    m_invoice = repair->value("invoice").toInt();
    m_cartridge = repair->value("cartridge").toInt();
    m_vendorId = repair->value("vendor_id").toInt();
    m_termsControl = repair->value("termsControl").toBool();

    delete repair;
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
    i_valuesMap.insert("Title", str);
}

int SRepairModel::clientId()
{
    return m_clientId;
}

void SRepairModel::setClientId(const int id)
{
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
}

void SRepairModel::setManagerIndex(const int index)
{
    if(index == -1)
        return;
    i_valuesMap.insert("manager", managersModel->databaseIDByRow(index));
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
}

void SRepairModel::setCurrentManagerIndex(const int index)
{
    if(index == -1)
        return;
    i_valuesMap.insert("current_manager", managersModel->databaseIDByRow(index));
}

int SRepairModel::engineer()
{
    return m_master;
}

int SRepairModel::engineerIndex()
{
    return engineersModel->rowByDatabaseID(m_master);
}

void SRepairModel::setEngineerIndex(const int index)
{
    if(index == -1)
        i_valuesMap.insert("master", QVariant());   // NULL DEFAULT '0' | FOREIGN KEY (`master`) REFERENCES `users` (`id`)
    else
        i_valuesMap.insert("master", engineersModel->databaseIDByRow(index));
}

QString SRepairModel::diagnosticResult()
{
    return m_diagnosticResult;
}

void SRepairModel::setDiagnosticResult(const QString str)
{
    m_diagnosticResult = str;
    i_valuesMap.insert("diagnostic_result", m_diagnosticResult);
    appendLogText(tr("Результат диагностики изменён на \"%1\"").arg(m_diagnosticResult));
}

QDateTime SRepairModel::inDate()
{
    return m_inDate;
}

void SRepairModel::setInDate(const QDateTime timestamp)
{
    i_valuesMap.insert("in_date", timestamp);
}

QDateTime SRepairModel::outDateUtc()
{
    return m_outDate;
}

QString SRepairModel::outDate()
{
    QDateTime date = m_outDate;
    date.setTimeZone(QTimeZone::utc());

    return date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss");
}

void SRepairModel::setOutDate(const QDateTime timestamp)
{
    i_valuesMap.insert("out_date", timestamp);
}

int SRepairModel::state()
{
    return m_state;
}

int SRepairModel::stateIndex()
{
    return statusesModel->rowByDatabaseID(m_state);
}

void SRepairModel::setState(const int id)
{
    m_state = id;
    i_valuesMap.insert("state", m_state);
    appendLogText(tr("Статус заказа изменён на \"%1\"").arg(statusesModel->getDisplayRole(m_state)));
}

void SRepairModel::setStateIndex(const int index)
{
    setState(statusesModel->databaseIDByRow(index));
}

int SRepairModel::newStateIndex()
{
    return statusesModel->rowByDatabaseID(m_newState);
}

void SRepairModel::setNewStateIndex(const int index)
{
    i_valuesMap.insert("new_state", statusesModel->databaseIDByRow(index));
}

int SRepairModel::userLock()
{
    return m_userLock;
}

void SRepairModel::setUserLock(const int id)
{
    i_valuesMap.insert("user_lock", id);
}

QDateTime SRepairModel::lockDatetime()
{
    return m_lockDatetime;
}

void SRepairModel::setLockDatetime(const QDateTime timestamp)
{
    i_valuesMap.insert("lock_datetime", timestamp);
}

bool SRepairModel::expressRepair()
{
    return m_expressRepair;
}

void SRepairModel::setExpressRepair(const bool state)
{
    i_valuesMap.insert("express_repair", state);
}

bool SRepairModel::quickRepair()
{
    return m_quickRepair;
}

void SRepairModel::setQuickRepair(const bool state)
{
    i_valuesMap.insert("quick_repair", state);
}

bool SRepairModel::isWarranty()
{
    return m_isWarranty;
}

void SRepairModel::setIsWarranty(const bool state)
{
    i_valuesMap.insert("is_warranty", state);
}

bool SRepairModel::isRepeat()
{
    return m_isRepeat;
}

void SRepairModel::setIsRepeat(const bool state)
{
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
    i_valuesMap.insert("box", repairBoxesModel->databaseIDByRow(index));
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

float SRepairModel::prepaidSumm()
{
    return m_prepaidSumm;
}

void SRepairModel::setPrepaidSumm(const float summ)
{
    i_valuesMap.insert("prepaid_summ", summ);
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
void SRepairModel::addPrepay(float amount, QString reason)
{
    QString logText;
    float new_prepaidSumm = m_prepaidSumm + (amount);
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

float SRepairModel::preAgreedAmount()
{
    return m_preAgreedAmount;
}

void SRepairModel::setPreAgreedAmount(const float summ)
{
    i_valuesMap.insert("pre_agreed_amount", summ);
}

float SRepairModel::repairCost()
{
    return m_repairCost;
}

void SRepairModel::setRepairCost(const float amount)
{
    m_repairCost = amount;
    i_valuesMap.insert("repair_cost", m_repairCost);
    appendLogText(tr("Стоимость ремонта в результатах диагностики изменёна на %1").arg(sysLocale.toCurrencyString(m_repairCost)));
}

float SRepairModel::realRepairCost()
{
    return m_realRepairCost;
}

void SRepairModel::setRealRepairCost(const float summ)
{
    i_valuesMap.insert("real_repair_cost", summ);
}

float SRepairModel::partsCost()
{
    return m_partsCost;
}

void SRepairModel::setPartsCost(const float summ)
{
    i_valuesMap.insert("parts_cost", summ);
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
    i_valuesMap.insert("barcode", str);
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

int SRepairModel::cartridge()
{
    return m_cartridge;
}

void SRepairModel::setCartridge(const int id)
{
    i_valuesMap.insert("cartridge", id);
}

bool SRepairModel::termsControl()
{
    return m_termsControl;
}

void SRepairModel::setTermsControl(const bool state)
{
    i_valuesMap.insert("termsControl", state);
}

bool SRepairModel::commit()
{
    if(i_id)
    {
        if(!update())
            throw 1;
    }
    else
    {
        setDiagnosticResult("");
        setRejectReason("");
        setInDate(QDateTime::currentDateTime());
        if(!insert())
            throw 1;
        appendLogText(tr("Устройство принято в ремонт №%1").arg(i_id));
    }

    i_logRecord->setRepairId(i_id);
    commitLogs();

    return i_nErr;
}

bool SRepairModel::lock(bool state)
{
    i_query->exec(QUERY_BEGIN);
    if(state)
    {
        QUERY_EXEC(i_query, i_nErr)(QUERY_LOCK_REPAIR(i_id,userDbData->value("id").toString()));
    }
    else
    {
        QUERY_EXEC(i_query, i_nErr)(QUERY_UNLOCK_REPAIR(i_id));
    }
    QUERY_COMMIT_ROLLBACK(i_query, i_nErr);
    return i_nErr;
}

bool SRepairModel::isLock()
{
    QUERY_EXEC(i_query, i_nErr)(QUERY_SEL_REPAIR_LOCK(i_id));
    i_query->first();
    if(i_query->value(0).toInt() == 0 || i_query->value(0).toInt() == userDbData->value("id").toInt())
        return 0;
    return 1;
}


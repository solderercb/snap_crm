#include "srepairmodel.h"

SRepairModel::SRepairModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "client" << "type" << "maker" << "office" << "manager" << "diagnostic_result" << "in_date" << "fault" << "reject_reason";
    tableName = "workshop";
    clientModel = new SClientModel();
}

SRepairModel::SRepairModel(const int repair, QObject *parent) : SRepairModel(parent)
{
    load(repair);
}

SRepairModel::~SRepairModel()
{

}

void SRepairModel::load(const int)
{

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

int SRepairModel::classId()
{
    return m_typeIndex;
}

void SRepairModel::setClassId(const int index)
{
    i_valuesMap.insert("type", index);
}

int SRepairModel::vendorId()
{
    return m_makerIndex;
}

void SRepairModel::setVendorId(const int index)
{
    i_valuesMap.insert("maker", index);
}

int SRepairModel::deviceId()
{
    return m_modelIndex;
}

void SRepairModel::setDeviceId(const int index)
{
    i_valuesMap.insert("model", index);
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
    return m_companyIndex;
}

void SRepairModel::setCompanyIndex(const int index)
{
    i_valuesMap.insert("company", index);
}

int SRepairModel::officeIndex()
{
    return m_officeIndex;
}

void SRepairModel::setOfficeIndex(const int index)
{
    i_valuesMap.insert("office", index);
}

int SRepairModel::startOfficeIndex()
{
    return m_startOfficeIndex;
}

void SRepairModel::setStartOfficeIndex(const int index)
{
    i_valuesMap.insert("start_office", index);
}

int SRepairModel::managerIndex()
{
    return m_managerIndex;
}

void SRepairModel::setManagerIndex(const int index)
{
    i_valuesMap.insert("manager", index);
}

int SRepairModel::currentManagerIndex()
{
    return m_currentManagerIndex;
}

void SRepairModel::setCurrentManagerIndex(const int index)
{
    i_valuesMap.insert("current_manager", index);
}

int SRepairModel::masterIndex()
{
    return m_masterIndex;
}

void SRepairModel::setMasterIndex(const int index)
{
    i_valuesMap.insert("master", index);
}

QString SRepairModel::diagnosticResult()
{
    return m_diagnosticResult;
}

void SRepairModel::setDiagnosticResult(const QString str)
{
    i_valuesMap.insert("diagnostic_result", str);
}

QDate SRepairModel::inDate()
{
    return m_inDate;
}

void SRepairModel::setInDate(const QDate date)
{
    i_valuesMap.insert("in_date", date);
}

QDate SRepairModel::outDate()
{
    return m_outDate;
}

void SRepairModel::setOutDate(const QDate date)
{
    i_valuesMap.insert("out_date", date);
}

int SRepairModel::stateIndex()
{
    return m_stateIndex;
}

void SRepairModel::setStateIndex(const int index)
{
    i_valuesMap.insert("state", index);
}

int SRepairModel::newStateIndex()
{
    return m_newStateIndex;
}

void SRepairModel::setNewStateIndex(const int index)
{
    i_valuesMap.insert("new_state", index);
}

int SRepairModel::userLock()
{
    return m_userLock;
}

void SRepairModel::setUserLock(const int id)
{
    i_valuesMap.insert("user_lock", id);
}

QDate SRepairModel::lockDatetime()
{
    return m_lockDatetime;
}

void SRepairModel::setLockDatetime(const QDate date)
{
    i_valuesMap.insert("lock_datetime", date);
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

int SRepairModel::paymentSystemIndex()
{
    return m_paymentSystemIndex;
}

void SRepairModel::setPaymentSystemIndex(const int index)
{
    i_valuesMap.insert("payment_system", index);
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
    return m_boxIndex;
}

void SRepairModel::setBoxIndex(const int index)
{
    i_valuesMap.insert("box", index);
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

void SRepairModel::setRepairCost(const float summ)
{
    i_valuesMap.insert("repair_cost", summ);
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

QDate SRepairModel::lastSave()
{
    return m_lastSave;
}

void SRepairModel::setLastSave(const QDate date)
{
    i_valuesMap.insert("last_save", date);
}

QDate SRepairModel::lastStatusChanged()
{
    return m_lastStatusChanged;
}

void SRepairModel::setLastStatusChanged(const QDate date)
{
    i_valuesMap.insert("last_status_changed", date);
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
    return m_informedStatusIndex;
}

void SRepairModel::setInformedStatusIndex(const int index)
{
    i_valuesMap.insert("informed_status", index);
}

QString SRepairModel::imageIds()
{
    return m_imageIds;
}

void SRepairModel::setImageIds(const QString str)
{
    i_valuesMap.insert("image_ids", str);
}

QColor SRepairModel::color()
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


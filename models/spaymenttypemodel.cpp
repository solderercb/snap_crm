#include "spaymenttypemodel.h"

SPaymentTypeModel::SPaymentTypeModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "payment_types";
    i_obligatoryFields << "client" << "reason" << "payment_system";
    i_logRecord->setGroup(SLogRecordModel::ACP);
//    i_logRecord->setType(SLogRecordModel::PaymentOperation);  // TODO: В АСЦ v3.7.31.1123 записи в журнал не производятся
}

SPaymentTypeModel::~SPaymentTypeModel()
{

}

int SPaymentTypeModel::id()
{
    return i_id;
}

void SPaymentTypeModel::setId(const int id)
{
    i_valuesMap.insert("id", id);
}

void SPaymentTypeModel::load()
{
    QSqlQuery query(QSqlDatabase::database("connMain"));
    query.exec(QUERY_SEL_PAYMENT_TYPE(i_id));
    if(!query.isValid())
        return;

    query.first();
    m_type = query.value("type").toInt();
    m_name = query.value("name").toString();
    m_client = query.value("client").toInt();
    m_periodic = query.value("periodic").toBool();
    m_payDate = query.value("pay_date").toDateTime();
    m_defSumm = query.value("def_summ").toFloat();
    m_reason = query.value("reason").toString();
    m_isArchive = query.value("is_archive").toBool();
    m_paymentSystem = query.value("payment_system").toInt();
    m_updateTime = query.value("updated_at").toDateTime();
}

void SPaymentTypeModel::load(const int id)
{
    setId(id);
    load();
}

int SPaymentTypeModel::type()
{
    return m_type;
}

void SPaymentTypeModel::setType(const int type)
{
    if(i_id)
        appendLogText(tr("Тип операции изменён с %1 на %2").arg(m_type, type));
    i_valuesMap.insert("type", type);
}

QString SPaymentTypeModel::name()
{
    return m_name;
}

void SPaymentTypeModel::setName(const QString& name)
{
    if(i_id)
        appendLogText(tr("Название операции изменено с %1 на %2").arg(m_name, name));
    i_valuesMap.insert("name", name);
}

int SPaymentTypeModel::client()
{
    return m_client;
}

void SPaymentTypeModel::setClient(const int client)
{
    if(i_id)
        appendLogText(tr("Контрагент изменён с %1 на %2").arg(m_client, client));
    i_valuesMap.insert("client", client);
}

bool SPaymentTypeModel::periodic()
{
    return m_periodic;
}

void SPaymentTypeModel::setPeriodic(const bool periodic)
{
    if(i_id)
    {
        if(periodic)
            appendLogText(tr("Включен режим периодичности операции"));
        else
            appendLogText(tr("Режим периодичности операции выключен"));
    }
    i_valuesMap.insert("periodic", periodic);
}

QDateTime SPaymentTypeModel::payDate()
{
    return m_payDate;
}

void SPaymentTypeModel::setPayDate(const QDateTime payDate)
{
    if(i_id)
        appendLogText(tr("Дата периодического выполнения операции изменена с %1 на %2").arg(m_payDate.toString("dd MMM yyyy"), payDate.toString("dd MMM yyyy")));
    i_valuesMap.insert("pay_date", payDate);
}

float SPaymentTypeModel::defSumm()
{
    return m_defSumm;
}

void SPaymentTypeModel::setDefSumm(const float defSumm)
{
    if(i_id)
        appendLogText(tr("Сумма по умолчанию изменена с %1 на %2").arg(sysLocale.toString(m_defSumm, 'f', 2), sysLocale.toString(defSumm, 'f', 2)));
    i_valuesMap.insert("def_summ", defSumm);
}

QString SPaymentTypeModel::reason()
{
    return m_reason;
}

void SPaymentTypeModel::setReason(const QString& reason)
{
    if(i_id)
        appendLogText(tr("Назначение платежа по умолчнию изменено с \"%1\" на \"%2\"").arg(m_reason, reason));
    i_valuesMap.insert("reason", reason);
}

bool SPaymentTypeModel::isArchive()
{
    return m_isArchive;
}

void SPaymentTypeModel::markArchive(const bool isArchive)
{
    if(i_id)
    {
        if(isArchive)
            appendLogText(tr("Операция помечена как архивная"));
        else
            appendLogText(tr("Метка \"архивная\" снята с операции"));
    }
    i_valuesMap.insert("is_archive", isArchive);
}

int SPaymentTypeModel::paymentSystem()
{
    return m_paymentSystem;
}

int SPaymentTypeModel::paymentSystemIndex()
{
    return paymentSystemsModel->rowByDatabaseID(m_paymentSystem);
}

void SPaymentTypeModel::setPaymentSystem(const int paymentSystem)
{
    if(i_id)
        appendLogText(tr("Платёжная система по умолчанию изменена с \"%1\" на \"%2\"").arg(paymentSystemsModel->getDisplayRole(m_paymentSystem, "system_id"), paymentSystemsModel->getDisplayRole(paymentSystem, "system_id")));
    i_valuesMap.insert("payment_system", paymentSystem);
}

bool SPaymentTypeModel::commit()
{
    i_valuesMap.insert("updated_at", QDateTime::currentDateTime());
    if(i_id)
    {
        update();
    }
    else
    {
        insert();
    }
//    commitLogs(); // TODO: В АСЦ v3.7.31.1123 записи в журнал не производятся

    return i_nErr;
}


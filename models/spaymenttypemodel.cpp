#include "spaymenttypemodel.h"
#include <QSqlQuery>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SLogRecordModel>
#include <SSqlQueryModel>

SPaymentTypeModel::SPaymentTypeModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "payment_types";
    i_obligatoryFields << "client" << "reason" << "payment_system";
    setPrimaryKeyIndex(0);
    i_logRecord->set_group(SLogRecordModel::ACP);
//    i_logRecord->setType(SLogRecordModel::PaymentOperation);  // TODO: В АСЦ v3.7.31.1123 записи в журнал не производятся
}

SPaymentTypeModel::~SPaymentTypeModel()
{

}

void SPaymentTypeModel::load(const int id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();
}

int SPaymentTypeModel::accountIndex()
{
    return paymentSystemsModel->rowByDatabaseID(account());
}

bool SPaymentTypeModel::commit()
{
    bool nErr = 1;
    set_updated(QDateTime::currentDateTime());

#ifdef QT_DEBUG
    checkTableName();
#endif

    if(isPrimaryKeyValid())
    {
        nErr = update();
    }
    else
    {
        nErr = insert();
    }
    setFieldsExecuted();
//    commitLogs(); // TODO: В АСЦ v3.7.31.1123 записи в журнал не производятся

    return nErr;
}

void SPaymentTypeModel::typeChanged(const int type)
{
    auto old = commitedData(C_type).value_or(QVariant());
    if(old.isValid())
        appendLogText(tr("Тип операции изменён с %1 на %2").arg(old.toInt(), type));
}

void SPaymentTypeModel::nameChanged(const QString name)
{
    auto old = commitedData(C_name).value_or(QVariant());
    if(old.isValid())
        appendLogText(tr("Название операции изменено с %1 на %2").arg(old.toString(), name));
}

void SPaymentTypeModel::clientChanged(const int id)
{
    auto old = commitedData(C_client).value_or(QVariant());
    if(old.isValid())
    {
        appendLogText(tr("Контрагент изменён с %1 на %2").arg(old.toInt(), id));    // TODO: заменить id на SClientModel::fullShortName()
    }
}

void SPaymentTypeModel::periodicStateChanged(const bool state)
{
    if(state)
        appendLogText(tr("Включен режим периодичности операции"));
    else
        appendLogText(tr("Режим периодичности операции выключен"));
}

void SPaymentTypeModel::periodicDateChanged(const QDateTime dateTime)
{
    auto old = commitedData(C_pariodicDate).value_or(QVariant());
    if(old.isValid())
    {
        appendLogText(tr("Дата периодического выполнения операции изменена с %1 на %2").arg(old.toDateTime().toString("dd MMM yyyy"), dateTime.toString("dd MMM yyyy")));
    }
}

void SPaymentTypeModel::defAmountChanged(const double amount)
{
    auto old = commitedData(C_defAmount).value_or(QVariant());
    if(old.isValid())
    {
        appendLogText(tr("Сумма по умолчанию изменена с %1 на %2").arg(sysLocale.toString(old.toDouble(), 'f', 2), sysLocale.toString(amount, 'f', 2)));
    }
}

void SPaymentTypeModel::reasonChanged(const QString reason)
{
    auto old = commitedData(C_reason).value_or(QVariant());
    if(old.isValid())
        appendLogText(tr("Назначение платежа по умолчнию изменено с \"%1\" на \"%2\"").arg(old.toString(), reason));
}

void SPaymentTypeModel::isArchiveChanged(const bool isArchive)
{
    if(isArchive)
        appendLogText(tr("Операция помечена как архивная"));
    else
        appendLogText(tr("Метка \"архивная\" снята с операции"));
}

void SPaymentTypeModel::accountChanged(const int id)
{
    auto old = commitedData(C_account).value_or(QVariant());
    if(old.isValid())
        appendLogText(tr("Платёжная система по умолчанию изменена с \"%1\" на \"%2\"").arg(paymentSystemsModel->getDisplayRole(old.toInt(), "system_id"), paymentSystemsModel->getDisplayRole(id, "system_id")));
}

void SPaymentTypeModel::logDataChange(const int index, const QVariant &data)
{
    if(!isPrimaryKeyValid())
        return;

    switch (index)
    {
        case C_type: typeChanged(data.toInt()); break;
        case C_name: nameChanged(data.toString()); break;
        case C_client: clientChanged(data.toInt()); break;
        case C_periodicEnabled: periodicStateChanged(data.toBool()); break;
        case C_pariodicDate: periodicDateChanged(data.toDateTime()); break;
        case C_defAmount: defAmountChanged(data.toDouble()); break;
        case C_reason: reasonChanged(data.toString()); break;
        case C_isArchive: isArchiveChanged(data.toBool()); break;
        case C_account: accountChanged(data.toInt()); break;
        default: break;
    }
}

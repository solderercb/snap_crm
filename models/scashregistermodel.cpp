#include "scashregistermodel.h"
#include <SAmountToWords>
#include <SUserSettings>
#include <SPaymentTypesModel>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SSqlQueryModel>
#include <SLogRecordModel>

SCashRegisterModel::SCashRegisterModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "created" << "type" << "summa" << "user" << "company" << "office" << "notes";
    i_tableName = "cash_orders";
    setPrimaryKeyIndex(0);
    connect(this, &SSingleRowModel::beginDataChange, this, &SCashRegisterModel::setDataRework);
}

SCashRegisterModel::SCashRegisterModel(int systemId, QObject *parent) :
    SCashRegisterModel(parent)
{
    set_systemId(systemId);
}

SCashRegisterModel::~SCashRegisterModel()
{
}

void SCashRegisterModel::load()
{
    SSingleRowJModel::load();

    // TODO: объект класса pinpad_logs; `cash_orders`.`card_info` <=> `pinpad_logs`.`id`
    //       объект класса images
}

void SCashRegisterModel::load(int id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();
}

// демо-данные (например, для отчетов)
void SCashRegisterModel::initDemo()
{
    m_record->setValue(C_id, 123456);
    m_record->setValue(C_created, QDateTime::currentDateTimeUtc());
    m_record->setValue(C_operationType, RecptSimple);
    m_record->setValue(C_amount, 1024);
    m_record->setValue(C_amountStr, amountToWords(amountAbs()));
    m_record->setValue(C_invoice, 0);
    m_record->setValue(C_client, 0);
    m_record->setValue(C_employee, usersModel->index(0, 1).data().toInt());
    m_record->setValue(C_user, usersModel->index(0, 1).data().toInt());
    m_record->setValue(C_company, 1);
    m_record->setValue(C_office, 1);
    m_record->setValue(C_reason, tr("Поступление денег в размере %1").arg(sysLocale.toCurrencyString(amount())));
    m_record->setValue(C_repair, 0);
    m_record->setValue(C_document, 0);
    m_record->setValue(C_imgId, 0);
    m_record->setValue(C_systemId, paymentSystemsModel->index(0, 1).data().toInt());
    m_record->setValue(C_cardFee, 123.45);
    m_record->setValue(C_isBackdate, 0);
    m_record->setValue(C_cardInfo, 0);
    m_record->setValue(C_fiscalDocNum, 9999);
    m_record->setValue(C_paymentItemSign, 0);
}

/*  Создание новой записи о кассовой операции в БД
 *  возвращает 1 в случае успеха
 */
bool SCashRegisterModel::commit()
{
    bool nErr = 1;
    if(isPrimaryKeyValid())
    {
        update();
    }
    else
    {
        if(amount() == 0)
            return 1;

        QSqlQuery query(QSqlDatabase::database("connThird"));
        QString q;

        initMandatoryField(C_user, userDbData->id());
        initMandatoryField(C_company, userDbData->company());
        initMandatoryField(C_office, userDbData->currentOffice());
        initMandatoryField(C_created, QDateTime::currentDateTime());

        auto fields_verify = fieldsVerifyFormatter();    // вызов этого метода должен быть до вызова метода insert()

        insert();

        // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
        q = "SELECT IF(\n  " + fields_verify.join(" AND\n  ") + "\n, 21930, 0)\n"\
                                                                "FROM `cash_orders`\n"\
                                                                "WHERE `id` = " + QString::number(id()) + ";";
        QUERY_EXEC_TH(&query, nErr, q);
        QUERY_EXEC_VRFY(&query, client());
    }

    if(!nErr)
        Global::throwError(Global::ThrowType::QueryError);

    if(!m_skipLogRecording)
        i_logRecord->commit();

    setFieldsExecuted();

    // TODO: Признак предмета расчета

    return nErr;
}

bool SCashRegisterModel::commit(double amount)
{
    set_amount(amount);
    return commit();
}

QStringList SCashRegisterModel::fieldsVerifyFormatter()
{
    QStringList fields;
    QMap<QString, QVariant>::ConstIterator i;

    foreach (auto field, cache)
    {
        if(field->state() & (SSingleRowModelBase::ModifiedField::None | SSingleRowModelBase::ModifiedField::Executed | SSingleRowModelBase::ModifiedField::Commited))
            continue;

        int index = cache.key(field);
        if(index == C_operationType || index == C_reason)
            continue;   // верификацию этих полей пропускаем, т. к. костыли для АСЦ в виде триггеров могут их изменить и сессия завершится Rollback'ом

        fields.append("IFNULL(`" + rec()->fieldName(index) + "`,'NULL') = IFNULL(" + fieldValueHandler(field->data()) + ",'NULL')");
    }

    return fields;
}

void SCashRegisterModel::unsetClient()
{
    setData(C_client, QVariant());
}

void SCashRegisterModel::reasonChanged(const QString &text)
{
    if(!m_skipLogRecording)
        i_logRecord->set_text(text);
}

void SCashRegisterModel::set_createdDate(const QDate &date)
{
    auto f = cache.constFind(C_created);
    QDateTime timestamp;
    if(f == cache.constEnd())
        timestamp = QDateTime::currentDateTime();
    else
        timestamp = f.value()->data().toDateTime();

    timestamp.setDate(date);
    set_created(timestamp);
}

void SCashRegisterModel::set_createdTime(const QTime &time)
{
    auto f = cache.constFind(C_created);
    QDateTime timestamp;
    if(f == cache.constEnd())
        timestamp = QDateTime::currentDateTime();
    else
        timestamp = f.value()->data().toDateTime();

    timestamp.setTime(time);
    set_created(timestamp);
}

void SCashRegisterModel::documentChanged(int id)
{
    i_logRecord->set_type(SLogRecordModel::Doc);
    i_logRecord->set_document(id);
}

void SCashRegisterModel::repairChanged(int id)
{
    i_logRecord->set_type(SLogRecordModel::Repair);
    i_logRecord->set_repair(id);
}

void SCashRegisterModel::invoiceChanged(int)
{
//    i_logRecord->setType(SLogRecordModel::Invoice);   // В АСЦ v3.7.31.1123 не реализовано
//    i_logRecord->setInvoiceId(m_invoice);
}

/* Установка текста записи в журнале
 * В случае ПН или РН его нельзя задать автоматически
 */
void SCashRegisterModel::set_logText(const QString &text)
{
    i_logRecord->set_text(text);
}

QString SCashRegisterModel::constructReason(const QString &linkedObjId)
{
    int opType = operationType();
    QString amountLocale = sysLocale.toCurrencyString(amountAbs());
    QString reason;
    switch(opType)
    {
        case PaymentType::ExpSimple:    reason = tr("Расход денег в размере %1").arg(amountLocale); break;
        case PaymentType::ExpInvoice:   reason = tr("Оплата приходной накладной №%1, в размере %2").arg(linkedObjId, amountLocale); break;
        case PaymentType::ExpZ:         reason = tr("Z отчёт, выемка средств в размере %1").arg(amountLocale); break;
        case PaymentType::ExpBalance:   reason = tr("Выдача денег в размере %1 со списанием с баланса клиента №%2").arg(amountLocale).arg(client()); break;  // TODO: добавить короткую форму ФИО
        case PaymentType::ExpSubsist:   reason = tr("Выплата аванса сторуднику в размере %1").arg(amountLocale); break;
        case PaymentType::ExpSalary:    reason = tr("Выплата заработной платы сторуднику в размере %1").arg(amountLocale); break;
        case PaymentType::ExpRepair:    reason = tr("Расход денег в размере %1 - возврат за невыполненный ремонт №2").arg(amountLocale, linkedObjId); break;
        case PaymentType::ExpGoods:     reason = tr("Расход денег в размере %1 - возврат за товары по РН №%2").arg(amountLocale, linkedObjId); break;
        case PaymentType::RecptSimple:  reason = tr("Поступление денег в размере %1").arg(amountLocale); break;
        case PaymentType::RecptPrepayRepair: reason = tr("Предоплата за ремонт №%1 в размере %2").arg(linkedObjId, amountLocale); break;
        case PaymentType::RecptBalance: reason = tr("Поступление денег в размере %1 с зачислением на баланс клиента №%2").arg(amountLocale).arg(client()); break;
        case PaymentType::RecptGoods:   reason = tr("Поступление денег в размере %1 по расходной накладной №%2").arg(amountLocale, linkedObjId); break;
        case PaymentType::RecptRepair:  reason = tr("Поступление денег в размере %1 в счёт выполненного(-ых) ремонта(-ов): %2").arg(amountLocale, linkedObjId); break;
        case PaymentType::ExpInvoiceUndo: reason = tr("Поступление средств в рамере %1. за товары в распроведённой ПН №%2").arg(amountLocale, linkedObjId); break;
        case PaymentType::RecptInvoice: reason = tr("Поступление денег в размере %1 по счёту №%2").arg(amountLocale, linkedObjId); break;
        case PaymentType::MoveCash:     reason = tr("Перемещение средств в размере %1; комплементарная операция №%2").arg(amountLocale, linkedObjId); break;
        case PaymentType::ExpDealer:    reason = tr("Расход денег в размере %1 в счёт выплаты поставщику за товары находившиеся на реализации").arg(amountLocale); break;
        case PaymentType::ExpRevert:    reason = tr("Возврат средств в размере %1").arg(amountLocale); break;
    }
    if(opType == PaymentType::AddSubCash)
    {
        if(amount() > 0)
            reason = tr("Поступление денег в размере %1 (%2) - внесение средств в кассу").arg(amountLocale, amountStr());
        else
            reason = tr("Расход денег в размере %1 (%2) - выдача средств из кассы").arg(amountLocale, amountStr());
    }
    else if(opType > PaymentType::ExpCustom)  // TODO: В АСЦ v3.7.31.1123 есть шаблоны только РКО
        reason = tr("Расход денег в размере %1, %2").arg(amountLocale, expenditureTypesModel->reasonByDatabaseId(opType));

    set_reason(reason);

    return reason;
}

QString SCashRegisterModel::constructReason(int linkedObjId)
{
    return constructReason(QString::number(linkedObjId));
}

/*  По умолчанию каждая кассовая операция сопровождается записью в журнал
 *  Но в некоторых случаях запись в журнал может производиться в отдельном методе класса,
 *  использующего объект этого класса, что приведёт к созданию двух записей в журнале.
 *  Например, при создании ПКО предоплаты ремонта. Чтобы этого избежать нужно вызвать
 *  этот метод без параметров или с параметром state = true.
*/
void SCashRegisterModel::set_skipLogRecording(bool state)
{
    m_skipLogRecording = state;
}

void SCashRegisterModel::set_officeIndex(const int index)
{
    set_office(officesModel->databaseIDByRow(index));
}

double SCashRegisterModel::amountAbs()
{
    double a = amount();
    if(a < 0)
        return -a;
    return a;
}

void SCashRegisterModel::amountChanged(double amount)
{
    Q_UNUSED(amount)

    set_amountStr(amountToWords(amountAbs()));
}

void SCashRegisterModel::employeeChanged(const int id)
{
    i_logRecord->set_type(SLogRecordModel::User);
    i_logRecord->set_user(id);
}

int SCashRegisterModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(company(), "id");
}

void SCashRegisterModel::set_companyIndex(const int index)
{
    set_company(companiesModel->databaseIDByRow(index, "id"));
}

void SCashRegisterModel::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_client: if(!data.toInt()) data = QVariant(); break;    // При просмотре записей журнала, касающихся клиента, записи о кассовых операциях отображать не нужно (для этого есть отдельная страница в карточке клиента)
        default: ;
    }
}

void SCashRegisterModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_created: if(data.toDateTime().date() != QDate::currentDate()) set_isBackdate(true); break;  // timestamp - локальное время (не UTC)
        case C_amount: amountChanged(data.toDouble()); break;
        case C_reason: reasonChanged(data.toString()); break;
        case C_employee: employeeChanged(data.toInt()); break;
        case C_document: documentChanged(data.toInt()); break;
        case C_repair: repairChanged(data.toInt()); break;
        case C_invoice: invoiceChanged(data.toInt()); break;
        default: break;
    }
}

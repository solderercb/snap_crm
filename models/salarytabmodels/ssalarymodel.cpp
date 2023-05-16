#include "ssalarymodel.h"

SSalaryModel::SSalaryModel(QObject *parent) : SDatabaseRecord(parent)
{
    i_tableName = "salary";
    i_obligatoryFields << "user_id" << "summ" << "balance" << "payment_date" << "notes" << "period_from" << "period_to" << "from_user" << "type";
}

void SSalaryModel::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString(""));
    if(!record->first())
        return;

    i_id = id;
    load(record->record());
    delete record;
}

void SSalaryModel::load(const QSqlRecord &record)
{
    m_employee = record.value("user_id").toInt();
    m_summ = record.value("summ").toDouble();
    m_balance = record.value("balance").toDouble();
    i_createdUtc = record.value("payment_date").toDateTime();
    m_notes = record.value("notes").toString();
    m_periodFrom = record.value("period_from").toDate();
    m_periodTo = record.value("period_to").toDate();
    m_user = record.value("from_user").toInt();
    m_type = record.value("type").toBool();
}

int SSalaryModel::id()
{
    return i_id;
}

int SSalaryModel::employee()
{
    return m_employee;
}

void SSalaryModel::setEmployee(const int user_id)
{
    i_valuesMap.insert("user_id", user_id);
}

double SSalaryModel::summ()
{
    return m_summ;
}

void SSalaryModel::setSumm(const double summ)
{
    m_summ = summ;
    i_valuesMap.insert("summ", summ);
}

double SSalaryModel::balance()
{
    return m_balance;
}

void SSalaryModel::setBalance(const double balance)
{
    i_valuesMap.insert("balance", balance);
}

QString SSalaryModel::paymentDate()
{
    return created();
}

void SSalaryModel::setPaymentDate(const QDateTime payment_date)
{
    i_valuesMap.insert("payment_date", payment_date);
}

QString SSalaryModel::notes()
{
    return m_notes;
}

void SSalaryModel::setNotes(const QString& notes)
{
    i_valuesMap.insert("notes", notes);
}

QDate SSalaryModel::periodFrom()
{
    return m_periodFrom;
}

void SSalaryModel::setPeriodFrom(const QDateTime period_from)
{
    i_valuesMap.insert("period_from", period_from);
}

QDate SSalaryModel::periodTo()
{
    return m_periodTo;
}

void SSalaryModel::setPeriodTo(const QDateTime period_to)
{
    i_valuesMap.insert("period_to", period_to);
}

int SSalaryModel::user()
{
    return m_user;
}

void SSalaryModel::setUser(const int from_user)
{
    i_valuesMap.insert("from_user", from_user);
}

bool SSalaryModel::type()
{
    return m_type;
}

void SSalaryModel::setType(const bool type)
{
    m_type = type;
    i_valuesMap.insert("type", type);
}

bool SSalaryModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        insert();
    }

    if(!i_nErr)
        throw 1;

    return i_nErr;
}

void SSalaryModel::setBalanceRecord(const int id)
{
    if(!id)
        return;

    i_valuesMap.insert("balance_record", id);
}


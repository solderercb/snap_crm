#include "sadditionalpaymentmodel.h"

SAdditionalPaymentModel::SAdditionalPaymentModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "additional_payments";
    i_obligatoryFields << "name" << "payment_date" << "user" << "to_user" << "price";
    i_idColumnName = "id";
}

int SAdditionalPaymentModel::id()
{
    return i_id;
}

QString SAdditionalPaymentModel::name()
{
    return m_name;
}

void SAdditionalPaymentModel::setName(const QString name)
{
    i_valuesMap.insert("name", name);
}

QDateTime SAdditionalPaymentModel::paymentDate()
{
    return m_paymentDate;
}

void SAdditionalPaymentModel::setPaymentDate(const QDateTime payment_date)
{
    i_valuesMap.insert("payment_date", payment_date);
}

int SAdditionalPaymentModel::user()
{
    return m_user;
}

void SAdditionalPaymentModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

int SAdditionalPaymentModel::employee()
{
    return m_employee;
}

void SAdditionalPaymentModel::setEmployee(const int to_user)
{
    i_valuesMap.insert("to_user", to_user);
}

double SAdditionalPaymentModel::summ()
{
    return m_summ;
}

void SAdditionalPaymentModel::setSumm(const double price)
{
    i_valuesMap.insert("price", price);
}

bool SAdditionalPaymentModel::commit()
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
        throw Global::ThrowType::QueryError;

    return i_nErr;
}


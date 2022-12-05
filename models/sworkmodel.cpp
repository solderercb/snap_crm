#include "sworkmodel.h"

SWorkModel::SWorkModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "works";
    i_obligatoryFields << "user" << "name" << "warranty";
}

SWorkModel::SWorkModel(const QList<QStandardItem *> &record, QObject *parent) :
    SWorkModel(parent)
{
    i_id = record.at(ColId)->data(Qt::DisplayRole).toInt();
    m_user = record.at(ColUser)->data(Qt::DisplayRole).toInt();
    m_repair = record.at(ColRepairId)->data(Qt::DisplayRole).toInt();
    m_documentId = record.at(ColDocumentId)->data(Qt::DisplayRole).toInt();
    m_name = record.at(ColName)->data(Qt::DisplayRole).toString();
    m_price = record.at(ColPrice)->data(Qt::DisplayRole).toFloat();
    m_count = record.at(ColCount)->data(Qt::DisplayRole).toInt();
    m_warranty = record.at(ColWarranty)->data(Qt::DisplayRole).toInt();
    m_priceId = record.at(ColPriceId)->data(Qt::DisplayRole).toInt();
    m_isPay = record.at(ColIsPay)->data(Qt::DisplayRole).toBool();
    i_createdUtc = record.at(ColAdded)->data(Qt::DisplayRole).toDateTime();
    m_type = record.at(ColType)->data(Qt::DisplayRole).toInt();
    m_payRepair = record.at(ColPayRepair)->data(Qt::DisplayRole).toInt();
    m_payRepair_quick = record.at(ColPayRepairQuick)->data(Qt::DisplayRole).toInt();
}

int SWorkModel::id()
{
    return i_id;
}

void SWorkModel::load(const int)
{

}

int SWorkModel::user()
{
    return m_user;
}

void SWorkModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

int SWorkModel::repair()
{
    return m_repair;
}

void SWorkModel::setRepair(const int repair)
{
    i_valuesMap.insert("repair", repair);
}

int SWorkModel::documentId()
{
    return m_documentId;
}

void SWorkModel::setDocumentId(const int document_id)
{
    i_valuesMap.insert("document_id", document_id);
}

QString SWorkModel::name()
{
    return m_name;
}

void SWorkModel::setName(const QString name)
{
    i_valuesMap.insert("name", name);
}

float SWorkModel::price()
{
    return m_price;
}

void SWorkModel::setPrice(const float price)
{
    i_valuesMap.insert("price", price);
}

int SWorkModel::count()
{
    return m_count;
}

void SWorkModel::setCount(const int count)
{
    i_valuesMap.insert("count", count);
}

int SWorkModel::warranty()
{
    return m_warranty;
}

void SWorkModel::setWarranty(const int warranty)
{
    i_valuesMap.insert("warranty", warranty);
}

int SWorkModel::priceId()
{
    return m_priceId;
}

void SWorkModel::setPriceId(const int price_id)
{
    i_valuesMap.insert("price_id", price_id);
}

bool SWorkModel::isPay()
{
    return m_isPay;
}

void SWorkModel::setIsPay(const bool is_pay)
{
    i_valuesMap.insert("is_pay", is_pay);
}

void SWorkModel::setCreated(const QDateTime added)
{
    i_valuesMap.insert("added", added);
}

int SWorkModel::type()
{
    return m_type;
}

void SWorkModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
}

int SWorkModel::payRepair()
{
    return m_payRepair;
}

void SWorkModel::setPayRepair(const int pay_repair)
{
    i_valuesMap.insert("pay_repair", pay_repair);
}

int SWorkModel::payRepairQuick()
{
    return m_payRepair_quick;
}

void SWorkModel::setPayRepairQuick(const int pay_repair_quick)
{
    i_valuesMap.insert("pay_repair_quick", pay_repair_quick);
}

bool SWorkModel::commit()
{
    if(i_id)
        update();
    else
    {
        setCreated(QDateTime::currentDateTime());
        insert();
    }

    return i_nErr;
}


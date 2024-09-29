#include "ssupplier.h"

SPartSupplier::SPartSupplier(QObject *parent)
    : SComRecord{parent}
{
    i_obligatoryFields << "request_id" << "supplier_url" << "item_url";
    i_tableName = "parts_requests_suppliers";
    i_idColumnName = "id";
}

int SPartSupplier::id()
{
    return i_id;
}

void SPartSupplier::load(const int &id)
{
    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("").arg(id));

    if(record->first())
    {
        i_id = record->value("id").toInt();
        m_requestId = record->value("request_id").toInt();
        m_supplierId = record->value("supplier_id").toInt();
        m_supplierUrl = record->value("supplier_url").toString();
        m_itemUrl = record->value("item_url").toString();
        m_moq = record->value("moq").toInt();
        m_price = record->value("price").toDouble();
        m_notes = record->value("notes").toString();
    }

    delete record;
}

int SPartSupplier::requestId()
{
    return m_requestId;
}

void SPartSupplier::setRequestId(const int &id)
{
    i_valuesMap.insert("request_id", id);
}

int SPartSupplier::supplierId()
{
    return m_supplierId;
}

void SPartSupplier::setSupplierId(const int &id)
{
    i_valuesMap.insert("supplier_id", id);
}

QString SPartSupplier::supplierUrl()
{
    return m_supplierUrl;
}

void SPartSupplier::setSupplierUrl(const QString &url)
{
    i_valuesMap.insert("supplier_url", url);
}

QString SPartSupplier::itemUrl()
{
    return m_itemUrl;
}

void SPartSupplier::setItemUrl(const QString &url)
{
    i_valuesMap.insert("item_url", url);
}

int SPartSupplier::moq()
{
    return m_moq;
}

void SPartSupplier::setMoq(const int &moq)
{
    i_valuesMap.insert("moq", moq);
}

double SPartSupplier::price()
{
    return m_price;
}

void SPartSupplier::setPrice(const double &price)
{
    i_valuesMap.insert("price", price);
}

QString SPartSupplier::notes()
{
    return m_notes;
}

void SPartSupplier::setNotes(const QString &notes)
{
    i_valuesMap.insert("notes", notes);
}


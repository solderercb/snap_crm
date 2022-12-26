#include "sworkmodel.h"

SWorkModel::SWorkModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "works";
    i_obligatoryFields << "user" << "name" << "warranty";
    i_logRecord->setType(SLogRecordModel::Repair);
}

SWorkModel::SWorkModel(const QList<QStandardItem *> &record, QObject *parent) :
    SWorkModel(parent)
{
    i_id = record.at(SStoreItemModel::SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();

    m_user = record.at(SStoreItemModel::SaleOpColumns::ColUser)->data(Qt::DisplayRole).toInt();
    m_repair = record.at(SStoreItemModel::SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt();
    m_name = record.at(SStoreItemModel::SaleOpColumns::ColName)->data(Qt::DisplayRole).toString();
    m_price = record.at(SStoreItemModel::SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toFloat();
    m_count = record.at(SStoreItemModel::SaleOpColumns::ColCount)->data(Qt::DisplayRole).toInt();
    m_warranty = record.at(SStoreItemModel::SaleOpColumns::ColWarranty)->data(Qt::DisplayRole).toInt();
    m_priceId = record.at(SStoreItemModel::SaleOpColumns::ColItemId)->data(Qt::DisplayRole).toInt();
    i_createdUtc = record.at(SStoreItemModel::SaleOpColumns::ColCreated)->data(Qt::DisplayRole).toDateTime();
//    m_documentId = record.at(SStoreItemModel::SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt();
//    m_isPay = record.at(SStoreItemModel::SaleOpColumns::)->data(Qt::DisplayRole).toBool();
//    m_type = record.at(SStoreItemModel::SaleOpColumns::Col)->data(Qt::DisplayRole).toInt();

    if(!i_id)
    {
        i_logRecord->setText(tr("Добавлена работа: %1, стоимость %2 из ремонта №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repair));
        i_valuesMap.insert("pay_repair", userDbData->value("pay_repair"));
        i_valuesMap.insert("pay_repair_quick", userDbData->value("pay_repair_quick"));
    }

    for(int i = 1; i < record.count(); i++) // в нулевом столбце id записи в таблице, он не изменяется средствами программы
    {
        if(!record.at(i)->data(Qt::UserRole+1).toBool())
            continue;

        setField(i, record.at(i)->data(Qt::DisplayRole));
        record.at(i)->setData(0, Qt::UserRole+1);   // снятие пометки изменённого поля
    }
    record.at(0)->setData(0, Qt::UserRole+1);   // снятие пометки изменённой строки
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

bool SWorkModel::update()
{

}

bool SWorkModel::remove()
{
    bool ret = 1;

    i_logRecord->setText(tr("Удалена работа: %1, стоимость %2 из ремонта №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repair));
    ret &= del();
    ret &= i_logRecord->commit();

    return ret;
}

void SWorkModel::setField(const int fieldNum, const QVariant value)
{
    switch(fieldNum)
    {
        case SStoreItemModel::ColUser: setUser(value.toInt()); break;
        case SStoreItemModel::ColObjId: setRepair(value.toInt()); break;
        case SStoreItemModel::ColName: setName(value.toString()); break;
        case SStoreItemModel::ColPrice: setPrice(value.toFloat()); break;
        case SStoreItemModel::ColCount: setCount(value.toInt()); break;
        case SStoreItemModel::ColWarranty: setWarranty(value.toInt()); break;
        case SStoreItemModel::ColItemId: setPriceId(value.toInt()); break;
        case SStoreItemModel::ColCreated: setCreated(value.toDateTime()); break;
    }
}

bool SWorkModel::commit()
{
    if(i_id)
        update();
    else
    {
        setUser(userDbData->value("id").toInt());
        setCreated(QDateTime::currentDateTime());
        setPayRepair(userDbData->value("pay_repair").toInt());
        setPayRepairQuick(userDbData->value("pay_repair_quick").toInt());
        insert();
    }
    i_logRecord->commit();

    return i_nErr;
}


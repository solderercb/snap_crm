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

    initQueryFields(record);
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

void SWorkModel::setUser(const int id, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Исполнитель работы изменён с %1 на %2").arg(usersModel->getDisplayRole(oldValue.toInt())).arg(usersModel->getDisplayRole(id)));
    else
        appendLogText(tr("Исполнителем работы назначен %1").arg(usersModel->getDisplayRole(id, 1)));

    i_valuesMap.insert("user", id);
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

void SWorkModel::setName(const QString name, const QVariant oldValue)
{
    if(!oldValue.toString().isEmpty())
        appendLogText(tr("Название работы изменено с \"%1\" на \"%2\"").arg(oldValue.toString(), name));
    else if(i_id)   // для новой произвольной работы или работы из прайс-листа запись в журнал не нужна
        appendLogText(tr("Название работы изменено на \"%1\"").arg(name));

    i_valuesMap.insert("name", name);
}

float SWorkModel::price()
{
    return m_price;
}

void SWorkModel::setPrice(const float price, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Стоимость работы изменёна с %1 на %2").arg(sysLocale.toCurrencyString(oldValue.toFloat())).arg(sysLocale.toCurrencyString(price)));
    else if(i_id)
        appendLogText(tr("Установлена стоимость работы %1").arg(sysLocale.toCurrencyString(price)));
    i_valuesMap.insert("price", price);
}

int SWorkModel::count()
{
    return m_count;
}

void SWorkModel::setCount(const int count, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Кол-во изменёно с \"%1\" на \"%2\"").arg(oldValue.toInt()).arg(count));

    i_valuesMap.insert("count", count);
}

int SWorkModel::warranty()
{
    return m_warranty;
}

void SWorkModel::setWarranty(const int warranty, const QVariant oldValue)
{
    if(!oldValue.toString().isEmpty())
        appendLogText(tr("Срок гарантии изменён с \"%1\" на \"%2\"").arg(warrantyTermsModel->getDisplayRole(oldValue.toInt(), 1)).arg(warrantyTermsModel->getDisplayRole(warranty, 1)));
    else if(i_id)
        appendLogText(tr("Срок гарантии установлен \"%1\"").arg(warrantyTermsModel->getDisplayRole(warranty, 1)));
    else
        appendLogText(tr("Установлен срок гарантии по умолчанию (\"%1\")").arg(warrantyTermsModel->getDisplayRole(warranty, 1)));

    i_valuesMap.insert("warranty", warranty);
}

int SWorkModel::priceId()
{
    return m_priceId;
}

void SWorkModel::setPriceId(const int id)
{
    if(id)
        i_valuesMap.insert("price_id", id);
    else
        i_valuesMap.insert("price_id", QVariant());
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

bool SWorkModel::remove()
{
    bool ret = 1;

    i_logRecord->setText(tr("Удалена работа \"%1\" стоимостью %2 из ремонта №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repair));
    ret &= del();
    ret &= i_logRecord->commit();

    return ret;
}

void SWorkModel::setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue)
{
    switch(fieldNum)
    {
        case SStoreItemModel::ColUser: setUser(value.toInt(), oldValue); break;
        case SStoreItemModel::ColObjId: setRepair(value.toInt()); break;
        case SStoreItemModel::ColName: setName(value.toString(), oldValue); break;
        case SStoreItemModel::ColPrice: setPrice(value.toFloat(), oldValue); break;
        case SStoreItemModel::ColCount: setCount(value.toInt(), oldValue); break;
        case SStoreItemModel::ColWarranty: setWarranty(value.toInt(), oldValue); break;
        case SStoreItemModel::ColItemId: setPriceId(value.toInt()); break;
        case SStoreItemModel::ColCreated: setCreated(value.toDateTime()); break;
    }
}

bool SWorkModel::commit()
{
    if(i_id)
    {
        qDebug().nospace() << "[" << this << "] commit() [update]";
        update();
    }
    else
    {
        qDebug().nospace() << "[" << this << "] commit() [insert]";
        appendLogText(tr("Добавлена работа \"%1\" стоимостью %2 в ремонт №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repair));
        setCreated(QDateTime::currentDateTime());
        setPayRepair(usersSalaryTaxesModel->value(userDbData->value("id").toInt(), "id", "pay_repair").toInt());
        setPayRepairQuick(usersSalaryTaxesModel->value(userDbData->value("id").toInt(), "id", "pay_repair_quick").toInt());
        insert();
    }
    commitLogs();

    return i_nErr;
}


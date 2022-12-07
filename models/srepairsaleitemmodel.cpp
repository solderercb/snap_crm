#include "srepairsaleitemmodel.h"

SRepairSaleItemModel::SRepairSaleItemModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "store_int_reserve";
    i_obligatoryFields << "item_id" << "count" << "from_user" << "to_user" << "notes" << "state" << "price";
}

/*  Конструктор класса, предназначенный для использования при продаже товара
*/
SRepairSaleItemModel::SRepairSaleItemModel(const QList<QStandardItem *> &record, QObject *parent) :
    SRepairSaleItemModel(parent)
{
    i_id = record.at(SStoreItemModel::SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();

    m_storeItem = new SStoreItemModel(record, count());
    m_storeItem->setSaleMode(SStoreItemModel::SaleMode::Repair);

    // TODO: сделать выборочную передачу значений: для не новой РН нужно передавать только изменённые данные
//    if(i_id == 0)
//    {
//        setItemId(record.at(SStoreItemModel::SaleOpColumns::ColItemId)->data(Qt::DisplayRole).toInt());
//        setName(record.at(SStoreItemModel::SaleOpColumns::ColName)->data(Qt::DisplayRole).toString());
//        setCount(record.at(SStoreItemModel::SaleOpColumns::ColCount)->data(Qt::DisplayRole).toInt());
//        setCreated(record.at(SStoreItemModel::SaleOpColumns::ColCreated)->data(Qt::DisplayRole).toDateTime());
//        setFromUser(userDbData->value("id").toInt());
//        setToUser(userDbData->value("id").toInt());
//        setNotes(record.at(SStoreItemModel::SaleOpColumns::ColNotes)->data(Qt::DisplayRole).toString());
//        setState(record.at(SStoreItemModel::SaleOpColumns::ColState)->data(Qt::DisplayRole).toInt());
//        setRepairId(record.at(SStoreItemModel::SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt());
//        setWorkId(record.at(SStoreItemModel::SaleOpColumns::ColWorkId)->data(Qt::DisplayRole).toInt());
//        setPrice(record.at(SStoreItemModel::SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toFloat());
//        setSn(record.at(SStoreItemModel::SaleOpColumns::ColSN)->data(Qt::DisplayRole).toString());
//        setWarranty(record.at(SStoreItemModel::SaleOpColumns::ColWarranty)->data(Qt::DisplayRole).toInt());
    //    }
}

SRepairSaleItemModel::~SRepairSaleItemModel()
{
    if(m_storeItem)
    {
        delete m_storeItem;
        m_storeItem = nullptr;
    }
}

int SRepairSaleItemModel::id()
{
    return i_id;
}

void SRepairSaleItemModel::load(const int)
{

}

int SRepairSaleItemModel::itemId()
{
    return m_itemId;
}

void SRepairSaleItemModel::setItemId(const int id)
{
    m_itemId = id;
    i_valuesMap.insert("item_id", m_itemId);
}

QString SRepairSaleItemModel::name()
{
    return m_name;
}

void SRepairSaleItemModel::setName(const QString name)
{
    m_name = name;
    i_valuesMap.insert("name", m_name);
}

int SRepairSaleItemModel::count()
{
    if( i_id == 0 )
        return 0;

    QUERY_EXEC(i_query,i_nErr)( QUERY_SEL_REPAIR_SALES_QTY(i_id));
    i_query->first();
    return i_query->value(0).toInt();
}

void SRepairSaleItemModel::setCount(const int count)
{
    m_count = count;
    i_valuesMap.insert("count", m_count);
}

void SRepairSaleItemModel::setCreated(const QDateTime created)
{
    if(i_id)
        i_createdUtc = created;
    else
        i_createdUtc = QDateTime::currentDateTimeUtc();

    i_valuesMap.insert("created", i_createdUtc.toLocalTime());
}

int SRepairSaleItemModel::fromUser()
{
    return m_fromUser;
}

void SRepairSaleItemModel::setFromUser(const int id)
{
    m_fromUser = id;
    i_valuesMap.insert("from_user", m_fromUser);
}

int SRepairSaleItemModel::toUser()
{
    return m_toUser;
}

void SRepairSaleItemModel::setToUser(const int id)
{
    m_toUser = id;
    i_valuesMap.insert("to_user", m_toUser);
}

QString SRepairSaleItemModel::notes()
{
    return m_notes;
}

void SRepairSaleItemModel::setNotes(const QString notes)
{
    m_notes = notes;
    i_valuesMap.insert("notes", m_notes);
}

int SRepairSaleItemModel::state()
{
    return m_state;
}

void SRepairSaleItemModel::setState(const int state)
{
    m_state = state;
    i_valuesMap.insert("state", m_state);
}

int SRepairSaleItemModel::repairId()
{
    return m_repairId;
}

void SRepairSaleItemModel::setRepairId(const int id)
{
    m_repairId = id;
    i_valuesMap.insert("repair_id", id);
    i_logRecord->setRepairId(id);
}

int SRepairSaleItemModel::workId()
{
    return m_workId;
}

void SRepairSaleItemModel::setWorkId(const int id)
{
    m_workId = id;
    i_valuesMap.insert("work_id", m_workId);
}

float SRepairSaleItemModel::price()
{
    return m_price;
}

void SRepairSaleItemModel::setPrice(const float price)
{
    m_price = price;
    i_valuesMap.insert("price", m_price);
}

QString SRepairSaleItemModel::sn()
{
    return m_sn;
}

void SRepairSaleItemModel::setSn(const QString sn)
{
    m_sn = sn;
    i_valuesMap.insert("sn", m_sn);
}

int SRepairSaleItemModel::warranty()
{
    return m_warranty;
}

void SRepairSaleItemModel::setWarranty(const int warranty)
{
    i_valuesMap.insert("warranty", warranty);
}

bool SRepairSaleItemModel::rLock()
{
    return m_rLock;
}

void SRepairSaleItemModel::setRLock(const bool r_lock)
{
    i_valuesMap.insert("r_lock", r_lock);
}

bool SRepairSaleItemModel::sale()
{
    setState(State::Sold);
    commit();
    i_nErr &= m_storeItem->saleRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::reserve()
{
    setState(State::RepairLinked);
    commit();
    i_nErr &= m_storeItem->reserveRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::unsale()
{
    setState(State::RepairLinked);
    commit();
    i_nErr &= m_storeItem->unsaleRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::free()
{
    setState(State::EngineerBasket);
    commit();
    i_nErr &= m_storeItem->free();
    return i_nErr;
}

bool SRepairSaleItemModel::commit()
{
    if(i_id)
        update();
    else
        insert();

    return i_nErr;
}


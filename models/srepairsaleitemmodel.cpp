#include "srepairsaleitemmodel.h"

SRepairSaleItemModel::SRepairSaleItemModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "store_int_reserve";
    i_obligatoryFields << "item_id" << "count" << "from_user" << "to_user" << "notes" << "state" << "price";
    i_logRecord->setType(SLogRecordModel::Repair);
}

/*  Конструктор класса, предназначенный для использования при продаже товара
*/
SRepairSaleItemModel::SRepairSaleItemModel(const QList<QStandardItem *> &record, QObject *parent) :
    SRepairSaleItemModel(parent)
{
    i_id = record.at(SStoreItemModel::SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();

    m_storeItem = new SStoreItemModel(record, count());
    m_storeItem->setSaleMode(SStoreItemModel::SaleMode::Repair);

    i_id = record.at(SStoreItemModel::SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();
    m_itemId = record.at(SStoreItemModel::SaleOpColumns::ColItemId)->data(Qt::DisplayRole).toInt();
    m_name = record.at(SStoreItemModel::SaleOpColumns::ColName)->data(Qt::DisplayRole).toString();
    m_count = record.at(SStoreItemModel::SaleOpColumns::ColCount)->data(Qt::DisplayRole).toInt();
    i_createdUtc = record.at(SStoreItemModel::SaleOpColumns::ColCreated)->data(Qt::DisplayRole).toDateTime();
    m_notes = record.at(SStoreItemModel::SaleOpColumns::ColNotes)->data(Qt::DisplayRole).toString();
    m_state = record.at(SStoreItemModel::SaleOpColumns::ColState)->data(Qt::DisplayRole).toInt();
    m_repairId = record.at(SStoreItemModel::SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt();
    m_workId = record.at(SStoreItemModel::SaleOpColumns::ColWorkId)->data(Qt::DisplayRole).toInt();
    m_price = record.at(SStoreItemModel::SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toFloat();
    m_sn = record.at(SStoreItemModel::SaleOpColumns::ColSN)->data(Qt::DisplayRole).toString();
    m_warranty = record.at(SStoreItemModel::SaleOpColumns::ColWarranty)->data(Qt::DisplayRole).toInt();

    i_logRecord->setType(SLogRecordModel::Repair);
    i_logRecord->setRepairId(m_repairId);

    if(!i_id)
        i_valuesMap.insert("user", userDbData->value("id"));

    for(int i = 1; i < record.count(); i++) // в нулевом столбце id записи в таблице, он не изменяется средствами программы
    {
        if(!record.at(i)->data(Qt::UserRole+1).toBool())
            continue;

        setField(i, record.at(i)->data(Qt::DisplayRole));
        record.at(i)->setData(0, Qt::UserRole+1);   // снятие пометки изменённого поля
    }
    record.at(0)->setData(0, Qt::UserRole+1);   // снятие пометки изменённой строки
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
    i_valuesMap.insert("item_id", m_itemId);
}

QString SRepairSaleItemModel::name()
{
    return m_name;
}

void SRepairSaleItemModel::setName(const QString name)
{
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
    i_valuesMap.insert("from_user", m_fromUser);
}

int SRepairSaleItemModel::toUser()
{
    return m_toUser;
}

void SRepairSaleItemModel::setToUser(const int id)
{
    i_valuesMap.insert("to_user", m_toUser);
}

QString SRepairSaleItemModel::notes()
{
    return m_notes;
}

void SRepairSaleItemModel::setNotes(const QString notes)
{
    i_valuesMap.insert("notes", m_notes);
}

int SRepairSaleItemModel::state()
{
    return m_state;
}

void SRepairSaleItemModel::setState(const int state)
{
    i_valuesMap.insert("state", state);
}

int SRepairSaleItemModel::repairId()
{
    return m_repairId;
}

void SRepairSaleItemModel::setRepairId(const int id)
{
    if(id)
    {
        i_valuesMap.insert("repair_id", id);
        i_logRecord->setRepairId(m_repairId);
    }
    else
    {
        i_valuesMap.insert("repair_id", QVariant());
    }
}

int SRepairSaleItemModel::workId()
{
    return m_workId;
}

void SRepairSaleItemModel::setWorkId(const int id)
{
    if(id)
        i_valuesMap.insert("work_id", m_workId);
    else
        i_valuesMap.insert("work_id", QVariant());
}

float SRepairSaleItemModel::price()
{
    return m_price;
}

void SRepairSaleItemModel::setPrice(const float price)
{
    i_valuesMap.insert("price", m_price);
}

QString SRepairSaleItemModel::sn()
{
    return m_sn;
}

void SRepairSaleItemModel::setSN(const QString sn)
{
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

bool SRepairSaleItemModel::reserve()
{
    i_nErr = m_storeItem->reserveRepair();
    commit(1);
    // Запись в журнал производится в методе класса SStoreItemModel
    return i_nErr;
}

bool SRepairSaleItemModel::linkRepair()
{
    setState(State::RepairLinked);
    setRepairId(m_repairId);
    i_logRecord->setText(tr("Установлена запчасть: %1, стоимость %2 из ремонта №%3").arg(m_name).arg(m_price).arg(m_repairId));
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::sale()
{
    setState(State::Sold);
    commit(1);
    // Запись в журнал производится в методе класса SStoreItemModel
    i_nErr &= m_storeItem->saleRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::unsale()
{
    setState(State::RepairLinked);
    commit(1);
    // Запись в журнал производится в методе класса SStoreItemModel
    i_nErr &= m_storeItem->unsaleRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::unlinkRepair()
{
    setState(State::EngineerBasket);
    setSN("");
    setRepairId(0);
    setWorkId(0);
    i_logRecord->setText(tr("Удалена запчасть: %1, стоимость %2 из ремонта №%3").arg(m_name).arg(m_price).arg(m_repairId));
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::free()
{
    setState(State::Archive);
    commit(1);
    // Запись в журнал производится в методе класса SStoreItemModel
}

void SRepairSaleItemModel::setField(const int fieldNum, const QVariant value)
{
    switch(fieldNum)
    {
        case SStoreItemModel::ColItemId: setItemId(value.toInt()); break;
    case SStoreItemModel::ColName: setName(value.toString()); break;
        case SStoreItemModel::ColObjId: setRepairId(value.toInt()); break;
        case SStoreItemModel::ColCount: setCount(value.toInt()); break;
        case SStoreItemModel::ColPrice: setPrice(value.toFloat()); break;
        case SStoreItemModel::ColWarranty: setWarranty(value.toInt()); break;
        case SStoreItemModel::ColState: setState((State)value.toInt()); break;
        case SStoreItemModel::ColSN: setSN(value.toString()); break;
    }
}

bool SRepairSaleItemModel::commit(const bool skipLogRecording)
{
    qDebug().nospace() << "[" << this << "] commit()";
    if(i_id)
        update();
    else
    {
        setCreated(QDateTime::currentDateTime());
        setFromUser(userDbData->value("id").toInt());
        setToUser(userDbData->value("id").toInt());
        if(!i_valuesMap.contains("notes"))
            setNotes(tr("Выдача товара \"%1\" сотруднику %2").arg(m_name, QSqlDatabase::database("connMain").userName()));
        insert();
    }

    if(!skipLogRecording)
        i_logRecord->commit();

    return i_nErr;
}


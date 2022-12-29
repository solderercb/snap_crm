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

    qDebug().nospace() << "[" << this << "] SRepairSaleItemModel() | " << QString("m_workId = %1 | m_repairId = %2").arg(m_workId).arg(m_repairId);
    i_logRecord->setRepairId(m_repairId);

    for(int i = 1; i < record.count(); i++) // в нулевом столбце id записи в таблице, он не изменяется средствами программы
    {
        if(!record.at(i)->data(Qt::UserRole+1).toBool())
            continue;

        setField(i, record.at(i)->data(Qt::DisplayRole), record.at(i)->data(Qt::UserRole));
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
    i_valuesMap.insert("item_id", id);
}

QString SRepairSaleItemModel::name()
{
    return m_name;
}

void SRepairSaleItemModel::setName(const QString name)
{
    i_valuesMap.insert("name", name);
}

int SRepairSaleItemModel::count()
{
    if( i_id == 0 )
        return 0;

    QUERY_EXEC(i_query,i_nErr)( QUERY_SEL_REPAIR_SALES_QTY(i_id));
    i_query->first();
    return i_query->value(0).toInt();
}

void SRepairSaleItemModel::setCount(const int count, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Количество изменёно с %1 на %2").arg(usersModel->getDisplayRole(oldValue.toInt())).arg(count));

    i_valuesMap.insert("count", count);
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
    i_valuesMap.insert("from_user", id);
}

int SRepairSaleItemModel::toUser()
{
    return m_toUser;
}

void SRepairSaleItemModel::setToUser(const int id, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Товар перемещён из корзины сотрудника %1 в корзину сотрудника %2").arg(usersModel->getDisplayRole(oldValue.toInt())).arg(usersModel->getDisplayRole(id)));

    i_valuesMap.insert("to_user", id);
}

QString SRepairSaleItemModel::notes()
{
    return m_notes;
}

void SRepairSaleItemModel::setNotes(const QString notes)
{
    i_valuesMap.insert("notes", notes);
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
        i_logRecord->setRepairId(id);
        appendLogText(tr("Установлена запчасть \"%1\" стоимостью %2 в ремонт №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repairId));
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
        i_valuesMap.insert("work_id", id);
    else
        i_valuesMap.insert("work_id", QVariant());
}

float SRepairSaleItemModel::price()
{
    return m_price;
}

void SRepairSaleItemModel::setPrice(const float price, const QVariant oldValue)
{
    if(oldValue.isValid())  // возможность изменять цену товара должна быть доступна только при административной правке
        appendLogText(tr("Стоимость товара изменена с %1 на %2").arg(sysLocale.toCurrencyString(oldValue.toFloat())).arg(sysLocale.toCurrencyString(price)));

    i_valuesMap.insert("price", price);
}

QString SRepairSaleItemModel::sn()
{
    return m_sn;
}

void SRepairSaleItemModel::setSN(const QString sn, const QVariant oldValue)
{
    if(!oldValue.toString().isEmpty())
        appendLogText(tr("Серийный номер изменён с \"%1\" на \"%2\"").arg(oldValue.toString()).arg(sn));
    else if (i_id && !sn.isEmpty())
        appendLogText(tr("Установлен серийный номер товара \"%1\"").arg(sn));

    i_valuesMap.insert("sn", sn);
}

int SRepairSaleItemModel::warranty()
{
    return m_warranty;
}

void SRepairSaleItemModel::setWarranty(const int warranty, const QVariant oldValue)
{
    if(oldValue.isValid())
        appendLogText(tr("Для детали \"%1\" срок гарантии изменён с \"%2\" на \"%3\"").arg(m_name).arg(warrantyTermsModel->getDisplayRole(oldValue.toInt(), 1)).arg(warrantyTermsModel->getDisplayRole(warranty, 1)));
    else if(i_id)
        appendLogText(tr("Для детали \"%1\" установлен срок гарантии \"%2\"").arg(m_name).arg(warrantyTermsModel->getDisplayRole(warranty, 1)));
    else
        appendLogText(tr("Для детали \"%1\" установлен срок гарантии по умолчанию (\"%2\")").arg(m_name).arg(warrantyTermsModel->getDisplayRole(warranty, 1)));

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
    qDebug().nospace() << "[" << this << "] ()";
    i_nErr = m_storeItem->reserveRepair();
    i_nErr = linkRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::linkRepair()
{
    setState(State::RepairLinked);
//    setRepairId(m_repairId);
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::sale()
{
    setState(State::Sold);
    commit();
    // Запись в журнал производится в методе класса SStoreItemModel
    i_nErr &= m_storeItem->saleRepair();
    return i_nErr;
}

bool SRepairSaleItemModel::unsale()
{
    setState(State::RepairLinked);
    commit();
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
    appendLogText(tr("Удалена запчасть \"%1\" стоимостью %2 из ремонта №%3").arg(m_name).arg(sysLocale.toCurrencyString(m_price)).arg(m_repairId));
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::free()
{
    setState(State::Archive);
    commit();
    // Запись в журнал производится в методе класса SStoreItemModel
}

void SRepairSaleItemModel::setField(const int fieldNum, const QVariant value, const QVariant oldValue)
{
    switch(fieldNum)
    {
        case SStoreItemModel::ColItemId: setItemId(value.toInt()); break;
        case SStoreItemModel::ColName: setName(value.toString()); break;
        case SStoreItemModel::ColObjId: setRepairId(value.toInt()); break;
        case SStoreItemModel::ColCount: setCount(value.toInt(), oldValue); break;
        case SStoreItemModel::ColPrice: setPrice(value.toFloat(), oldValue); break;
        case SStoreItemModel::ColWarranty: setWarranty(value.toInt(), oldValue); break;
        case SStoreItemModel::ColState: setState((State)value.toInt()); break;
        case SStoreItemModel::ColSN: setSN(value.toString(), oldValue); break;
        case SStoreItemModel::ColWorkId: setWorkId(value.toInt()); break;
    }
}

bool SRepairSaleItemModel::commit()
{
    qDebug().nospace() << "[" << this << "] commit()";
    if(i_id)
        update();
    else
    {
        setCreated(QDateTime::currentDateTime());
        if(!i_valuesMap.contains("from_user"))
            setFromUser(userDbData->value("id").toInt());
        if(!i_valuesMap.contains("to_user"))
            setToUser(userDbData->value("id").toInt());
        if(!i_valuesMap.contains("notes"))
            setNotes(tr("Выдача товара \"%1\" сотруднику %2").arg(m_name, QSqlDatabase::database("connMain").userName()));
        insert();
    }

    commitLogs();

    return i_nErr;
}


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
    m_price = record.at(SStoreItemModel::SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toDouble();
    m_sn = record.at(SStoreItemModel::SaleOpColumns::ColSN)->data(Qt::DisplayRole).toString();
    m_warranty = record.at(SStoreItemModel::SaleOpColumns::ColWarranty)->data(Qt::DisplayRole).toInt();

    qDebug().nospace() << "[" << this << "] SRepairSaleItemModel() | " << QString("m_workId = %1 | m_repairId = %2").arg(m_workId).arg(m_repairId);
    i_logRecord->setRepairId(m_repairId);

    initQueryFields(record);
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
        appendLogText(tr("Количество \"%1\" изменёно с %2 на %3").arg(m_name, usersModel->getDisplayRole(oldValue.toInt())).arg(count));

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
        appendLogText(tr("Товар \"%1\" перемещён из корзины сотрудника %2 в корзину сотрудника %3").arg(m_name, usersModel->getDisplayRole(oldValue.toInt()), usersModel->getDisplayRole(id)));

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
        appendLogText(tr("Добавлен товар \"%1\" стоимостью %2 в кол-ве %3ед").arg(m_name, sysLocale.toCurrencyString(m_price)).arg(m_count));
        i_valuesMap.insert("repair_id", id);
        i_logRecord->setRepairId(id);
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

double SRepairSaleItemModel::price()
{
    return m_price;
}

void SRepairSaleItemModel::setPrice(const double price, const QVariant oldValue)
{
    if(oldValue.isValid())  // возможность изменять цену товара должна быть доступна только при административной правке
        appendLogText(tr("Стоимость товара \"%1\" изменена с %2 на %3").arg(m_name, sysLocale.toCurrencyString(oldValue.toDouble()), sysLocale.toCurrencyString(price)));

    i_valuesMap.insert("price", price);
}

QString SRepairSaleItemModel::sn()
{
    return m_sn;
}

void SRepairSaleItemModel::setSN(const QString sn, const QVariant oldValue)
{
    if(!oldValue.toString().isEmpty())
        appendLogText(tr("Серийный номер товара \"%1\" изменён с \"%2\" на \"%3\"").arg(m_name, oldValue.toString(), sn));
    else if (i_id && !sn.isEmpty())
        appendLogText(tr("Серийный номер товара \"%1\" установлен \"%2\"").arg(m_name, sn));

    i_valuesMap.insert("sn", sn.isEmpty()?QVariant():sn);
}

int SRepairSaleItemModel::warranty()
{
    return m_warranty;
}

void SRepairSaleItemModel::setWarranty(const int warranty, const QVariant oldValue)
{
    QString text;
    if(oldValue.isValid())
        text = tr("Срок гарантии на товар \"%1\" изменён с \"%2\" на \"%3\"").arg(m_name, warrantyTermsModel->getDisplayRole(oldValue.toInt(), 1), warrantyTermsModel->getDisplayRole(warranty, 1));
    else if(i_id)
        text = tr("Срок гарантии на товар \"%1\" установлен \"%2\"").arg(m_name, warrantyTermsModel->getDisplayRole(warranty, 1));
    else
        text = tr("Срок гарантии на товар \"%1\" установлен по умолчанию (\"%2\")").arg(m_name, warrantyTermsModel->getDisplayRole(warranty, 1));

    appendLogText(text, QString::number(SStoreItemModel::SaleOpColumns::ColWarranty)); // disambiguation здесь нужен для установки порядка записей в журнал
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

bool SRepairSaleItemModel::request()
{
    qDebug().nospace() << "[" << this << "] request()";
    setState(State::Requested);
    // TODO: создание уведомления ответственному сотруднику
    setNotes("");
    commit();

    return i_nErr;
}

/*  Выдача товара сотруднику
 *  Может быть инициирована ответственным сотрудником или как ответ на запрос детали
*/
bool SRepairSaleItemModel::reserve()
{
    QString logText;
    qDebug().nospace() << "[" << this << "] reserve()";
    if(!i_id)
    {
        logText = tr("Выдача %1ед. товара сотруднику %2").arg(m_count).arg(allUsersMap->value(userDbData->value("id").toInt()));
        setNotes(logText);
        i_nErr = m_storeItem->reserve(logText);
    }
    setState(State::EngineerBasket);
    commit();
    return i_nErr;
}

/*  Установка товара в ремонт (привязка)
 *  Может быть инициирована сотрудником, обладающим правом "Брать детали со склада", или при добавлении товара из корзины сотрудника
*/
bool SRepairSaleItemModel::linkRepair(const int id)
{
    QString logText;
    qDebug().nospace() << "[" << this << "] linkRepair()";
    setRepairId(id);
    if(!i_id)
    {
        logText = tr("Автовыдача %1ед. товара сотруднику %2 для ремонта №%3").arg(m_count).arg(allUsersMap->value(userDbData->value("id").toInt())).arg(id);
        setNotes(logText);
        i_nErr = m_storeItem->reserve(logText);
    }
    setState(State::RepairLinked);
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::sale()
{
    setState(State::Sold);
    commit();
    i_nErr &= m_storeItem->saleRepair(tr("Продажа %1ед. товара в связи с выдачей ремонта №%2.").arg(m_count).arg(m_repairId));
    return i_nErr;
}

bool SRepairSaleItemModel::unsale()
{
    setState(State::RepairLinked);
    commit();
    i_nErr &= m_storeItem->unsaleRepair(tr("Возврат %1ед. товара по причене отмены выдачи ремонта №%2.").arg(m_count).arg(m_repairId));
    return i_nErr;
}

bool SRepairSaleItemModel::unlinkRepair()
{
    setState(State::EngineerBasket);
    setSN("");
    setRepairId(0);
    setWorkId(0);
    appendLogText(tr("Удален товар \"%1\" стоимостью %2 в кол-ве %3ед.").arg(m_name, sysLocale.toCurrencyString(m_price)).arg(m_count));
    commit();
    return i_nErr;
}

bool SRepairSaleItemModel::free()
{
    setState(State::Archive);
    commit();
    i_nErr &= m_storeItem->free(tr("Возврат %1ед. товара на склад").arg(m_count));
    return i_nErr;
}

void SRepairSaleItemModel::setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue)
{
    switch(fieldNum)
    {
        case SStoreItemModel::ColItemId: setItemId(value.toInt()); break;
        case SStoreItemModel::ColName: setName(value.toString()); break;
        case SStoreItemModel::ColObjId: setRepairId(value.toInt()); break;
        case SStoreItemModel::ColCount: setCount(value.toInt(), oldValue); break;
        case SStoreItemModel::ColPrice: setPrice(value.toDouble(), oldValue); break;
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


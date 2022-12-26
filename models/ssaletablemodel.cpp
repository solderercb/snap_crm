#include "ssaletablemodel.h"

SSaleTableModel::SSaleTableModel(QObject *parent) :
    QStandardItemModel(parent),
    m_queryData(new QSqlQueryModel)
{
    m_itemsPendingRemoveList = new QMap<int, int>;
    m_worksPendingRemoveList = new QMap<int, int>;

    QStandardItemModel::setHorizontalHeaderLabels(m_fieldsDep);
    setHorizontalHeaderLabels({"",tr("UID"),tr("Наименование"),tr("Кол-во"),tr("Доступно"),tr("Цена"),tr("Сумма"),tr("Место"),tr("Серийный номер"),tr("Гарантия"),tr("Сотрудник")});

    connect(m_queryData, SIGNAL(modelReset()), this, SLOT(sqlDataChanged()));
#if QT_VERSION >= 0x060000
    connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &)), this, SLOT(dataChanaged(const QModelIndex&, const QModelIndex&, const QList<int> &)));
#else
    connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)), this, SLOT(dataChanaged(const QModelIndex&, const QModelIndex&, const QVector<int> &)));
#endif
}

SSaleTableModel::~SSaleTableModel()
{
    delete m_itemsPendingRemoveList;
    delete m_worksPendingRemoveList;
    delete m_queryData;
}

QVariant SSaleTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
        case SStoreItemModel::SaleOpColumns::ColPrice:
        case SStoreItemModel::SaleOpColumns::ColSumm: return sysLocale.toString(QStandardItemModel::data(index, role).toFloat(), 'f', 2);
        case SStoreItemModel::SaleOpColumns::ColBox: return itemBoxesModel->getDisplayRole(QStandardItemModel::data(index, role).toInt(), 1);
        case SStoreItemModel::SaleOpColumns::ColWarranty: return warrantyTermsModel->getDisplayRole(QStandardItemModel::data(index, role).toInt(), 1);
        case SStoreItemModel::SaleOpColumns::ColUser: return allUsersMap->value(QStandardItemModel::data(index, role).toInt());
        }
    }
    if ( role == Qt::BackgroundRole )
    {
        int state = value(index.row(), SStoreItemModel::SaleOpColumns::ColState).toInt();
        if( (m_tableMode == TablesSet::StoreSale && state) ||
            (m_tableMode == TablesSet::WorkshopSale && state == SRepairSaleItemModel::State::EngineerBasket) )
            return QColor("light gray");
    }

    return QStandardItemModel::data(index, role);
}

Qt::ItemFlags SSaleTableModel::flags(const QModelIndex &index) const
{
    if( m_modelState == State::StoreNew || m_modelState == State::StoreReserved || m_modelState == State::WorkshopRW )   // у вкладки будет дополнительный режим — правка резерва (в АСЦ такого вроде не было)
    {
        switch (index.column())
        {
            case SStoreItemModel::SaleOpColumns::ColName: if(!this->index(index.row(), SStoreItemModel::SaleOpColumns::ColUID).data().toString().isEmpty()) return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
            case SStoreItemModel::SaleOpColumns::ColCount:
            case SStoreItemModel::SaleOpColumns::ColPrice:
            case SStoreItemModel::SaleOpColumns::ColSN:
            case SStoreItemModel::SaleOpColumns::ColWarranty:
            case SStoreItemModel::SaleOpColumns::ColUser:
                return Qt::ItemIsEnabled | Qt::ItemIsEditable;
            default:
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    else
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
}

bool SSaleTableModel::insertRecord(int row, const QSqlRecord &record, const int recType)
{
    Q_ASSERT_X(record.count() == columnCount(), "insertRecord()", "different column count");
    QList<QStandardItem*> rowData;
    QStandardItem *item;
    int recordId = record.value(SStoreItemModel::SaleOpColumns::ColId).toInt();

    int i;
    for(i = 0; i < record.count(); i++)
    {
        item = new QStandardItem();
        item->setData(record.value(i), Qt::EditRole);
        if(i == SStoreItemModel::SaleOpColumns::ColItemId)
        {
        }

        if( recordId == 0 || i == 0)   // все поля новых записей (добавленных в таблицу store_int_reserve впервые или для работ)
        {                              // или только 0-й столбец записи о товаре, добавленном из корзины сотрудника
            item->setData(record.value(SStoreItemModel::SaleOpColumns::ColRecordType).toInt(), DataRoles::RecordType);
            item->setData(1, DataRoles::Changed);
            if(recordId == 0 && i == SStoreItemModel::SaleOpColumns::ColUser)
            {
                item->setData(userDbData->value("id").toInt(), Qt::EditRole);
                // инициализация поля кодом пользователя производится тоже только для новых товаров; в будущем это позволит
                // реализовать режим админинстративной правки с возможностью вставлять детали из чужой корзины
            }
        }

#ifdef QT_DEBUG
        if(i == SStoreItemModel::SaleOpColumns::ColCount)
        {
            int qtyLimit;
            if(record.value(SStoreItemModel::SaleOpColumns::ColAvail).toInt() > 5)
                qtyLimit = 5;
            else
                qtyLimit = record.value(SStoreItemModel::SaleOpColumns::ColAvail).toInt() + 1;

            int rand = QRandomGenerator::global()->bounded(qtyLimit);
            item->setData(rand?rand:1, Qt::DisplayRole);
        }
#endif

        rowData << item;
    }

    QStandardItemModel::insertRow(row, rowData);
    emit amountChanged(amountTotal(), m_amountItems, m_amountWorks);

    if((m_editStrategy == OnFieldChange || m_editStrategy == OnRowChange) && m_tableMode == TablesSet::WorkshopSale)
        saveTablesWorkshop();

    return true;
}

/* Возвращает 0 в случае неудачи
*/
bool SSaleTableModel::appendRecord(const QSqlRecord &record)
{
    return insertRecord(rowCount(), record);
}

int SSaleTableModel::isItemAlreadyInList(int id)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(value(i, SStoreItemModel::SaleOpColumns::ColItemId).toInt() == id)
            return i;
    }
    return -1;
}

/* Добавление произвольной работы в конец таблицы
*/
void SSaleTableModel::addCustomWork()
{
    qDebug().nospace() << "[" << this << "] addCustomWork()";
    QSqlRecord *customWork = new QSqlRecord();
    QSqlField *field;
    m_currentIndex = rowCount();
    for(int i = 0; i < SStoreItemModel::SaleOpColumns::ColRecordType + 1; i++)
    {
        field = new QSqlField("", QVariant::Int);
        switch(i)
        {
            case SStoreItemModel::SaleOpColumns::ColId: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColUID: field->setValue(""); break;
            case SStoreItemModel::SaleOpColumns::ColName: field->setValue(""); break;
            case SStoreItemModel::SaleOpColumns::ColCount: field->setValue(1); break;
            case SStoreItemModel::SaleOpColumns::ColAvail: field->setValue(""); break;
            case SStoreItemModel::SaleOpColumns::ColPrice: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColSumm: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColBox: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColSN: field->setValue(""); break;
            case SStoreItemModel::SaleOpColumns::ColWarranty: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColUser: field->setValue(userDbData->value("id").toInt()); break;
            case SStoreItemModel::SaleOpColumns::ColRealization: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColRetPercent: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColState: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColNotes: field->setValue(""); break;
            case SStoreItemModel::SaleOpColumns::ColItemId: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColInPrice: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColObjId: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColDealer: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColBuyer: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColCreated: field->setValue(QDateTime::currentDateTimeUtc()); break;
            case SStoreItemModel::SaleOpColumns::ColWorkId: field->setValue(0); break;
            case SStoreItemModel::SaleOpColumns::ColRecordType: field->setValue(RecordType::Work); break;
        }

        customWork->append(*field);
        delete field;
    }
    insertRecord(m_currentIndex, *customWork);
    delete customWork;
    emit modelReset();
}

/* Добавление работы из прайс-листа
*/
bool SSaleTableModel::addWorkByUID(const int uid, const int priceOption)
{
//    QSqlQueryModel *newItemModel;
//    int row = -1;
//    bool ret = 1;

//    if(m_modelState == SSaleTableModel::StoreNew || m_modelState == SSaleTableModel::StoreReserved || m_modelState == SSaleTableModel::WorkshopRW )
//    {
//        newItemModel = new QSqlQueryModel(this);
//        newItemModel->setQuery(QUERY_SEL_PART_FOR_SALE(uid, priceColModel->index(priceOption, 2).data().toString()), QSqlDatabase::database("connMain"));
//        newItemModel->record(0).setValue(SStoreItemModel::SaleOpColumns::ColObjId, m_objId);
//        ret = appendRecord(newItemModel->record(0));
//        delete newItemModel;
//    }
//    emit modelReset();
//    return ret;
}

/*  Добавление товара по id
 *  Запрещено добавлять один и тот же товар дважды (TODO: пересмотреть это решение)
 *  возвращает 0 в случае неудачи
*/
bool SSaleTableModel::addItemByUID(const int uid, const int priceOption, const int count)
{
    QSqlQueryModel *newItemModel;
    int row = -1;
    bool ret = 1;

    if(m_modelState == SSaleTableModel::StoreNew || m_modelState == SSaleTableModel::StoreReserved || m_modelState == SSaleTableModel::WorkshopRW )
    {
        try
        {
            if(m_tableMode == TablesSet::StoreSale && isItemAlreadyInList(uid) >= 0)
                throw 0;

            newItemModel = new QSqlQueryModel(this);
            newItemModel->setQuery(QUERY_SEL_PART_FOR_SALE(uid, priceColModel->index(priceOption, 2).data().toString(), count), QSqlDatabase::database("connMain"));
            if(count > newItemModel->record(0).value("avail").toInt())
                throw 1;

            if(m_tableMode == TablesSet::StoreSale)
            {
                ret = appendRecord(newItemModel->record(0));
            }
            else
            {
                if(m_currentIndex == -1 && rowCount())  // если не выбрана строка и таблица не пуста
                    throw 2;

//                newItemModel->record(0).setValue(SStoreItemModel::SaleOpColumns::ColCount, count);    // альтернатива запросу с передачей требуемого кол-ва
                ret = insertRecord(getItemInsertionRow(), newItemModel->record(0));
            }
        }
        catch (int exception)
        {
            QString msgCaption, msgText;
            if(exception == 0)
            {
                qDebug() << QString("товар UID %1 уже добавлен").arg(value(row, SStoreItemModel::SaleOpColumns::ColUID).toString());
                msgCaption = tr("Повтор");
                msgText = tr("\"%1\" (UID %2) уже добавлен").arg(value(row, SStoreItemModel::SaleOpColumns::ColName).toString()).arg(value(row, SStoreItemModel::SaleOpColumns::ColUID).toString());
            }
            else if(exception == 1)
            {
                qDebug() << QString("Товар UID %1 не доступен").arg(newItemModel->record(0).value("UID").toString());
                msgCaption = tr("Товар отсутствует");
                msgText = tr("\"%1\" (UID %2) не доступен для продажи").arg(value(row, SStoreItemModel::SaleOpColumns::ColName).toString()).arg(value(row, SStoreItemModel::SaleOpColumns::ColUID).toString());
            }
            else if(exception == 2)
            {
                msgCaption = tr("Информация");
                msgText = tr("Не выбрана работа. Укажите работу в которой была использована деталь");
            }

            shortlivedNotification *newPopup = new shortlivedNotification(this, msgCaption, msgText, QColor(255,255,255), QColor(245,245,245));
            ret = 0;
        }
        delete newItemModel;
    }
    emit modelReset();
    return ret;
}

bool SSaleTableModel::addItemFromBasket(const int id, const int qty, const int priceOpt)
{
    bool ret = 1;
    const QString count = qty?QString::number(qty):"`count`";
    int insertionRow = getItemInsertionRow();
    QSqlQueryModel *newItemModel = new QSqlQueryModel(this);
    newItemModel->setQuery(QUERY_SEL_PART_FROM_BASKET(id, priceColModel->index(priceOpt, 2).data().toString(), count), QSqlDatabase::database("connMain"));
    ret = insertRecord(insertionRow, newItemModel->record(0));
    if(qty) // добавление части товара из корзины сотрудника
    {
        /**/qDebug() << "[" << this << "] addItemFromBasket() | TODO: создать копию записи в табл. store_int_reserve с разницей кол-ва";
//        m_itemsPendingSplitList
        setData(index(insertionRow, SStoreItemModel::SaleOpColumns::ColCount), qty);   // обновить кол-во товара в старой записи
    }
}

void SSaleTableModel::removeRowHandler(const int row, const int db_id)
{
    if(m_tableMode == TablesSet::StoreSale)
    {
        storeSaleRemoveRow(row, db_id);
    }
    else
    {
        repairSaleRemoveRow(row, db_id);
    }

    emit amountChanged(amountTotal(), m_amountItems, m_amountWorks);
    emit modelReset();
}

void SSaleTableModel::buttonHandler(const int buttonNum, const int row)
{
    // действия при прямой продаже и в карте ремонта разные; формируем уникальный идентификатор
    int action = m_tableMode << 8 | buttonNum;
    switch(action)
    {
        case (WorkshopSale << 8 | 0):
        case (StoreSale << 8 | 1): removeRowHandler(row, index(row, SStoreItemModel::SaleOpColumns::ColId).data().toInt()); break; // кнопка "удалить"
        case (WorkshopSale << 8 | 2): ; break; // кнопка "добавить деталь"
        case (WorkshopSale << 8 | 1):
        case (StoreSale << 8 | 2): ; break; // нет действия
    }
}

/* Удаление или пометка на удаление строки в режиме продажи: частичный возврат или частичная отмена
 * ранее зарезервированного товара.
 * При первом нажатии кнопки Удалить, в массив добавятся данные, а при повторном - удалятся (на случай,
 * если пользователь промахнулся). При подтверждении возврата или проведении РН резерва, записи с
 * соответствующими id будут обновлены, будет установлен флаг is_cancellation (state).
 * row  - номер строки
 * db_id - id записи в таблице store_sales
 */
void SSaleTableModel::storeSaleRemoveRow(const int row, const int db_id)
{
    if(m_modelState == SSaleTableModel::StoreNew)   // в режиме создания новой РН просто удаляем строки из модели
        removeRows(row, 1);
    else    // в режимах просмотра резерва или проведённой накладной, помечаем строки на возврат/отмену резерва
    {
        if(m_itemsPendingRemoveList->contains(row))
        {
            m_itemsPendingRemoveList->remove(row);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), SStoreSaleItemModel::Active);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), value(row, SStoreItemModel::SaleOpColumns::ColCount, QtyBackup).toInt());   // восстанавливаем значение из UserRole
        }
        else
        {
            m_itemsPendingRemoveList->insert(row, db_id);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), SStoreSaleItemModel::Cancelled);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), value(row, SStoreItemModel::SaleOpColumns::ColCount), QtyBackup); // при пометке на удаление, в UserRole сохраняем текущее кол-во; это на случай, если пользователь промахнулся строкой и тут же нажал кнопку еще раз
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), 0);   // кол-во устанавливаем 0
        }
    }
}

/* Удаление товара из списка в режиме ремонта
 * row  - номер строки
 * db_id - id записей в таблицах store_int_reserve и works
*/
void SSaleTableModel::repairSaleRemoveRow(const int row, const int db_id)
{
    bool newState = 1;
    int recordType = index(row, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool();
    QMap<int, int> *pendingRemoveList;
    if(recordType == RecordType::Work)   // сначала обрабатываем записи о товарах привязанных к удаляемой работе
    {
        pendingRemoveList = m_worksPendingRemoveList;
        int i = row + 1;
        while(i < rowCount() && index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool() == RecordType::Item)
        {
            repairSaleRemoveRow(i, index(i, SStoreItemModel::SaleOpColumns::ColId).data().toInt());
            i++;
        }
    }
    else
        pendingRemoveList = m_itemsPendingRemoveList;

    if(pendingRemoveList->contains(row))
    {
        pendingRemoveList->remove(row);
        newState = 0;
    }
    else
    {
        pendingRemoveList->insert(row, db_id);
    }
    setData(index(row, SStoreItemModel::SaleOpColumns::ColState), newState);

    if(m_editStrategy == EditStrategy::OnFieldChange || m_editStrategy == EditStrategy::OnRowChange)
    {
        try
        {
            saveTablesWorkshop();
        }
        catch(int)
        {}
    }
}

QMap<int, int>* SSaleTableModel::getPendingRemoveList()
{
    return m_itemsPendingRemoveList;
}

int SSaleTableModel::pendingRemoveItemsCount()
{
    if(m_itemsPendingRemoveList->isEmpty())
        return rowCount();

    return m_itemsPendingRemoveList->size();
}

/* Обновление цены
 * в данном методе для всех товаров, уже добавленных в таблицу, будут запрошен соответствующая колонка цен
 */
void SSaleTableModel::setPriceColumn(QSqlQuery *query)
{
    QString qry = query->lastQuery();   // для проверки правильности запроса

    Q_ASSERT_X(qry.contains(":id", Qt::CaseSensitive), "SSaleTableModel::setPriceColumn(QSqlQuery *)", "в подготовленном запросе не задан (или задан не подходящий) placeholder");
    for(int i = 0; i < rowCount(); i++)
    {
        query->bindValue(":id", index(i,  SStoreItemModel::SaleOpColumns::ColItemId).data().toInt());
        query->exec();
        query->first();
        setData(index(i,  SStoreItemModel::SaleOpColumns::ColPrice), query->record().value(0).toFloat());
    }
}

/* Загрузка таблицы товаров ранее сохранённого документа (продажа)
 * Возвращает 1 в случае ошибки
*/
bool SSaleTableModel::loadStoreSale(const int doc_id)
{
    setTableMode(TablesSet::StoreSale);
    setDocumentId(doc_id);
    if(m_modelState == State::StoreSold)
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC(m_objId), QSqlDatabase::database("connMain"));
    else if( m_modelState == State::StoreReserved || m_modelState == State::StoreCancelled )
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(m_objId), QSqlDatabase::database("connMain"));
    else
        return 1;

    return 0;
}

/* Загрузка таблицы товаров ремонта
 * Возвращает 1 в случае ошибки
*/
bool SSaleTableModel::loadWorkshopSale(const int repair_id)
{
    setTableMode(TablesSet::WorkshopSale);
    setRepairId(repair_id);
    m_queryData->setQuery(QUERY_SEL_REPAIR_WORKS_AND_PARTS(m_objId), QSqlDatabase::database("connMain"));

    return 0;
}

void SSaleTableModel::setClient(int id)
{
    m_client = id;
}

void SSaleTableModel::unsetClient()
{
//    values.insert("client", "NULL");
    m_client = 0;
}

void SSaleTableModel::setDocumentId(int id)
{
    m_objId = id;
}

void SSaleTableModel::setRepairId(int id)
{
    m_objId = id;
}

/*  Продажа (в т. ч. ранее зарезервированных) и резерв товаров.
 *  Вызов метода происходит в результате нажатия кнопки пользователем (т. е. сохранение не происходит автоматически)
 *  Допускается, что покупатель откажется от части зарезервированных товаров (помеченные пользователем строки)
 *  Возвращает 0 в случае ошибки
 */
bool SSaleTableModel::saveTablesStore(StoreOpType type)
{
    bool ret = 1;

    if(!m_itemsPendingRemoveList->isEmpty())
        ret = storeBackOutItems(StoreOpType::FreeReserved);

    m_itemsAffected = 0;
    for(int i = 0; i < rowCount() && ret; i++)
    {
        if(index(i, SStoreItemModel::SaleOpColumns::ColState).data().toBool())  // частично снятые с резерва пропускаем
            continue;

        m_itemsAffected++;
        setData(index(i, SStoreItemModel::SaleOpColumns::ColObjId), m_objId);
        setData(index(i, SStoreItemModel::SaleOpColumns::ColBuyer), m_client);
        SStoreSaleItemModel *itm = storeItem(i);
        if(type == StoreOpType::Sale)
            ret = itm->sale();
        else
            ret = itm->reserve();

        delete itm;
    }

    if(!ret)
        throw 1;

    return ret;
}

/*  Сохранение таблицы работ и деталей в ремонте.
 *  Вызов метода может происходить, в зависимости от настроек пользователя, при:
 *   - завершении редактирования таблицы (т. е. автоматически после любого изменения; по умолчанию)
 *   - TODO: вручную.
 *  Возвращает 0 в случае ошибки
 *  типы операций:
 *   - в таблицу была добавлена новая деталь: все поля помечены как изменённые, workId = <NNNN>, ObjId = RepairId и State = Linked; UPDATE store_items и INSERT store_int_reserve
 *   - в таблицу была добавлена деталь из корзины сотрудника: поля workId = <NNNN>, ObjId = RepairId и State = Linked, возможно изменено поле Count; UPDATE store_int_reserve
 *   - из таблицы удалена деталь: поля SN = NULL, workId = NULL, ObjId = NULL и State = EngineerBasket;  UPDATE store_int_reserve
 */
bool SSaleTableModel::saveTablesWorkshop()
{
    qDebug().nospace() << "[" << this << "] saveTablesWorkshop()";
    bool ret = 1;
    bool rowModified = 0;

    qDebug().nospace() << "[" << this << "] saveTablesWorkshop() | " << QString("rowCount before remove: %1").arg(rowCount());
    repairRemoveRows();
    qDebug().nospace() << "[" << this << "] saveTablesWorkshop() | " << QString("rowCount after remove: %1").arg(rowCount());

    for(int i = 0; i < rowCount() && ret; i++)
    {
        rowModified = index(i, SStoreItemModel::SaleOpColumns::ColId).data(Changed).toBool();

        if(!rowModified)
            qDebug().nospace() << "[" << this << "] saveTablesWorkshop() | " << QString("row %1 not changed, skipping...").arg(i);

        if(index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool() == RecordType::Work)
        {
            SWorkModel *work = repairWork(i);
            if(rowModified)
                ret = work->commit();
            m_lastHandledWorkId = work->id();
            delete work;
        }
        else if(rowModified)   // обработка записи товара (только изменённые строки)
        {
            setItemWorkId(i, m_lastHandledWorkId);
            SRepairSaleItemModel *itm = repairItem(i);
            if(!index(i, SStoreItemModel::SaleOpColumns::ColId).data().toInt())
                ret = itm->reserve();
            else
                ret = itm->commit(1);
        }
    }
    if(!ret)
        throw 1;

    return ret;
}

/*
 * типы операций:
 *  - выдача ремонта: поле State = Sold; UPDATE store_items и UPDATE store_int_reserve; пропуск полей State == Sold (¹)
 *  - возврат ремонта (с возвратом ЗИП на склад): поле State = Linked; UPDATE store_items и UPDATE store_int_reserve
 *  - возврат ремонта (без возврата ЗИП на склад): нет действий
 * (¹) выдача ремонта после возврата (случай без возврата ЗИП на склад): пропуск полей State == Sold
 */
bool SSaleTableModel::saveTablesWorkshop(RepairOpType operation)
{
    qDebug().nospace() << "[" << this << "] saveTablesWorkshop(RepairOpType)";
    bool ret = 1;

    ret = saveTablesWorkshop();   // если по каким-либо причинам таблица не сохранена
    for(int i = 0; i < rowCount() && ret; i++)
    {
        qDebug().nospace() << "[" << this << "] saveTablesWorkshop(RepairOpType) | " << QString("i = %1").arg(i);
        if(index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool() == RecordType::Work)
            continue;

        SRepairSaleItemModel *itm = repairItem(i);
        switch(operation)
        {
            case RepairOpType::Sale: ret = itm->sale(); break;
            case RepairOpType::Unsale: ret = itm->unsale(); break;
            default:;
        }
    }

    if(!ret)
        throw 1;

    return ret;
}

bool SSaleTableModel::reserveItems()
{
    if(m_tableMode == TablesSet::StoreSale)
        return saveTablesStore(StoreOpType::Reserve);

    return saveTablesWorkshop(RepairOpType::Update);
}

/* Возврат товара
 * Допускается полный возврат товара (если пользователь не пометил отдельные cтроки)
 * или частичный. Также допускается многократный частичный возврат.
 */
bool SSaleTableModel::unsaleItems()
{
    if(m_tableMode == TablesSet::WorkshopSale)
        return true;

    if(m_itemsPendingRemoveList->isEmpty())
    {
        storeMarkAllItemsToRemove(StoreOpType::Unsale);
    }
    return storeBackOutItems(StoreOpType::Unsale);
}

/*
 * reason — причина возврата, указанная пользователем; будет записана в журнал
*/
bool SSaleTableModel::unsaleItems(const QString &reason)
{
    setExtraUnsaleReason(reason);
    unsaleItems();
}

void SSaleTableModel::setExtraUnsaleReason(const QString &reason)
{
    m_extraUnsaleReason = reason;
}

/* Полное снятие резерва
 * Очередь на удаление очищается и заполняется снова всеми строками
 */
bool SSaleTableModel::freeItems()
{
    if(m_tableMode == TablesSet::WorkshopSale)
        return true;

    m_itemsPendingRemoveList->clear();
    storeMarkAllItemsToRemove(StoreOpType::FreeReserved);
    return storeBackOutItems(StoreOpType::FreeReserved);
}

/*  В данном методе производится непосредственно возврат/снятие резерва
 */
bool SSaleTableModel::storeBackOutItems(StoreOpType type)
{
    bool ret = 1;
    m_itemsAffected = 0;

    QMap<int, int>::const_iterator i;
    for (i = m_itemsPendingRemoveList->constBegin(); i != m_itemsPendingRemoveList->constEnd() && ret; ++i)
    {
        m_itemsAffected++;
        SStoreSaleItemModel *itm = storeItem(i.key());
        if(type == StoreOpType::Unsale)
        {
            itm->setExtraUnsaleReason(m_extraUnsaleReason);
            ret = itm->unsale();
        }
        else
            ret = itm->free();

        delete itm;
    }

    if(ret)
        m_itemsPendingRemoveList->clear();
    else
        m_itemsAffected = 0;

    if(!ret)
        throw 1;

    return ret;
}

bool SSaleTableModel::repairRemoveRows()
{
    qDebug().nospace() << "[" << this << "] repairRemoveRows()";
    bool ret = 1;

    if(!m_itemsPendingRemoveList->isEmpty())
        ret = repairRemoveItems();
    if(!m_worksPendingRemoveList->isEmpty())
        ret = repairRemoveWorks();

    for(int i = rowCount() - 1; i >= 0 && ret; i--)
    {
        qDebug().nospace() << "[" << this << "] repairRemoveRows() | " << QString("ColState = %1 (i = %2)").arg(index(i, SStoreItemModel::SaleOpColumns::ColState).data().toInt()).arg(i);
        if(index(i, SStoreItemModel::SaleOpColumns::ColState).data().toInt())
            QStandardItemModel::removeRow(i);
    }
}

/*  Обработка очереди на удаление работ/товаров
 *  В случае сбоя вызывается исключение
*/
bool SSaleTableModel::repairRemoveItems()
{
    qDebug().nospace() << "[" << this << "] repairRemoveItems()";
    bool ret = 1;

    QMap<int, int>::const_iterator i;
    for (i = m_itemsPendingRemoveList->constBegin(); i != m_itemsPendingRemoveList->constEnd() && ret; ++i)
    {
        SRepairSaleItemModel *itm = repairItem(i.key());
        ret = itm->unlinkRepair();
        delete itm;
        if(!ret)
            throw 1;
    }
    m_itemsPendingRemoveList->clear();

    return ret;
}

bool SSaleTableModel::repairRemoveWorks()
{
    qDebug().nospace() << "[" << this << "] repairRemoveWorks()";
    bool ret = 1;

    QMap<int, int>::const_iterator i;
    for (i = m_worksPendingRemoveList->constBegin(); i != m_worksPendingRemoveList->constEnd() && ret; ++i)
    {
        SWorkModel *itm = repairWork(i.key());
        ret = itm->remove();
        delete itm;
        if(!ret)
            throw 1;
    }
    m_worksPendingRemoveList->clear();

    return ret;
}

void SSaleTableModel::sale()
{
    int state = 0;
    if(m_tableMode == TablesSet::WorkshopSale)
        return;

    saveTablesStore(StoreOpType::Sale);
}

int SSaleTableModel::itemsAffected()
{
    return m_itemsAffected;
}

/*  Помещает все строки таблицы в список на удаление
 */
void SSaleTableModel::storeMarkAllItemsToRemove(StoreOpType type)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(type == StoreOpType::Unsale)
            if(index(i, SStoreItemModel::SaleOpColumns::ColState).data().toBool())  // возвращённые ранее пропускаем; касается только простых продаж
                continue;

        removeRowHandler(i, index(i, SStoreItemModel::SaleOpColumns::ColId).data().toInt());
    }
}

SStoreSaleItemModel *SSaleTableModel::storeItem(const int rownum)
{
    SStoreSaleItemModel *item = new SStoreSaleItemModel(row(rownum), parent());
    return item;
}

SRepairSaleItemModel *SSaleTableModel::repairItem(const int rownum)
{
    SRepairSaleItemModel *item = new SRepairSaleItemModel(row(rownum));
    return item;
}

SWorkModel *SSaleTableModel::repairWork(const int rownum)
{
    SWorkModel *item = new SWorkModel(row(rownum));
    return item;
}

int SSaleTableModel::tableMode()
{
    return m_tableMode;
}

void SSaleTableModel::setTableMode(const TablesSet mode)
{
    m_tableMode = mode;
    if(mode == TablesSet::StoreSale)
    {
        m_hiddenColumns = 1 << SStoreItemModel::SaleOpColumns::ColUser | 1 << SStoreItemModel::SaleOpColumns::ColRealization | 1 << SStoreItemModel::SaleOpColumns::ColRetPercent | 1 << SStoreItemModel::SaleOpColumns::ColState | 1 << SStoreItemModel::SaleOpColumns::ColNotes | 1 << SStoreItemModel::SaleOpColumns::ColItemId | 1 << SStoreItemModel::SaleOpColumns::ColInPrice | 1 << SStoreItemModel::SaleOpColumns::ColObjId | 1 << SStoreItemModel::SaleOpColumns::ColDealer | 1 << SStoreItemModel::SaleOpColumns::ColBuyer | 1 << SStoreItemModel::SaleOpColumns::ColRecordType | 1 << SStoreItemModel::SaleOpColumns::ColCreated | 1 << SStoreItemModel::SaleOpColumns::ColWorkId;
        m_editStrategy = EditStrategy::OnManualSubmit;
    }
    else
    {
        m_hiddenColumns = 1 << SStoreItemModel::SaleOpColumns::ColAvail | 1 << SStoreItemModel::SaleOpColumns::ColRealization | 1 << SStoreItemModel::SaleOpColumns::ColRetPercent | 1 << SStoreItemModel::SaleOpColumns::ColState | 1 << SStoreItemModel::SaleOpColumns::ColNotes | 1 << SStoreItemModel::SaleOpColumns::ColItemId | 1 << SStoreItemModel::SaleOpColumns::ColInPrice | 1 << SStoreItemModel::SaleOpColumns::ColObjId | 1 << SStoreItemModel::SaleOpColumns::ColDealer | 1 << SStoreItemModel::SaleOpColumns::ColBuyer | 1 << SStoreItemModel::SaleOpColumns::ColRecordType | 1 << SStoreItemModel::SaleOpColumns::ColCreated | 1 << SStoreItemModel::SaleOpColumns::ColWorkId;
        m_editStrategy = EditStrategy::OnFieldChange;
    }
}

bool SSaleTableModel::isColumnHidden(const int column)
{
    return (m_hiddenColumns >> column)&0x01;
}

int SSaleTableModel::editStrategy()
{
    return m_editStrategy;
}

QList<QStandardItem *> SSaleTableModel::row(int row) const
{
    QList<QStandardItem *> rowItems;
    for(int column = 0; column < columnCount(); column++)
    {
        rowItems << QStandardItemModel::item(row, column);
    }

    return rowItems;
}

/* Возвращает номер строки для добавления товара.
*/
int SSaleTableModel::getItemInsertionRow()
{
    int rows = rowCount();

    // если таблица пуста, то добавляем произвольную работу
    if(!rows)
    {
        addCustomWork();
    }

    // если в таблице только одна запись или следующая строка соотв. тоже работе, то вставить деталь нужно перед ней
    if(m_currentIndex + 1 == rows || !recordType(m_currentIndex + 1))
    {
        return m_currentIndex + 1;
    }

    // с текущей выбранной строки производим поиск строки с работой
    for(int i = m_currentIndex+1; i < rows; i++)
    {
        if(!recordType(i))
        {
            return i;
        }
    }

    // если такая строка не найдена, то добавляем в конец таблицы
    return rows;
}

bool SSaleTableModel::recordType(const int row)
{

    return index(row, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool();
}

/* Установка id работы и номера ремонта
 * сброс значений производится в методе SRepairSaleItemModel::unlinkRepair()
*/
void SSaleTableModel::setItemWorkId(const int row, const int workId)
{
    if(index(row, SStoreItemModel::SaleOpColumns::ColWorkId).data() == workId)
        return;

    setData(index(row, SStoreItemModel::SaleOpColumns::ColWorkId), workId);
    setData(index(row, SStoreItemModel::SaleOpColumns::ColObjId), m_objId);
}

void SSaleTableModel::indexSelected(const QModelIndex &index)
{
    m_currentIndex = index.row();
}

/* т. к. некоторые данные обрабатываются по логическому номеру столбца, важно, чтобы они не изменились
 * В этом методе будет производиться сравнение со значениями из списка по умолчанию
 * TODO: вообще, нужны более универсальные способы получения данных конкретного столбца, но это потом...
 */
void SSaleTableModel::setHorizontalHeaderLabels(const QStringList &labels)
{

    for(int i = 0; i < labels.count(); i++)
    {
        Q_ASSERT_X(horizontalHeaderItem(i)->text() == m_fieldsDep.at(i), "SSaleTableModel::setHorizontalHeaderLabels()", "fields dependencies");
    }
    QStandardItemModel::setHorizontalHeaderLabels(labels);
}

/* Сумма всех товаров
 *
 */
double SSaleTableModel::amountTotal()
{
    m_amountTotal = 0;
    m_amountItems = 0;
    m_amountWorks = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(value(i, SStoreItemModel::SaleOpColumns::ColRecordType).toBool())
            m_amountItems += value(i, SStoreItemModel::SaleOpColumns::ColSumm).toFloat();
        else
            m_amountWorks += value(i, SStoreItemModel::SaleOpColumns::ColSumm).toFloat();

        m_amountTotal += value(i, SStoreItemModel::SaleOpColumns::ColSumm).toFloat();
    }
    return m_amountTotal;
}

QString SSaleTableModel::amountTotalLocale()
{
    return sysLocale.toString(amountTotal(), 'f', 2);
}

QString SSaleTableModel::amountItemsLocale()
{
    return sysLocale.toString(m_amountItems, 'f', 2);
}

QString SSaleTableModel::amountWorksLocale()
{
    return sysLocale.toString(m_amountWorks, 'f', 2);
}

QVariant SSaleTableModel::value(const int row, const int column, const int role) const
{
    return QStandardItemModel::data(index(row, column), role);
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для сохранённых данных будет достаточно вызвать метод this->setQuery()
 *
 */
void SSaleTableModel::sqlDataChanged()
{
    setRowCount(m_queryData->rowCount());
    setColumnCount(m_queryData->columnCount());
    m_itemsPendingRemoveList->clear();
    this->blockSignals(true);
    for(int i = 0; i < m_queryData->rowCount(); i++)
    {
        for(int j = 0; j < m_queryData->columnCount(); j++)
        {
//            QStandardItemModel::setHeaderData(j, Qt::Horizontal, m_queryData->headerData(j, Qt::Horizontal));
            QStandardItemModel::setData(index(i, j), m_queryData->index(i, j).data());
        }
        // Чтобы лишний раз не лезть в другие индексы объекта, в UserRole записываем значение is_cancellation (state);
        // оно будет использовано для отрисовки кнопки: товары, возврат которых был оформлен при предыдущем
        // администрировании документа, придётся продать в новом документе.
        QStandardItemModel::setData(index(i, 0), m_queryData->index(i, SStoreItemModel::SaleOpColumns::ColState).data(), DataRoles::State);
        QStandardItemModel::setData(index(i, 0), m_queryData->index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data(), DataRoles::RecordType);
    }
    this->blockSignals(false);
    m_queryData->blockSignals(true);
    m_queryData->clear();
    m_queryData->blockSignals(false);
    emit amountChanged(amountTotal(), m_amountItems, m_amountWorks);
    emit modelReset();
}

#if QT_VERSION >= 0x060000
void SSaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void SSaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    int row = topLeft.row();
//    qDebug().nospace() << "[" << this << "] dataChanaged() | TopLeft[R,C] = [" << topLeft.row() << "," << topLeft.column() << "] | BottomRight[R,C] = [" << bottomRight.row() << "," << bottomRight.column() << "] | changed flag = " << index(row, SStoreItemModel::SaleOpColumns::ColId).data(DataRoles::Changed ).toBool();
    int column = topLeft.column();
    if( (column == SStoreItemModel::SaleOpColumns::ColCount || column == SStoreItemModel::SaleOpColumns::ColPrice) /*&& m_amountChangedSignalFilter*/ )   // был глюк, что без фильтра при добавлении первого товара в таблицу не обновляется общая сумма документа
    {
        QStandardItemModel::setData(index(row, SStoreItemModel::SaleOpColumns::ColSumm), value(row, SStoreItemModel::SaleOpColumns::ColCount).toInt() * value(row, SStoreItemModel::SaleOpColumns::ColPrice).toFloat() );
        emit amountChanged(amountTotal(), m_amountItems, m_amountWorks);
    }
//    QStandardItemModel::setData(index(row, SStoreItemModel::SaleOpColumns::ColId), 1, DataRoles::Changed );
//    qDebug().nospace() << "[" << this << "] dataChanaged() | changed flag = " << index(row, SStoreItemModel::SaleOpColumns::ColId).data(DataRoles::Changed ).toBool();
}

bool SSaleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        // если ячейки "Кол-во" и "Доступно" ранее были подсвечены ошибочными, то после редактирования сбрасываем фоновый цвет
        if( index.column() == SStoreItemModel::SaleOpColumns::ColCount)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                QStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
                QStandardItemModel::setData(this->index(index.row(), SStoreItemModel::SaleOpColumns::ColAvail), QVariant(), Qt::BackgroundRole);
            }
        }

        // то же для ячекйки "Цена"
        if( index.column() == SStoreItemModel::SaleOpColumns::ColPrice)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                QStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
            }
        }

        if(index.data(Qt::DisplayRole) != value)
        {
            QStandardItemModel::setData(index, 1, Changed); // пометка поля изменённым
            QStandardItemModel::setData(this->index(index.row(), SStoreItemModel::SaleOpColumns::ColId), 1, Changed); // пометка строки изменённой
        }
        return QStandardItemModel::setData(index, value);
    }
    else
    {
        return QStandardItemModel::setData(index, value, role);
    }
    return false;
}

void SSaleTableModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_queryData->setQuery(query, db);
}

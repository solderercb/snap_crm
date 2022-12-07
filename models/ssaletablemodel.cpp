#include "ssaletablemodel.h"

SSaleTableModel::SSaleTableModel(QObject *parent) :
    QStandardItemModel(parent),
    m_queryData(new QSqlQueryModel)
{
    m_pendingRemoveList = new QMap<int, int>;

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
    delete m_pendingRemoveList;
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

    m_amountChangedSignalFilter = 0;  // при добавлении новой строки сигнал amountChanged генерировать не нужно
    if(!QStandardItemModel::insertRow(row))
        return false;

    for(int i = 0; i < record.count(); i++)
    {
        m_amountChangedSignalFilter = 1;
        if(i == SStoreItemModel::SaleOpColumns::ColUser)
            setData(index(row, i), userDbData->value("id").toInt());
        else
            setData(index(row, i), record.value(i));
    }

#ifdef QT_DEBUG
    int qtyLimit;
    if(record.value(SStoreItemModel::SaleOpColumns::ColAvail).toInt() > 5)
        qtyLimit = 5;
    else
        qtyLimit = record.value(SStoreItemModel::SaleOpColumns::ColAvail).toInt() + 1;

    int rand = QRandomGenerator::global()->bounded(qtyLimit);
    setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), rand?rand:1);
#endif

    return true;
}

bool SSaleTableModel::appendRecord(const QSqlRecord &record)
{
    return insertRecord(rowCount(), record);
}

void SSaleTableModel::removeRowHandler(const int row, const int db_id)
{
    if(m_tableMode == TablesSet::StoreSale)
    {
        storeSaleRemoveRow(row, db_id);
    }
    else
    {
        // пропуск уже обработанных; касается и работ и деталей
        if(index(row, SStoreItemModel::SaleOpColumns::ColState).data().toInt() == SRepairSaleItemModel::State::RepairLinked)
            workshopSaleRemoveRow(row, db_id);
        else
            return;
    }

    emit amountChanged(amountTotal(), m_amountItems, m_amountWorks);
    emit modelReset();
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
    // TODO: реализовать EditStrategy
    if(m_modelState == SSaleTableModel::StoreNew)   // в режиме создания новой РН просто удаляем строки из модели
        removeRows(row, 1);
    else    // в режимах просмотра резерва или проведённой накладной, помечаем строки на возврат/отмену резерва
    {
        if(m_pendingRemoveList->contains(row))
        {
            m_pendingRemoveList->remove(row);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), SStoreSaleItemModel::Active);
//            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), 0, Changed);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), value(row, SStoreItemModel::SaleOpColumns::ColCount, QtyBackup).toInt());   // восстанавливаем значение из UserRole
        }
        else
        {
            m_pendingRemoveList->insert(row, db_id);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), SStoreSaleItemModel::Cancelled);
//            setData(index(row, SStoreItemModel::SaleOpColumns::ColState), 1, Changed);
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), value(row, SStoreItemModel::SaleOpColumns::ColCount), QtyBackup); // при пометке на удаление, в UserRole сохраняем текущее кол-во; это на случай, если пользователь промахнулся строкой и тут же нажал кнопку еще раз
            setData(index(row, SStoreItemModel::SaleOpColumns::ColCount), 0);   // кол-во устанавливаем 0
        }
    }
}

/*  Удаление товара из списка в режиме ремонта
 * row  - номер строки
 * db_id - id записей в таблицах store_int_reserve и works
*/
void SSaleTableModel::workshopSaleRemoveRow(const int row, const int db_id)
{
    int new_state = SRepairSaleItemModel::State::EngineerBasket;
    if(index(row, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool() == RecordType::Work)   // сначала обрабатываем записи о товарах привязанных к работе
    {
        int i = row + 1;
        while(i < rowCount() && index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool() == RecordType::Item)
        {
            workshopSaleRemoveRow(i, index(i, SStoreItemModel::SaleOpColumns::ColId).data().toInt());
            i++;
        }
    }
    if(m_editStrategy == EditStrategy::OnManualSubmit)  // При ручном сохранении списка производится пометка строк на удаление
    {
        if(m_pendingRemoveList->contains(row))
        {
            m_pendingRemoveList->remove(row);
            new_state = SRepairSaleItemModel::State::RepairLinked;
        }
        else
        {
            m_pendingRemoveList->insert(row, db_id);
        }
        setData(index(row, SStoreItemModel::SaleOpColumns::ColState), new_state);
    }
    else
    {

    }
}

QMap<int, int>* SSaleTableModel::getPendingRemoveList()
{
    return m_pendingRemoveList;
}

int SSaleTableModel::pendingRemoveItemsCount()
{
    if(m_pendingRemoveList->isEmpty())
        return rowCount();

    return m_pendingRemoveList->size();
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
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC(m_documentId), QSqlDatabase::database("connMain"));
    else if( m_modelState == State::StoreReserved || m_modelState == State::StoreCancelled )
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(m_documentId), QSqlDatabase::database("connMain"));
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
    m_queryData->setQuery(QUERY_SEL_REPAIR_WORKS_AND_PARTS(m_repairId), QSqlDatabase::database("connMain"));

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
    m_documentId = id;
}

void SSaleTableModel::setRepairId(int id)
{
    m_repairId = id;
}

/*  Продажа (в т. ч. ранее зарезервированных) и резерв товаров.
 *  Вызов метода происходит в результате нажатия кнопки пользователем (т. е. сохранение не происходит автоматически)
 *  Допускается, что покупатель откажется от части зарезервированных товаров (помеченные пользователем строки)
 *  Возвращает 0 в случае ошибки
 */
bool SSaleTableModel::saveTablesStore(SaleOpType type)
{
    bool ret = 1;

    if(!m_pendingRemoveList->isEmpty())
        ret = storeBackOutItems(FreeReserved);

    m_itemsAffected = 0;
    for(int i = 0; i < rowCount() && ret && m_nIntegrityErr; i++)
    {
        if(index(i, SStoreItemModel::SaleOpColumns::ColState).data().toBool())  // частично снятые с резерва пропускаем
            continue;

        m_itemsAffected++;
        row(i).at(SStoreItemModel::SaleOpColumns::ColObjId)->setData(m_documentId, Qt::DisplayRole);
        row(i).at(SStoreItemModel::SaleOpColumns::ColBuyer)->setData(m_client, Qt::DisplayRole);
        SStoreSaleItemModel *itm = storeItem(i);
        if(type == Sale)
            ret = itm->sale();
        else
            ret = itm->reserve();

        delete itm;
    }

    if(!ret)
        throw 1;

    if(!m_nIntegrityErr)
        throw 2;

//    clearChangedFlags();
    return ret;
}

/*  Сохранение таблицы работ и деталей в ремонте.
 *  Вызов метода может происходить при завершении редактирования таблицы (т. е. автоматически после любого
 *  изменения; по умолчанию) или TODO: вручную, в зависимости от настроек пользователя.
 *  Возвращает 0 в случае ошибки
 */
bool SSaleTableModel::saveTablesWorkshop(RepairOpType operation)
{
    bool ret = 1;

    for(int i = 0; i < rowCount() && ret && m_nIntegrityErr; i++)
    {
        if(0)  // TODO: пропуск не изменённых
            continue;

        this->blockSignals(true);
        row(i).at(SStoreItemModel::SaleOpColumns::ColObjId)->setData(m_repairId, Qt::DisplayRole);
        row(i).at(SStoreItemModel::SaleOpColumns::ColBuyer)->setData(m_client, Qt::DisplayRole);
        this->blockSignals(false);
        if(index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool())
        {
            SRepairSaleItemModel *itm = repairItem(i);
            switch(operation)
            {
                case RepairOpType::Update: ret = itm->reserve(); break;
                case RepairOpType::Sale: ret = itm->sale(); break;
                case RepairOpType::Unsale: ret = itm->unsale(); break;
                case RepairOpType::Free: ret = itm->free(); break;
                default:;
            }
        }
        else
        {
            SWorkModel *itm = repairWork(i);
            switch(operation)
            {
                case RepairOpType::Update: ret = itm->update(); break;
                case RepairOpType::Free: ret = itm->remove(); break;
                default:;   // у записей в таблице works нет поля состояния и при выдаче ничего делать не нужно
            }
        }
    }

    if(!ret)
        throw 1;

    if(!m_nIntegrityErr)
        throw 2;

//    clearChangedFlags();
    return ret;
}

bool SSaleTableModel::reserveItems()
{
    if(m_tableMode == TablesSet::StoreSale)
        return saveTablesStore(Reserve);

    return saveTablesWorkshop(RepairOpType::Update);
}

/* Возврат товара
 * Допускается полный возврат товара (если пользователь не пометил отдельные cтроки)
 * или частичный. Также допускается многократный частичный возврат.
 */
bool SSaleTableModel::unsaleItems()
{
    if(m_pendingRemoveList->isEmpty())
    {
        markAllItemsToRemove(Unsale);
    }
    if(m_tableMode == TablesSet::StoreSale)
        return storeBackOutItems(Unsale);

    return workshopBackOutItems(Unsale);
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
 * Помеченные пользователем строки игнорируются
 */
bool SSaleTableModel::freeItems()
{
    m_pendingRemoveList->clear();
    markAllItemsToRemove(FreeReserved);
    if(m_tableMode == TablesSet::StoreSale)
        return storeBackOutItems(FreeReserved);

    return workshopBackOutItems(FreeReserved);
}

/*  В данном методе производится непосредственно возврат/снятие резерва
 */
bool SSaleTableModel::storeBackOutItems(BackOutOpType type)
{
    bool ret = 1;
    m_itemsAffected = 0;

    QMap<int, int>::const_iterator i;
    for (i = m_pendingRemoveList->constBegin(); i != m_pendingRemoveList->constEnd() && ret && m_nIntegrityErr; ++i)
    {
        m_itemsAffected++;
        SStoreSaleItemModel *itm = storeItem(i.key());
        if(type == Unsale)
        {
            itm->setExtraUnsaleReason(m_extraUnsaleReason);
            ret = itm->unsale();
        }
        else
            ret = itm->free();

        delete itm;
    }

    if(ret && m_nIntegrityErr)
        m_pendingRemoveList->clear();
    else
        m_itemsAffected = 0;

    if(!ret)
        throw 1;

    if(!m_nIntegrityErr)
        throw 2;

    return ret;
}

bool SSaleTableModel::workshopBackOutItems(BackOutOpType)
{

}

int SSaleTableModel::itemsAffected()
{
    return m_itemsAffected;
}

/*  Помещает все строки таблицы в список на удаление
 */
void SSaleTableModel::markAllItemsToRemove(BackOutOpType type)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(type == Unsale)
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

bool SSaleTableModel::integrityStatus()
{
    return  m_nIntegrityErr;
}

int SSaleTableModel::tableMode()
{
    return m_tableMode;
}

void SSaleTableModel::setTableMode(const TablesSet mode)
{
    m_tableMode = mode;
    if(mode == TablesSet::StoreSale)
        m_hiddenColumns = 1 << SStoreItemModel::SaleOpColumns::ColUser | 1 << SStoreItemModel::SaleOpColumns::ColRealization | 1 << SStoreItemModel::SaleOpColumns::ColRetPercent | 1 << SStoreItemModel::SaleOpColumns::ColState | 1 << SStoreItemModel::SaleOpColumns::ColNotes | 1 << SStoreItemModel::SaleOpColumns::ColItemId | 1 << SStoreItemModel::SaleOpColumns::ColInPrice | 1 << SStoreItemModel::SaleOpColumns::ColObjId | 1 << SStoreItemModel::SaleOpColumns::ColDealer | 1 << SStoreItemModel::SaleOpColumns::ColBuyer | 1 << SStoreItemModel::SaleOpColumns::ColRecordType | 1 << SStoreItemModel::SaleOpColumns::ColCreated | 1 << SStoreItemModel::SaleOpColumns::ColWorkId;
    else
        m_hiddenColumns = 1 << SStoreItemModel::SaleOpColumns::ColAvail | 1 << SStoreItemModel::SaleOpColumns::ColRealization | 1 << SStoreItemModel::SaleOpColumns::ColRetPercent | 1 << SStoreItemModel::SaleOpColumns::ColState | 1 << SStoreItemModel::SaleOpColumns::ColNotes | 1 << SStoreItemModel::SaleOpColumns::ColItemId | 1 << SStoreItemModel::SaleOpColumns::ColInPrice | 1 << SStoreItemModel::SaleOpColumns::ColObjId | 1 << SStoreItemModel::SaleOpColumns::ColDealer | 1 << SStoreItemModel::SaleOpColumns::ColBuyer | 1 << SStoreItemModel::SaleOpColumns::ColRecordType | 1 << SStoreItemModel::SaleOpColumns::ColCreated | 1 << SStoreItemModel::SaleOpColumns::ColWorkId;
}

bool SSaleTableModel::isColumnHidden(const int column)
{
    return (m_hiddenColumns >> column)&0x01;
}

int SSaleTableModel::editStrategy()
{
    return m_editStrategy;
}

void SSaleTableModel::clearChangedFlags()
{
    blockSignals(true);
    for(int i = 0; i < rowCount(); i++)
    {
        QStandardItemModel::setData(index(i, SStoreItemModel::SaleOpColumns::ColId), 0, DataRoles::Changed );
    }
    blockSignals(false);
}

QList<QStandardItem *> SSaleTableModel::row(int row) const
{
    QList<QStandardItem *> rowItems;
    for(int column = 0; column < columnCount(); column++)
    {
        // TODO: сделать выборочную передачу значений: для не новой РН нужно передавать только изменённые данные
//        if(m_modelState == State::New)
            rowItems << QStandardItemModel::item(row, column);
//        else
//        {
//            if(QStandardItemModel::item(row, column)->data(Changed).toBool())
//                rowItems << QStandardItemModel::item(row, column);
//        }
    }

    return rowItems;
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
    m_pendingRemoveList->clear();
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

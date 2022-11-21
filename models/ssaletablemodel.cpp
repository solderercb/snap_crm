#include "ssaletablemodel.h"

SSaleTableModel::SSaleTableModel(QObject *parent) :
    QStandardItemModel(parent),
    queryData(new QSqlQueryModel)
{
    pendingRemoveList = new QMap<int, int>;
    fields = new QMap<QString, int>;

    QStandardItemModel::setHorizontalHeaderLabels(fieldsDep);
    // TODO: заменить unicode emoji в заголовке на картинку для корректного отображения в Win 7
    setHorizontalHeaderLabels({"🗑","UID","Наименование","Кол-во","Доступно","Цена","Сумма","Место","Серийный номер","Гарантия"});

    connect(queryData, SIGNAL(modelReset()), this, SLOT(sqlDataChanged()));
#if QT_VERSION >= 0x060000
    connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &)), this, SLOT(dataChanaged(const QModelIndex&, const QModelIndex&, const QList<int> &)));
#else
    connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)), this, SLOT(dataChanaged(const QModelIndex&, const QModelIndex&, const QVector<int> &)));
#endif
}

SSaleTableModel::~SSaleTableModel()
{
    delete fields;
    delete pendingRemoveList;
    delete queryData;
}

QVariant SSaleTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
        case ColPrice:
        case ColSumm:
            {
                return sysLocale.toString(QStandardItemModel::data(index, role).toFloat(), 'f', 2);
            }
        case ColBox:
            {
                return itemBoxesModel->getDisplayRole(QStandardItemModel::data(index, role).toInt(), 1);
            }
        case ColWarranty:
            {
                return warrantyTermsModel->getDisplayRole(QStandardItemModel::data(index, role).toInt(), 1);
            }
        }
    }
    if ( role == Qt::BackgroundRole )
    {
        if(value(index.row(), "is_cancellation").toBool())
            return QColor("light gray");
    }

    return QStandardItemModel::data(index, role);
}

Qt::ItemFlags SSaleTableModel::flags(const QModelIndex &index) const
{
    if( m_modelState == State::New || m_modelState == State::Reserved )   // у вкладки будет дополнительный режим — правка резерва (в АСЦ такого вроде не было)
    {
        switch (index.column()) {
        case ColCount:
        case ColPrice:
        case ColSN:
        case ColWarranty:
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

bool SSaleTableModel::insertRecord(int row, const QSqlRecord &record)
{
    bool firstRun = 0;
    Q_ASSERT_X(record.count() == columnCount(), "insertRecord()", "different column count");

    if(fields->size() == 0)
        firstRun = 1;

    amountChangedSignalFilter = 0;  // при добавлении новой строки сигнал amountChanged генерировать не нужно
    if(!QStandardItemModel::insertRow(row))
        return false;

    for(int i = 0; i < record.count(); i++)
    {
        if(firstRun)
        {
            fields->insert(record.fieldName(i), i);
        }

        if(!firstRun && record.fieldName(i) != fields->key(i))
        {
            return false;
        }
        amountChangedSignalFilter = 1;
        setData(index(row, i), record.value(i));

    }
#ifdef QT_DEBUG
    int qtyLimit;
    if(record.value(fields->value("avail")).toInt() > 5)
        qtyLimit = 5;
    else
        qtyLimit = record.value(fields->value("avail")).toInt() + 1;

    int rand = QRandomGenerator::global()->bounded(qtyLimit);
    setData(index(row, ColCount), rand?rand:1);
#endif

    return true;
}

bool SSaleTableModel::appendRecord(const QSqlRecord &record)
{
    return insertRecord(rowCount(), record);
}

/* Пометка строки на удаление: частичный возврат или частичная отмена ранее зарезервированного товара.
 * При первом нажатии кнопки Удалить, в массив добавятся данные, а при повторном - удалятся (на случай, если пользователь промахнулся).
 * При подтверждении возврата или проведении РН резерва, записи с соответствующими id будут обновлены, будет установлен флаг is_cancellation.
 * row  - номер строки
 * db_id - id записи в таблице store_sales
 */
bool SSaleTableModel::removeRowHandler(int row, int db_id)
{
    if(pendingRemoveList->contains(row))
    {
        pendingRemoveList->remove(row);
        setData(index(row, ColIsCancellation), 0);
//        setData(index(row, ColIsCancellation), 0, Changed);
        setData(index(row, ColCount), value(row, "count", QtyBackup).toInt());   // восстанавливаем значение из UserRole
    }
    else
    {
        pendingRemoveList->insert(row, db_id);
        setData(index(row, ColIsCancellation), 1);
//        setData(index(row, ColIsCancellation), 1, Changed);
        setData(index(row, ColCount), value(row, "count"), QtyBackup); // при пометке на удаление, в UserRole сохраняем текущее кол-во; это на случай, если пользователь промахнулся строкой и тут же нажал кнопку еще раз
        setData(index(row, ColCount), 0);   // кол-во устанавливаем 0
    }
    emit modelReset();

    return 0;
}

QMap<int, int>* SSaleTableModel::getPendingRemoveList()
{
    return pendingRemoveList;
}

int SSaleTableModel::pendingRemoveItemsCount()
{
    if(pendingRemoveList->isEmpty())
        return rowCount();

    return pendingRemoveList->size();
}

/* Обновление цены
 * в данном методе для всех товаров, уже добавленных в таблицу, будут запрошен соответствующая колонка цен
 */
void SSaleTableModel::setPriceColumn(QSqlQuery *query)
{
    QString qry = query->lastQuery();   // для проверки правильности запроса
    int  item_id;
    item_id = fields->value("item_id");

    Q_ASSERT_X(qry.contains(":id", Qt::CaseSensitive), "SSaleTableModel::setPriceColumn(QSqlQuery *)", "в подготовленном запросе не задан (или задан не подходящий) placeholder");
    for(int i = 0; i < rowCount(); i++)
    {
        query->bindValue(":id", index(i,  item_id).data().toInt());
        query->exec();
        query->first();
        setData(index(i,  ColPrice), query->record().value(0).toFloat());
    }
}

/* Загрузка таблицы товаров ранее сохранённого документа
 * Возвращает 1 в случае ошибки
*/
bool SSaleTableModel::load(int doc_id)
{
    documentId = doc_id;
    if(m_modelState == State::Payed)
        queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC(documentId), QSqlDatabase::database("connMain"));
    else if( m_modelState == State::Reserved || m_modelState == State::Cancelled )
        queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(documentId), QSqlDatabase::database("connMain"));
    else
        return 1;

    return 0;
}

void SSaleTableModel::setClient(int id)
{
//    values.insert("client", QString::number(id));
    m_client = id;
}

void SSaleTableModel::unsetClient()
{
//    values.insert("client", "NULL");
    m_client = 0;
}

void SSaleTableModel::setDocumentId(int id)
{
//    values.insert("document_id", QString::number(id));
    documentId = id;
}

/*  Продажа (в т. ч. ранее зарезервированных) и резерв товаров.
 *  Допускается, что покупатель откажется от части зарезервированных товаров (помеченные пользователем строки)
 *  Возвращает 0 в случае ошибки
 */
bool SSaleTableModel::saleItems(SaleOpType type)
{
    bool ret = 1;

    if(!pendingRemoveList->isEmpty())
        ret = backOutItems(FreeReserved);

    m_itemsAffected = 0;
    for(int i = 0; i < rowCount() && ret && nIntegrityErr; i++)
    {
        if(index(i, ColIsCancellation).data().toBool())  // частично снятые с резерва пропускаем
            continue;

        m_itemsAffected++;
        SSaleItemModel *itm = item(i);
        itm->setClient(m_client);
        itm->setDocumentId(documentId);
        itm->setMode(SSaleItemModel::RegularSales);
        if(type == Sale)
            ret = itm->sale();
        else
            ret = itm->reserve();

        nIntegrityErr = itm->integrityStatus();

        delete itm;
    }

    if(!ret)
        throw 1;

    if(!nIntegrityErr)
        throw 2;

    return ret;
}

bool SSaleTableModel::reserveItems()
{
    return saleItems(Reserve);
}

/* Возврат товара
 * Допускается полный возврат товара (если пользователь не пометил отдельные cтроки)
 * или частичный. Также допускается многократный частичный возврат.
 */
bool SSaleTableModel::unsaleItems()
{
    bool ret = 1;

    if(pendingRemoveList->isEmpty())
    {
        markAllItemsToRemove(Unsale);
    }
    ret = backOutItems(Unsale);

    return ret;
}

bool SSaleTableModel::unsaleItems(const QString &reason)
{
    setUnsaleReason(reason);
    unsaleItems();
}

void SSaleTableModel::setUnsaleReason(const QString &reason)
{
    m_unsaleReason = reason;
}

/* Полное снятие резерва
 * Помеченные пользователем строки игнорируются
 */
bool SSaleTableModel::freeItems()
{
    bool ret = 1;

    pendingRemoveList->clear();
    markAllItemsToRemove(FreeReserved);
    ret = backOutItems(FreeReserved);

    return ret;
}

/*  В данном методе производится непосредственно возврат/снятие резерва
 */
bool SSaleTableModel::backOutItems(BackOutOpType type)
{
    bool ret = 1;
    m_itemsAffected = 0;

    QMap<int, int>::const_iterator i;
    for (i = pendingRemoveList->constBegin(); i != pendingRemoveList->constEnd() && ret && nIntegrityErr; ++i)
    {
        m_itemsAffected++;
        SSaleItemModel *itm = item(i.key());
        itm->setMode(SSaleItemModel::RegularSales);
        if(type == Unsale)
        {
            itm->setUnsaleReason(m_unsaleReason);
            ret = itm->unsale();
        }
        else
            ret = itm->free();

        nIntegrityErr = itm->integrityStatus();

        delete itm;
    }

    if(ret && nIntegrityErr)
        pendingRemoveList->clear();
    else
        m_itemsAffected = 0;

    if(!ret)
        throw 1;

    if(!nIntegrityErr)
        throw 2;

    return ret;
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
            if(index(i, ColIsCancellation).data().toBool())  // возвращённые ранее пропускаем
                continue;

        removeRowHandler(i, index(i, ColId).data().toInt());
    }
}

SSaleItemModel *SSaleTableModel::item(const int rownum)
{
    SSaleItemModel *item = new SSaleItemModel(row(rownum), parent());
    return item;
}

bool SSaleTableModel::integrityStatus()
{
    return  nIntegrityErr;
}

QList<QStandardItem *> SSaleTableModel::row(int row)
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
 * В этом методе будет производиться сравнение со значениями из списока по умолчанию
 * TODO: вообще, нужны более универсальные способы получения данных конкретного столбца, но это потом...
 */
void SSaleTableModel::setHorizontalHeaderLabels(const QStringList &labels)
{

    for(int i = 0; i < labels.count(); i++)
    {
        Q_ASSERT_X(horizontalHeaderItem(i)->text() == fieldsDep.at(i), "SSaleTableModel::setHorizontalHeaderLabels()", "fields dependencies");
    }
    QStandardItemModel::setHorizontalHeaderLabels(labels);
}

/* Сумма всех товаров
 *
 */
double SSaleTableModel::totalAmount()
{
    double amount = 0;
    int isCancellationCol = fields->value("is_cancellation");
    for(int i = 0; i < rowCount(); i++)
    {
        amount += value(i, ColSumm).toFloat() * !value(i, isCancellationCol).toBool();
    }
    return amount;
}

QString SSaleTableModel::totalAmountLocal()
{
    return sysLocale.toString(totalAmount(), 'f', 2);
}

QVariant SSaleTableModel::value(int row, int column) const
{
    return QStandardItemModel::data(index(row, column));
}

QVariant SSaleTableModel::value(int row, QString field) const
{
    return QStandardItemModel::data(index(row, fields->value(field)));
}

QVariant SSaleTableModel::value(int row, QString field, int role) const
{
    return QStandardItemModel::data(index(row, fields->value(field)), role);
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для просмотра проведённого документа будет достаточно вызвать метод this->setQuery()
 *
 */
void SSaleTableModel::sqlDataChanged()
{
    setRowCount(queryData->rowCount());
    setColumnCount(queryData->columnCount());
    pendingRemoveList->clear();
    fields->clear();
    for(int j = 0; j < queryData->columnCount(); j++)
    {
        fields->insert(queryData->headerData(j, Qt::Horizontal).toString(), j);
//        QStandardItemModel::setHeaderData(j, Qt::Horizontal, queryData->headerData(j, Qt::Horizontal));
    }
    for(int i = 0; i < queryData->rowCount(); i++)
    {
        for(int j = 0; j < queryData->columnCount(); j++)
        {
//            QStandardItemModel::setHeaderData(j, Qt::Horizontal, queryData->headerData(j, Qt::Horizontal));
            QStandardItemModel::setData(index(i, j), queryData->index(i, j).data());
        }
        // В UserRole записываем значение is_cancellation (чтобы лишний раз не лезть в другие индексы объекта);
        // оно будет использовано для отрисовки кнопки: товары, возврат которых был оформлен при предыдущем
        // администрировании документа, придётся продать в новом документе.
        QStandardItemModel::setData(index(i, 0), queryData->index(i, fields->value("is_cancellation")).data(), Qt::UserRole);
    }
    queryData->blockSignals(true);
    queryData->clear();
    queryData->blockSignals(false);
    emit modelReset();
}

#if QT_VERSION >= 0x060000
void SSaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void SSaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    int row = topLeft.row();
    int column = topLeft.column();
    if( (column == ColCount || column == ColPrice) /*&& amountChangedSignalFilter*/ )   // был глюк, что без фильтра при добавлении первого товара в таблицу не обновляется общая сумма документа
    {
        setData(index(row, ColSumm), value(row, ColCount).toInt() * value(row, ColPrice).toFloat() );
        emit amountChanged(totalAmount());
    }
}

bool SSaleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        // если ячейки "Кол-во" и "Доступно" ранее были подсвечены ошибочными, то после редактирования сбрасываем фоновый цвет
        if( index.column() == ColCount)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                QStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
                QStandardItemModel::setData(this->index(index.row(), fields->value("avail")), QVariant(), Qt::BackgroundRole);
            }
        }

        // то же для ячекйки "Цена"
        if( index.column() == ColPrice)
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
    queryData->setQuery(query, db);
}

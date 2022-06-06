#include "saletablemodel.h"

SaleTableModel::SaleTableModel(QObject *parent) :
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

SaleTableModel::~SaleTableModel()
{
    delete fields;
    delete pendingRemoveList;
}

QVariant SaleTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
        case 5:
        case 6:
            {
                return sysLocale.toString(QStandardItemModel::data(index, role).toFloat(), 'f', 2);
            }
        case 7:
            {
                return itemBoxesModel->getDisplayRole(QStandardItemModel::data(index, role).toInt(), 1);
            }
        case 9:
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

Qt::ItemFlags SaleTableModel::flags(const QModelIndex &index) const
{
    if( modelState == 0 || modelState == 2 )   // у вкладки будет несколько режимов′: создание новой РН, правка резерва (в АСЦ такого вроде не было) и просмотр проведённой
    {
        switch (index.column()) {
        case 3:
        case 5:
        case 8:
        case 9:
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

bool SaleTableModel::insertRecord(int row, const QSqlRecord &record)
{
//    qDebug() << "SaleTableModel::insertRecord";
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
            if(record.fieldName(i) == "count")
                countCol = i;
            if(record.fieldName(i) == "price")
                priceCol = i;
            if(record.fieldName(i) == "summ")
                summCol = i;
        }

        if(!firstRun && record.fieldName(i) != fields->key(i))
        {
            qDebug() << "название столбца не совпадает: record.fieldName(i) =" << record.fieldName(i) << "; fields->key(i) =" << fields->key(i);
            return false;
        }
        amountChangedSignalFilter = 1;
        setData(index(row, i), record.value(i));

    }
#ifdef QT_DEBUG
    int rand = QRandomGenerator::global()->bounded(record.value(fields->value("avail")).toInt() + 1);
    setData(index(row, countCol), rand?rand:1);
#endif

    return true;
}

bool SaleTableModel::appendRecord(const QSqlRecord &record)
{
    return insertRecord(rowCount(), record);
}

/* Пометка строки на удаление: частичный возврат или частичная отмена ранее зарезервированного товара.
 * При первом нажатии кнопки Удалить, в массив добавятся данные, а при повторном - удалятся (на случай, если пользователь промахнулся).
 * При подтверждении возврата или проведении РН резерва, записи с соответствующими id будут обновлены, будет установлен флаг is_cancellation.
 * row  - номер строки
 * db_id - id записи в таблице store_sales
 */
bool SaleTableModel::removeRowHandler(int row, int db_id)
{
    if(pendingRemoveList->contains(row))
    {
        pendingRemoveList->remove(row);
        setData(index(row, fields->value("is_cancellation")), 0);
        setData(index(row, fields->value("count")), value(row, "count", Qt::UserRole).toInt());   // восстанавливаем значение из UserRole
    }
    else
    {
        pendingRemoveList->insert(row, db_id);
        setData(index(row, fields->value("is_cancellation")), 1);
        setData(index(row, fields->value("count")), value(row, "count"), Qt::UserRole); // при пометке на удаление, в UserRole сохраняем текущее кол-во; это на случай, если пользователь промахнулся строкой и тут же нажал кнопку еще раз
        setData(index(row, fields->value("count")), 0);   // кол-во устанавливаем 0
    }
    emit modelReset();

    return 0;
}

QMap<int, int>* SaleTableModel::getPendingRemoveList()
{
    return pendingRemoveList;
}

bool SaleTableModel::revertMode()
{
    if(pendingRemoveList->size() == 0)    // Если ни один товар не помечен на удаление, то распроводим всю накладную
    {
        return 1;
    }
    else
        return 0;
}

/* Обновление цены
 * в данном методе для всех товаров, уже добавленных в таблицу, будут запрошен соответствующая колонка цен
 */
void SaleTableModel::setPriceColumn(QSqlQuery *query)
{
//    qDebug() << "SaleTableModel::setPriceColumn()";
    QString qry = query->lastQuery();   // для проверки правильности запроса
    int  item_id;
    item_id = fields->value("item_id");

    Q_ASSERT_X(qry.contains(":id", Qt::CaseSensitive), "SaleTableModel::setPriceColumn(QSqlQuery *)", "в подготовленном запросе не задан (или задан не подходящий) placeholder");
    for(int i = 0; i < rowCount(); i++)
    {
        query->bindValue(":id", index(i,  item_id).data().toInt());
        query->exec();
        query->first();
        setData(index(i,  priceCol), query->record().value(0).toFloat());
    }
}

/* т. к. некоторые данные обрабатываются по логическому номеру столбца, важно, чтобы они не изменились
 * В этом методе будет производиться сравнение со значениями из списока по умолчанию
 * TODO: вообще, нужны более универсальные способы получения данных конкретного столбца, но это потом...
 */
void SaleTableModel::setHorizontalHeaderLabels(const QStringList &labels)
{

    for(int i = 0; i < labels.count(); i++)
    {
        Q_ASSERT_X(horizontalHeaderItem(i)->text() == fieldsDep.at(i), "SaleTableModel::setHorizontalHeaderLabels()", "fields dependencies");
    }
    QStandardItemModel::setHorizontalHeaderLabels(labels);
}

/* Сумма всех товаров
 *
 */
double SaleTableModel::totalAmount()
{
    double amount = 0;
    int isCancellationCol = fields->value("is_cancellation");
    for(int i = 0; i < rowCount(); i++)
    {
        amount += value(i, summCol).toFloat() * !value(i, isCancellationCol).toBool();
    }
    return amount;
}

QVariant SaleTableModel::value(int row, int column) const
{
    return QStandardItemModel::data(index(row, column));
}

QVariant SaleTableModel::value(int row, QString field) const
{
    return QStandardItemModel::data(index(row, fields->value(field)));
}

QVariant SaleTableModel::value(int row, QString field, int role) const
{
    return QStandardItemModel::data(index(row, fields->value(field)), role);
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для просмотра проведённого документа будет достаточно вызвать метод this->setQuery()
 *
 */
void SaleTableModel::sqlDataChanged()
{
//    qDebug() << "SaleTableModel::sqlDataChanged()";
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
void SaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void SaleTableModel::dataChanaged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    int row = topLeft.row();
    int column = topLeft.column();
    if( (column == countCol || column == priceCol) && amountChangedSignalFilter )   // без фильтра при добавлении первого товара в таблицу не обновляется общая сумма документа
    {
        setData(index(row, summCol), value(row, countCol).toInt() * value(row, priceCol).toFloat() );
        emit amountChanged(totalAmount());
    }
}

bool SaleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        // если ячейки "Кол-во" и "Доступно" ранее были подсвечены ошибочными, то после редактирования сбрасываем фоновый цвет
        if( index.column() == countCol)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                QStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
                QStandardItemModel::setData(this->index(index.row(), fields->value("avail")), QVariant(), Qt::BackgroundRole);
            }
        }

        // то же для ячекйки "Цена"
        if( index.column() == priceCol)
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

void SaleTableModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    queryData->setQuery(query, db);
}

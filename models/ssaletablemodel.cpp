#include "ssaletablemodel.h"
#include <QSqlQuery>
#include <QSqlField>
#include <QMetaEnum>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SAppLog>
#include <SUserSettings>
#include <SPermissions>
#include <SComSettings>
#include <SSqlQueryModel>
#include <SStoreSaleItemModel>
#include <SRepairSaleItemModel>
#include <SWorkModel>
#include <SCartridgeCardModel>
#include <FlashPopup>
#if defined QT_DEBUG || defined S_TEST
#include <SQueryLog>
#include <QRandomGenerator>
#endif

SSaleTableModel::SSaleTableModel(QObject *parent) :
    SStandardItemModel(parent),
    m_queryData(new QSqlQueryModel)
{
    m_standardItemModel = this;
    setHorizontalHeaderLabels();

    connect(m_queryData, &QSqlQueryModel::modelReset, this, &SSaleTableModel::sqlDataChanged);
}

SSaleTableModel::~SSaleTableModel()
{
    delete m_queryData;
}

void SSaleTableModel::initDemo()
{
    addCustomWork();
    setData(index(0, Columns::Name), "<work>");
    setData(index(0, Columns::Count), 5);
    setData(index(0, Columns::Price), 100);
    setData(index(0, Columns::Amount), 500);
    setData(index(0, Columns::SN), "SN123456");
    addCustomWork();
    setData(index(1, Columns::Name), "<item 1>");
    setData(index(1, Columns::Count), 10);
    setData(index(1, Columns::Price), 50);
    setData(index(1, Columns::Amount), 500);
    setData(index(1, Columns::SN), "SN654321");
    setData(index(1, Columns::RecordType), SSaleTableModel::RecordType::Item);
    addCustomWork();
    setData(index(2, Columns::Name), "<item 2>");
    setData(index(2, Columns::Count), 20);
    setData(index(2, Columns::Price), 50);
    setData(index(2, Columns::Amount), 1000);
    setData(index(2, Columns::Warranty), warrantyTermsModel->index(4, 1).data().toInt());
    setData(index(2, Columns::User), engineersModel->index(0, 1).data().toInt());
    setData(index(2, Columns::RecordType), SSaleTableModel::RecordType::Item);
}

QVariant SSaleTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case Columns::Price: return dataLocalizedFromDouble(index);
            case Columns::Amount: return dataLocalizedFromDouble(amountItem(index.row()));
            case Columns::Box: return itemBoxesModel->getDisplayRole(unformattedData(index).toInt(), 1);
            case Columns::Warranty: return warrantyFromId(index);
            case Columns::User: return allUsersMap->value(unformattedData(index).toInt());
            default: break;
        }
    }
    else if ( role == Qt::BackgroundRole )
    {
        if(isRowMarkedRemove(index.row()))
            return QColor("light gray");
    }
    else if(role == Qt::ToolTipRole)
    {
        switch (index.column()) {
            case Columns::Id: break;
            default: return data(index, Qt::DisplayRole);
        }
    }

    return SStandardItemModel::data(index, role & 0xFF);
}

double SSaleTableModel::amountItem(const int row) const
{
    return price(row) * count(row);
}

Qt::ItemFlags SSaleTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QModelIndex SSaleTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QStandardItemModel::index(row, column, parent);
}

bool SSaleTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ret = SStandardItemModel::setData(index, value, role);
    switch (index.column())
    {
        case Columns::Count:
        case Columns::Price: rowAmountUpdate(index); calculateAmounts(); break;
        default: break;
    }

    return ret;
}

/* Добавление произвольной работы в конец таблицы
*/
void SSaleTableModel::addCustomWork()
{
    auto customWork = std::make_unique<QSqlRecord>();

    for(int i = 0; i < SSaleTableModel::staticMetaObject.enumerator(SSaleTableModel::staticMetaObject.indexOfEnumerator("Columns")).keyCount(); i++)
    {
        auto field = std::make_unique<QSqlField>("", QVariant::Int);
        switch(i)
        {

            case Columns::Id: field->setValue(0); break;
            case Columns::UID: field->setValue(""); break;
            case Columns::Name: field->setValue(""); break;
            case Columns::Count: field->setValue(1); break;
            case Columns::Avail: field->setValue("999999"); break;
            case Columns::Price: field->setValue(0); break;
            case Columns::Amount: field->setValue(0); break;
            case Columns::Box: field->setValue(0); break;
            case Columns::SN: field->setValue(""); break;
            case Columns::Warranty: field->setValue(0); break;
            case Columns::User: field->setValue(userDbData->id()); break;
            case Columns::Realization: field->setValue(0); break;
            case Columns::RetPercent: field->setValue(0); break;
            case Columns::State: field->setValue(0); break;
            case Columns::Notes: field->setValue(""); break;
            case Columns::ItemId: field->setValue(0); break;
            case Columns::InPrice: field->setValue(0); break;
            case Columns::ObjId: field->setValue(m_objId); break;
            case Columns::Dealer: field->setValue(0); break;
            case Columns::Buyer: field->setValue(0); break;
            case Columns::Created: field->setValue(QDateTime::currentDateTimeUtc()); break;
            case Columns::WorkId: field->setValue(0); break;
            case Columns::RecordType: field->setValue(RecordType::Work); break;
            case Columns::WorkType: field->setValue(0); break;
        }

        customWork->append(*field);
    }

    m_currentIndex = rowCount();
    int type = RecordType::Work;
    insertRows(m_currentIndex, 1);
    setRecordType(m_currentIndex, type);
    auto model = RecordFactory::instance().create(saleMode() | type);
    if(!model)
        return;

    setCacheItem(m_currentIndex, model);
    if(!setSingleRowModelData(*customWork, model)) // копирование данных из QSqlRecord в SSingleRowJModel
        return;

    if(!endModelChange())
        SStandardItemModel::removeRow(m_currentIndex);

    // пересчет итогов и эмиссия сигнала amountChanged не нужны, т. к. стоимость работы 0

    if(m_currentIndex == 0) // При добавлении первой строки в таблицу нужно послать сигнал modelReset.
        endResetModel();
}

/* Добавление работы из прайс-листа
*/
bool SSaleTableModel::addWorkByID(const int id, const int priceOption)
{
    if(!isModelEditable())
        return 0;

    auto work = std::make_unique<QSqlQuery>(loadConnection());
    bool ret = 1;

    work->exec(QUERY_SEL_WORK_FROM_PRICELIST(id, workPriceColModel->index(priceOption, 2).data().toString()));
    if(!work->first())
        return 0;

    m_currentIndex = rowCount();
    int type = RecordType::Work;
    insertRows(m_currentIndex, 1);
    setRecordType(m_currentIndex, type);
    auto model = RecordFactory::instance().create(saleMode() | type);
    if(!model)
        return 0;

    setCacheItem(m_currentIndex, model);
    if(!setSingleRowModelData(work->record(), model))
        return 0; // копирование данных из QSqlRecord в SSingleRowJModel
    setData(index(m_currentIndex, Columns::User), userDbData->id());

    if(!endModelChange())
        SStandardItemModel::removeRow(m_currentIndex);

    calculateAmounts();

    if(m_currentIndex == 0) // При добавлении первой строки в таблицу нужно послать сигнал modelReset.
        endResetModel();

    return ret;
}

void SSaleTableModel::flashPopupInfoMessage(const QString &caption, const QString &text)
{
    auto p = new shortlivedNotification(this,
                                        caption,
                                        text,
                                        QColor(255,255,255),
                                        QColor(245,245,245));
    Q_UNUSED(p)
}

bool SSaleTableModel::checkBeforeItemAdd(const int id, const int count)
{
    auto query = std::unique_ptr<QSqlQuery>();
    QString msgCaption, msgText;

    query = std::make_unique<QSqlQuery>(loadConnection());
    query->exec(QUERY_SEL_ITEM_ACTUAL_QTY(id));
    if(!query->first())
    {
        auto p = new shortlivedNotification(this, tr("Ошибка"), tr("Товар с указанным ID не найден"), QColor("#FFC7AD"), QColor("#FFA477"));
        Q_UNUSED(p);
        return 0;
    }

    const QSqlRecord &record = query->record();

    if(count > (record.value(0).toInt() - record.value(2).toInt())) // `count` - `reserved`
    {
        query->exec(QUERY_SEL_STORE_ITEM_UID_NAME(id));
        query->first();
        qDebug() << QString("Запрошенное кол-во товара UID %1 не доступно")
                        .arg(query->record().value(0).toString());
        msgCaption = tr("Товар отсутствует");
        msgText = tr("Запрошенное кол-во товара \"%1\" (UID %2) не доступно для продажи").arg(
                      query->record().value(1).toString(),
                      query->record().value(0).toString());

        flashPopupInfoMessage(msgCaption, msgText);
        return 0;
    }

    query->exec(QUERY_SEL_PART_WAREHOUSE(id));
    query->first();
    if(query->record().value(0).toInt() != userDbData->currentOffice())
    {
        QString debugText = QString("Запрошенный товар UID %1 числится на складе другого офиса: %2, офис пользователя: %3")
                                .arg(record.value(Columns::UID).toString())
                                .arg(query->record().value(0).toInt())
                                .arg(userDbData->currentOffice());
        appLog->appendRecord(debugText);
        qDebug() << debugText;
        msgCaption = tr("Информация");
        msgText = tr("Товар числится на складе другого офиса");

        flashPopupInfoMessage(msgCaption, msgText);
        return 0;
    }

    return 1;
}

/*  Добавление товара по id
 *  В режиме простой продажи нельзя добавить один и тот же товар дважды (TODO: пересмотреть это решение)
 *  возвращает 0 в случае ошибки
*/
bool SSaleTableModel::addItemByID(const int id, const SStoreItemModel::PriceOption priceOption, const int count)
{
    if(!isModelEditable())
        return 0;

    if(!checkBeforeItemAdd(id, count))  // специфические проверки производной модели
        return 0;
    if(!SSaleTableModel::checkBeforeItemAdd(id, count)) // общие проверки
        return 0;

    bool ret = 1;
    auto query = std::make_unique<QSqlQuery>(loadConnection());
    query->exec(QUERY_SEL_PART_FOR_SALE(id, priceColModel->value(priceOption, "id", "dbColumn").toString(), count));
    if(!query->first())
        return false;

    int row = itemInsertionRow();
    int type = RecordType::Item;
    insertRows(row, 1);
    setRecordType(row, type);
    auto model = RecordFactory::instance().create(saleMode() | type);
    if(!model)
        return 0;

    setCacheItem(row, model);
    if(!setSingleRowModelData(query->record(), model))
        return 0; // копирование данных из QSqlRecord в SSingleRowJModel
    SSaleTableModel::setData(index(row, Columns::User), userDbData->id());
    itemAddHook(row);

    if(!endModelChange())
        SStandardItemModel::removeRow(row);

    calculateAmounts();
    return ret;
}

/*  Добавление товара по id
 *  Это перегруженный метод; товар добавляется с ценой, заданной при вызове метода setPriceColumn()
 *  возвращает 0 в случае неудачи
 */
bool SSaleTableModel::addItemByID(const int id, const int count)
{
    return addItemByID(id, m_priceColumnId, count);
}

void SSaleTableModel::endRemoveRow()
{
    calculateAmounts();
    endResetModel();    // генерация сигнала modelReset() нужна для корректной раскраски строки, помеченной на удаление
}

/* Обновление цены
 * в данном методе для всех товаров, уже добавленных в таблицу, будут запрошен соответствующая колонка цен
 */
void SSaleTableModel::setPriceColumn(const SStoreItemModel::PriceOption id)
{
    m_priceColumnId = id;
    if(rowCount() == 0)
        return;

    auto query = std::make_unique<QSqlQuery>(loadConnection());

    query->prepare(QUERY_SEL_STORE_ITEMS_ITEM_PRICE(priceColModel->value(id, "id", "dbColumn").toString()));
    for(int i = 0; i < rowCount(); i++)
    {
        query->bindValue(":id", index(i,  Columns::ItemId).data().toInt());
        query->exec();
        query->first();
        setData(index(i,  Columns::Price), query->record().value(0).toDouble());
    }

    calculateAmounts();
}

void SSaleTableModel::setClient(int id)
{
    m_client = id;
}

void SSaleTableModel::unsetClient()
{
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

std::shared_ptr<SSingleRowJModel> SSaleTableModel::singleRowModel(const int row)
{
    auto model = cacheItem(row);
    if(model)
    {
        return model;
    }

    int type = recordType(row);
    model = RecordFactory::instance().create(saleMode() | type);
    if(model)
    {
        initSingleRowModel(row, model);
        setCacheItem(row, model);
        return model;
    }

    return nullptr;
}

/* Инициализация SSingleRowModel ранее загруженными данными из указанной строки
 * Метод вызывается при первом редактировании (например, изменение стоимости работы в карточке ремонта)
 * Возвращает 0 если в процессе возникла ошибка
*/
bool SSaleTableModel::initSingleRowModel(const int row, std::shared_ptr<SSingleRowJModel> model)
{
    model->setParent(this);
    auto mapper = dynamic_cast<MapperForSingleRowModel*>(model.get());
    if(!mapper)
        return 0;

    for(int i = 0; i < columnCount(); i++)
    {
        mapper->initSingleRowModelField(i, unformattedData(index(row, i))); // инициализация m_record
    }

    return 1;
}

/* Копирование данных из QSqlRecord в кэш SSingleRowModel
 * Метод используется при добавлении строк
 * Возвращает 0 если в процессе возникла ошибка
*/
bool SSaleTableModel::setSingleRowModelData(const QSqlRecord &record, std::shared_ptr<SSingleRowJModel> model)
{
    // кол-во столбцов источника должно соответствовать кол-ву столбцов модели таблицы, иначе маппинг будет работать некорректно
    Q_ASSERT_X(record.count() == columnCount(), "initSingleRowModel()", "different column count");
    bool ret = 1;
    model->setParent(this);

#if defined QT_DEBUG || defined S_TEST
    int qtyLimit;
    if(record.value(Columns::Avail).toInt() > 5)
        qtyLimit = 5;
    else
        qtyLimit = record.value(Columns::Avail).toInt() + 1;

    int rand = QRandomGenerator::global()->bounded(qtyLimit);
#endif

    auto mapper = dynamic_cast<MapperForSingleRowModel*>(model.get());
    if(!mapper)
        return 0;

    for(int i = 0; ret && (i < record.count()); i++)
    {
        QVariant value;

        if(i == Columns::ObjId)
            value = m_objId;
        else
            value = record.value(i);

        ret &= mapper->setSingleRowModelData(i, value);
    }

#if defined QT_DEBUG || defined S_TEST
    if(rand > 1)
    {
        int srm_index = mapper->singleRowModelIndex(Columns::Count);
        if(srm_index >= 0)
            model->setData(srm_index, rand);
    }
#endif

    return ret;
}

void SSaleTableModel::addToPendingRemoveList(std::shared_ptr<SSingleRowJModel> model)
{
    m_recordsPendingRemoveMap.insert(model.get(), model);
}

int SSaleTableModel::itemsAffected()
{
    return m_itemsAffected;
}

/*  Определение номера видимого столбца.
 *  Возвращает номер столбца, начиная с 1, или 0 если запрошенный столбец скрыт.
*/
int SSaleTableModel::visibleColumnIndex(const int column)
{
    int visibleColumn = 0;

    if((m_hiddenColumns >> column)&0x01)
        return visibleColumn;

    for(int i = 0; i < columnCount() && i <= column; i++)
    {
        visibleColumn += !((m_hiddenColumns >> i)&0x01);
    }

    return visibleColumn;
}

/* Генерирует набор флагов для отрисовки иконок в первом столбце таблицы
 * Это базовый метод, учитывающий только вариант таблицы, её состояние и тип строки
*/
int SSaleTableModel::pixmapFlags(const QModelIndex &index) const
{
    int row_state = saleMode() << 16 |\
                state() << 8 |\
                recordType(index.row());

    return row_state;
}

int SSaleTableModel::getParentWorkRow(const int itemRow)
{
    int i;
    for(i = itemRow; i >= 0; i--)
    {
        if(recordType(i) == RecordType::Work)
            break;
    }
    return i;
}

void SSaleTableModel::setRecordType(const int row, const int type)
{
    QStandardItemModel::setData(index(row, Columns::RecordType), type);
}

/* Возвращает id работы
 * Для обычного ремонта id = 0
 * Для заправок картриджей id >= 1
*/
int SSaleTableModel::workType(const int row) const
{
    return index(row, Columns::WorkType).data().toInt();
}

/*  Возвращает кол-во строк в таблице, не помеченных на удаление
 *  (данный метод нужен для работы в режиме OnManualSubmit)
*/
int SSaleTableModel::activeRowCount() const
{
    int rows = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(isRowMarkedRemove(i))
            continue;
        rows++;
    }
    return rows;
}

void SSaleTableModel::indexSelected(const QModelIndex &index)
{
    m_currentIndex = index.row();
}

/* т. к. некоторые данные обрабатываются по логическому номеру столбца, важно, чтобы они не изменились
 * В этом методе будет производиться сравнение со значениями из списка по умолчанию
 * TODO: вообще, нужны более универсальные способы получения данных конкретного столбца, но это потом...
 */
void SSaleTableModel::setHorizontalHeaderLabels()
{
    QStringList m_fieldsDep;
    QMetaEnum e = SSaleTableModel::staticMetaObject.enumerator(SSaleTableModel::staticMetaObject.indexOfEnumerator("Columns"));
    for(int i = 0; i < e.keyCount(); i++)
    {
        m_fieldsDep.append(e.key(i));
    }
    SStandardItemModel::setHorizontalHeaderLabels(m_fieldsDep);
}

void SSaleTableModel::rowAmountUpdate(const QModelIndex &index)
{
    QModelIndex summIdx = index;
    if(index.column() != Columns::Amount)
        summIdx = index.siblingAtColumn(Columns::Amount);

    emit dataChanged(summIdx, summIdx, QVector<int>({Qt::DisplayRole}));
}

/* Сумма всех товаров
 */
void SSaleTableModel::calculateAmounts()
{
    double amount;
    m_amountTotal = 0;
    m_amountItems = 0;
    m_amountWorks = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(isRowMarkedRemove(i))
            continue;

        amount = amountItem(i);
        // TODO: в идеале нужно реализовать это с использованием фабрики
        if(recordType(i) == RecordType::Item)
            m_amountItems += amount;
        else
            m_amountWorks += amount;

        m_amountTotal += amount;
    }
    emit amountChanged(m_amountTotal, m_amountItems, m_amountWorks);
}

double SSaleTableModel::amountTotal()
{
    return m_amountTotal;
}

QString SSaleTableModel::amountTotalLocale()
{
    return STableModelsCommonMethods::dataLocalizedFromDouble(m_amountTotal);
}

double SSaleTableModel::amountItems()
{
    return m_amountItems;
}

QString SSaleTableModel::amountItemsLocale()
{
    return STableModelsCommonMethods::dataLocalizedFromDouble(m_amountItems);
}

QString SSaleTableModel::amountWorksLocale()
{
    return STableModelsCommonMethods::dataLocalizedFromDouble(m_amountWorks);
}

void SSaleTableModel::setState(int state)
{
    if(m_modelState == state)
        return;

    m_modelState = state;
    endResetModel();    // для перерисовки кнопок в таблице
}

int SSaleTableModel::state() const
{
    return m_modelState;
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для сохранённых данных будет достаточно вызвать метод this->setQuery()
 */
void SSaleTableModel::sqlDataChanged()
{
    clear();
    setHorizontalHeaderLabels();
    setRowCount(m_queryData->rowCount());
    this->blockSignals(true);
    for(int i = 0; i < m_queryData->rowCount(); i++)
    {
        for(int j = 0; j < m_queryData->columnCount(); j++)
        {
            QStandardItemModel::setData(index(i, j), m_queryData->index(i, j).data());
        }
    }
    this->blockSignals(false);
    m_queryData->blockSignals(true);
    m_queryData->clear();
    m_queryData->blockSignals(false);
    calculateAmounts();

    endResetModel();
}

void SSaleTableModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_queryData->setQuery(query, db);
}

/* Метод получения данных для отчетов LimeReport
 * Смотри описание метода с таким же названием в классе SSingleRowJModel
 */
void SSaleTableModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackData() | info.dataType = " << info.dataType << "; info.index = " << info.index << "; info.columnName = " << info.columnName;
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = rowCount()?0:1; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = metaObject()->property(info.index + 1).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = metaObject()->property( metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(this); break;
        case LimeReport::CallbackInfo::ColumnCount: data = metaObject()->propertyCount() - 1; break;
        case LimeReport::CallbackInfo::RowCount: data = rowCount(); break;
    }
}

/* "Навигация" по модели данных
*/
void SSaleTableModel::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackDataChangePos() | type = " << type;
    if(type == LimeReport::CallbackInfo::First)
        m_reportRowNum = 0;
    else
    {
        if(m_reportRowNum+1 >= rowCount())
        {
            result = 0;
            return;
        }
        m_reportRowNum++;
    }
    result = 1;
}

QString SSaleTableModel::reportUID()
{
    return index(m_reportRowNum, Columns::UID).data().toString();
}

QString SSaleTableModel::reportId()
{
    return index(m_reportRowNum, Columns::Id).data().toString();
}

QString SSaleTableModel::reportItem()
{
    return index(m_reportRowNum, Columns::Name).data().toString();
}

QString SSaleTableModel::reportQty()
{
    return index(m_reportRowNum, Columns::Count).data().toString();
}

QString SSaleTableModel::reportPrice()
{
    return index(m_reportRowNum, Columns::Price).data().toString();
}

QString SSaleTableModel::reportSumm()
{
    return index(m_reportRowNum, Columns::Amount).data().toString();
}

QString SSaleTableModel::reportSN()
{
    return index(m_reportRowNum, Columns::SN).data().toString();
}

QString SSaleTableModel::reportWarranty()
{
    return index(m_reportRowNum, Columns::Warranty).data().toString();
}

QString SSaleTableModel::reportPerformer()
{
    return index(m_reportRowNum, Columns::User).data().toString();
}

QSqlDatabase SSaleTableModel::loadConnection() const
{
    if(!m_connection)
        return QSqlDatabase::database("connMain");
    return *(m_connection.get());
}

QSqlDatabase SSaleTableModel::commitConnection() const
{
    if(!m_connection)
        return QSqlDatabase::database("connThird");
    return *(m_connection.get());
}

void SSaleTableModel::setConnection(const QSqlDatabase &conn)
{
    m_connection = std::make_shared<QSqlDatabase>(conn);
}

bool SSaleTableModel::isWarranty()
{
    return m_isWarranty;
}

void SSaleTableModel::setIsWarranty(const bool isWarranty)
{
    m_isWarranty = isWarranty;
}

void SSaleTableModel::clear()
{
    m_recordsPendingRemoveMap.clear();
    SStandardItemModel::clear();
}

#if defined QT_DEBUG || defined S_TEST
int SSaleTableModel::dbgAddRandomItems(const int qty)
{
    auto query = std::make_unique<QSqlQuery>(loadConnection());
    int added = 0;

    for(int j = 0; j < (qty + 10); j++)
    {
        query->exec(QString("SELECT `id` FROM (SELECT ROUND(@i * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM store_items AS t1 CROSS JOIN (SELECT @i := 0) AS dummy WHERE t1.`count` - t1.`reserved` > 0 AND t1.`is_realization` = 1) AS t1 ON t1.`num` = `rand`.`rand`;"));
        if(!query->first())
            continue;

        if(!query->value(0).isValid() || query->value(0).isNull())
            continue;   // после обновления сервера на mysql 5.6.51 (win) пришлось чуть-чуть изменить запрос для случайного товара; также в только что открытой сессии результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их

        if(addItemByID(query->record().value(0).toInt(), m_priceColumnId))
            added++;

        if(added == qty)
            break;
    }

    return added;
}
#endif

/**************************************************************************************/
StoreSaleModel::StoreSaleModel(QObject *parent) :
    SSaleTableModel(parent)
{
    m_hiddenColumns = 1 << Columns::User | 1 << Columns::Realization | 1 << Columns::RetPercent | 1 << Columns::State | 1 << Columns::Notes | 1 << Columns::ItemId | 1 << Columns::InPrice | 1 << Columns::ObjId | 1 << Columns::Dealer | 1 << Columns::Buyer | 1 << Columns::RecordType | 1 << Columns::Created | 1 << Columns::WorkId;
}

/* Загрузка таблицы товаров ранее сохранённого документа (продажа)
 * Возвращает 1 в случае ошибки
*/
bool StoreSaleModel::loadTable(const int doc_id)
{
    setDocumentId(doc_id);
    if(m_modelState == State::Sold)
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC(m_objId), loadConnection());
    else if( m_modelState == State::Reserved || m_modelState == State::Cancelled )
        m_queryData->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(m_objId), loadConnection());
    else
        return 1;

    return 0;
}

bool StoreSaleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index))
        return 0;

    if (role == Qt::EditRole)
    {
        // если ячейки "Кол-во" и "Доступно" ранее были подсвечены ошибочными, то после редактирования сбрасываем фоновый цвет
        if( index.column() == Columns::Count)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                SStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
                SStandardItemModel::setData(index.siblingAtColumn(Columns::Avail), QVariant(), Qt::BackgroundRole);
            }
        }

        // то же для ячекйки "Цена"
        if( index.column() == Columns::Price)
        {
            if(index.data(Qt::BackgroundRole) == QColor(255,209,209) )
            {
                SStandardItemModel::setData(index, QVariant(), Qt::BackgroundRole);
            }
        }
    }

    return SSaleTableModel::setData(index, value, role);
}

void StoreSaleModel::removeRow(const int row)
{
    if(m_modelState == State::New)   // в режиме создания новой РН просто удаляем строки из модели
        removeRows(row, 1);
    else
        markRowRemove(row);

    endRemoveRow();
}

/* Удаление или пометка на удаление строки в режиме продажи: частичный возврат или частичная отмена
 * ранее зарезервированного товара.
 * При первом нажатии кнопки Удалить, в массив добавятся данные, а при повторном - удалятся (на случай,
 * если пользователь промахнулся). При подтверждении возврата или проведении РН резерва, записи с
 * соответствующими id будут обновлены, будет установлен флаг is_cancellation (state).
 * row  - номер строки
 * db_id - id записи в таблице store_sales
 */
void StoreSaleModel::markRowRemove(const int row)
{
    SSingleRowJModel *key_ptr = cacheItem(row).get();   // сырой указатель используется в качестве ключа
    if(m_recordsPendingRemoveMap.contains(key_ptr))
    {
        setData(index(row, Columns::State), SStoreSaleItemModel::State::Active);
        m_recordsPendingRemoveMap.remove(key_ptr);
    }
    else
    {
        // кол-во не обнуляем, т. к. пользователь мог промахнуться по строке; оно обнулится непосредственно перед коммитом
        setData(index(row, Columns::State), SStoreSaleItemModel::State::Cancelled);
        addToPendingRemoveList(cacheItem(row));
    }

    endResetModel();
    calculateAmounts();
}

/*  Помещает все строки таблицы в список на удаление
 */
void StoreSaleModel::markAllItemsToRemove(OpType type)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(type == OpType::Unsale)
            if(index(i, Columns::State).data().toBool())  // возвращённые ранее пропускаем; касается только простых продаж
                continue;

        markRowRemove(i);
    }
}

/*  Продажа (в т. ч. ранее зарезервированных) и резерв товаров.
 *  Вызов метода происходит в результате нажатия кнопки пользователем (т. е. сохранение не происходит автоматически)
 *  Допускается, что покупатель откажется от части зарезервированных товаров (помеченные пользователем строки)
 *  Возвращает 0 в случае ошибки
 */
bool StoreSaleModel::commit(int operation)
{
    bool ret = 1;

    if(!m_recordsPendingRemoveMap.isEmpty())
        ret = backOutItems(OpType::FreeReserved);

    m_itemsAffected = 0;
    for(int i = 0; i < rowCount() && ret; i++)
    {
        auto rec = singleRowModel(i);
        if(!rec || (data(index(i, Columns::State)).toInt() == SStoreSaleItemModel::State::Cancelled))
            continue;

        auto item = dynamic_cast<SStoreSaleItemModel*>(rec.get());
        if(!item)
            continue;

        m_itemsAffected++;
        setData(index(i, Columns::ObjId), m_objId);
        setData(index(i, Columns::Buyer), m_client);

        // TODO: в идеале нужно реализовать это с использованием фабрики
        if(operation == OpType::Sale)
            ret = item->sale();
        else
            ret = item->reserve();
    }

    if(!ret)
        throw Global::ThrowType::QueryError;

    // Флаги Changed в QStandardItem должны очищаться в методе endCommit

    return ret;
}

void StoreSaleModel::prepareUnsaleItems(const QString &reason)
{
    if(!reason.isEmpty())
        setExtraUnsaleReason(reason);

    if(m_recordsPendingRemoveMap.isEmpty())
    {
        markAllItemsToRemove(OpType::Unsale);
    }
}

/* Возврат товара
 * Допускается полный возврат товара (если пользователь не пометил отдельные cтроки)
 * или частичный. Также допускается многократный частичный возврат.
 * reason — причина возврата, указанная пользователем; будет записана в журнал
 */
bool StoreSaleModel::unsaleItems(const QString &reason)
{
    prepareUnsaleItems(reason);
    return backOutItems(OpType::Unsale);
}

void StoreSaleModel::setExtraUnsaleReason(const QString &reason)
{
    m_extraUnsaleReason = reason;
}

void StoreSaleModel::prepareFreeItems()
{
    m_recordsPendingRemoveMap.clear();
    markAllItemsToRemove(OpType::FreeReserved);
}

/* Полное снятие резерва
 * Очередь на удаление очищается и заполняется снова всеми строками
 */
bool StoreSaleModel::freeItems()
{
    prepareFreeItems();
    return backOutItems(OpType::FreeReserved);
}

/*  В данном методе производится непосредственно возврат/снятие резерва
 */
bool StoreSaleModel::backOutItems(OpType type)
{
    bool ret = 1;
    m_itemsAffected = 0;

    QMap<SSingleRowJModel*, std::shared_ptr<SSingleRowJModel>>::const_iterator i;
    for (i = m_recordsPendingRemoveMap.constBegin(); i != m_recordsPendingRemoveMap.constEnd() && ret; ++i)
    {
        auto item = dynamic_cast<SStoreSaleItemModel*>(i->get());
        if(!item)
            continue;

        m_itemsAffected++;
        // TODO: в идеале нужно реализовать это с использованием фабрики
        if(type == OpType::Unsale)
        {
            item->set_unsaleReason(m_extraUnsaleReason);
            ret = item->unsale();
        }
        else
            ret = item->free();

    }

    if(ret)
        m_recordsPendingRemoveMap.clear();
    else
        m_itemsAffected = 0;

    if(!ret)
        throw Global::ThrowType::QueryError;

    return ret;
}

Qt::ItemFlags StoreSaleModel::flags(const QModelIndex &index) const
{
    int condition;
    if(isModelEditable())   // у вкладки будет дополнительный режим — правка резерва (в АСЦ такого вроде не было)
    {
        condition = recordType(index.row()) << 7 | index.column();
        switch (condition)
        {
            case SSaleTableModel::RecordType::Item << 7 | Columns::Count:
            case SSaleTableModel::RecordType::Item << 7 | Columns::Price:
            case SSaleTableModel::RecordType::Item << 7 | Columns::SN:
            case SSaleTableModel::RecordType::Item << 7 | Columns::Warranty:
            case SSaleTableModel::RecordType::Item << 7 | Columns::User:
                return Qt::ItemIsEnabled | Qt::ItemIsEditable;
            default:
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }
    }
    else
    {
        return SSaleTableModel::flags(index);
    }
}

/* Генерирует набор флагов для отрисовки иконок в первом столбце таблицы
 * Переопределённый метод, добавляющий флаг состояния строки.
 * При простой продаже, в таблице отображаются строки отменённых продаж и резервов;
 * при пометке на возврат/отмену, кнопка ещё должна отображаться (вдруг пользователь промахнулся),
 * а после коммита уже не должна.
*/
int StoreSaleModel::pixmapFlags(const QModelIndex &index) const
{
    int bs = SSaleTableModel::pixmapFlags(index);

    auto srm = cacheItem(index.row());
    if(!srm.get() || !srm->isDirty())
        return bs | (index.siblingAtColumn(Columns::State).data().toInt() << 1);

    auto mapper = dynamic_cast<MapperForSingleRowModel*>(srm.get());
    if(!mapper)
        return bs;

    int srm_index = mapper->singleRowModelIndex(Columns::State);
    return bs | (srm->commitedData(srm_index).value_or(0).toInt() << 1);
}

bool StoreSaleModel::isModelEditable() const
{
    return (m_modelState == State::New) || (m_modelState == State::Reserved);
}

bool StoreSaleModel::checkBeforeItemAdd(const int id, const int)
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(itemId(i) == id)
        {
            qDebug() << QString("товар UID %1 уже добавлен").arg(id);
            QString msgCaption = tr("Повтор");
            QString msgText = tr("\"%1\" (UID %2) уже добавлен")
                        .arg(name(i))
                        .arg(id);

            flashPopupInfoMessage(msgCaption, msgText);
            return 0;
        }
    }

    return 1;
}

bool StoreSaleModel::isRowMarkedRemove(const int row) const
{
    if(state(row) == SStoreSaleItemModel::Cancelled)
        return 1;

    return 0;
}

int StoreSaleModel::itemInsertionRow()
{
    return rowCount();
}

#if defined QT_DEBUG || defined S_TEST
void StoreSaleModel::dbgAddRandomItem()
{
    // при простой продаже добавление от 1 до 3 случайных товаров
    int i = qMax(1, QRandomGenerator::global()->bounded(3));

    dbgAddRandomItems(i);
}
#endif

/************************************************************************************/
WorkshopSaleModel::WorkshopSaleModel(QObject *parent) :
    SSaleTableModel(parent)
{
    m_hiddenColumns = 1 << Columns::Avail | 1 << Columns::Realization | 1 << Columns::RetPercent | 1 << Columns::State | 1 << Columns::Notes | 1 << Columns::ItemId | 1 << Columns::InPrice | 1 << Columns::ObjId | 1 << Columns::Dealer | 1 << Columns::Buyer | 1 << Columns::RecordType | 1 << Columns::Created | 1 << Columns::WorkId;
}

WorkshopSaleModel::~WorkshopSaleModel()
{
//    if(m_cartridgeCardModel)
//        delete m_cartridgeCardModel;
}

void WorkshopSaleModel::initDemo()
{
    setEditStrategy(SSaleTableModel::OnManualSubmit);
    SSaleTableModel::initDemo();
}

/* Загрузка таблицы товаров ремонта
 * Возвращает 1 в случае ошибки
*/
bool WorkshopSaleModel::loadTable(const int repair_id)
{
    setRepairId(repair_id);
    m_queryData->setQuery(QUERY_SEL_REPAIR_WORKS_AND_PARTS(m_objId), loadConnection());

    return 0;
}

bool WorkshopSaleModel::checkBeforeItemAdd(const int, const int)
{
    if(m_currentIndex == -1 && rowCount())  // если не выбрана строка и таблица не пуста
    {
        QString msgCaption = tr("Информация");
        QString msgText = tr("Не выбрана работа. Укажите работу в которой была использована деталь");

        flashPopupInfoMessage(msgCaption, msgText);
        return 0;
    }

    return 1;
}

/* Инициализация SSingleRowModel данными из таблицы store_int_reserve, загруженными ранее.
 * Метод используется при добавлении товаров из корзины.
 * Возвращает 0 если в процессе возникла ошибка
*/
bool WorkshopSaleModel::initSingleRowModel(const QSqlRecord &record, std::shared_ptr<SSingleRowJModel> model)
{
    bool ret = 1;

    model->setParent(this);
    auto mapper = dynamic_cast<MapperForSingleRowModel*>(model.get());
    if(!mapper)
        return 0;

    for(int i = 0; ret && (i < record.count()); i++)
    {
        QVariant value = record.value(i);
        mapper->setExtraData(i, value);
        mapper->initSingleRowModelField(i, value);
    }

    return ret;
}

/* Передача данных в отдельные поля модели
 * Метод должен вызываться после добавления объекта модели в кэш
*/
void WorkshopSaleModel:: itemAddHook(const int row)
{
    auto model = singleRowModel(row);
    model->setData(SRepairSaleItemModel::C_repairId, m_objId);
    model->setData(SRepairSaleItemModel::C_state, SRepairSaleItemModel::RepairLinked);
}

/* Добавление товара из корзины сотрудника
 * Если qty не передаётся или равен нулю, добавляется всё зарезервированное кол-во
*/
bool WorkshopSaleModel::addItemFromBasket(const int id, const int qty)
{
    if(!isModelEditable())
        return 0;

    if(!checkBeforeItemAdd(id, qty))
        return 0;

    bool ret = 1;
    const QString count = qty?QString::number(qty):"`count`";
    auto query = std::make_unique<QSqlQuery>(loadConnection());
    query->exec(QUERY_SEL_PART_FROM_BASKET(id, count));
    if(!query->first())
        return false;

    int row = itemInsertionRow();
    insertRows(row, 1);
    int type = RecordType::Item;
    setRecordType(row, type);
    auto model = RecordFactory::instance().create(saleMode() | type);
    if(!model)
        return 0;

    setCacheItem(row, model);
    if(!initSingleRowModel(query->record(), model))
        return 0;

    model->setData(SRepairSaleItemModel::C_toUser, userDbData->id());
    itemAddHook(row);

    if(!endModelChange())
        SStandardItemModel::removeRow(row);

    calculateAmounts();

    if(qty) // добавление части товара
    {
        qDebug() << "[" << this << "] addItemFromBasket() | TODO: создать копию записи в табл. store_int_reserve с разницей кол-ва";
//        m_itemsPendingSplitList
//        setData(index(insertionRow, Columns::Count), qty);   // обновить кол-во товара в старой записи
    }

    return ret;
}

void WorkshopSaleModel::removeRow(const int row)
{
    int editStrategyBackup = m_editStrategy;
    m_editStrategy = OnManualSubmit;
    markRowRemove(row);
    m_editStrategy = editStrategyBackup;
    endModelChange();

    endRemoveRow();
}

/* Пометка строки на удаление
 * row  - номер строки
 * db_id - id записей в таблицах store_int_reserve и works
*/
void WorkshopSaleModel::markRowRemove(const int row)
{
    auto rmUncommited = [&](const int r){
        if(m_editStrategy == OnManualSubmit && !data(index(r, Columns::Id)).toInt())
        {
            // если текущая запись еще не закоммичена, то просто удаляем её
            SSaleTableModel::removeRows(r, 1);
            m_currentIndex = r - 1;
            return 1;
        }
        return 0;
    };

    if(recordType(row) == RecordType::Work)   // сначала обрабатываем записи о товарах привязанных к удаляемой работе
    {
        int i = row + 1;
        while(i < rowCount() && recordType(i) == RecordType::Item)
        {
            if(rmUncommited(i))
                continue;

            markRowRemove(i);
            i++;
        }
    }

    if(rmUncommited(row))
        return;

    SSingleRowJModel *key_ptr = cacheItem(row).get();   // сырой указатель используется в качестве ключа
    if(m_recordsPendingRemoveMap.contains(key_ptr))
    {   // в режиме ручного сохранения списка работ и деталей можно отменить пометку на удаление
        m_recordsPendingRemoveMap.remove(key_ptr);
        setData(index(row, Columns::State), SRepairSaleItemModel::RepairLinked);
        m_currentIndex = row;
    }
    else
    {
        int newState;
        if(m_repairType == Type::Regular)
            newState = SRepairSaleItemModel::EngineerBasket;
        else
            newState = SRepairSaleItemModel::Archive;

        setData(index(row, Columns::State), newState);
        addToPendingRemoveList(cacheItem(row));
        m_currentIndex = row - 1;
    }
}

bool WorkshopSaleModel::isDirty()
{
    if(!m_recordsPendingRemoveMap.isEmpty())
        return 1;

    return SStandardItemModel::isDirty();
}

/*  Метод сохранения таблицы для случаев выдачи ремонта или возврата ранее выданного ремонта
 *  (обрабатываются только товары)
 */
bool WorkshopSaleModel::commit(int operation)
{
    bool ret = 1;

    ret = commit();   // если по каким-либо причинам таблица не сохранена
    for(int i = 0; i < rowCount() && ret; i++)
    {
        if(recordType(i) == RecordType::Work)
            continue;

        auto rec = singleRowModel(i);
        if(!rec)
            ret = 0;

        auto item = dynamic_cast<SRepairSaleItemModel*>(rec.get());
        Q_ASSERT_X(item, "WorkshopSaleModel::commit(int)", "Can't cast SSingleRowJModel to SRepairSaleItemModel");

        switch(operation)
        {
            case OpType::Sale: ret = item->sale(); break;
            case OpType::Unsale: ret = item->unsale(); break;
            default:;
        }
    }

    if(!ret)
        throw Global::ThrowType::QueryError;

    return ret;
}

/*  Сохранение таблицы работ и деталей в ремонте.
 *  Вызов метода может происходить, в зависимости от настроек пользователя, при:
 *   - завершении редактирования таблицы (т. е. автоматически после любого изменения; по умолчанию)
 *   - TODO: вручную.
 *  Возвращает 0 в случае ошибки
 */
bool WorkshopSaleModel::commit()
{
    bool ret = 1;
    int lastHandledWorkId = 0;
    int editStrategyBackup = m_editStrategy;

    ret = removeRows();

    // Перед вызовом методов записи данных в БД нужно дополнительно задать значения некоторых полей
    // и при этом избежать зацикливания, если включен режим сохранения после каждого изменения
    m_editStrategy = EditStrategy::Nop;

    for(int i = 0; i < rowCount() && ret; i++)
    {
        auto rec = cacheItem(i);

        int recType = recordType(i);

        if(!rec || !rec->isDirty())
        {
            if(recType == RecordType::Work)
                lastHandledWorkId = index(i, Columns::Id).data().toInt();
            continue;
        }

        SSaleTableModel::setData(index(i, Columns::ObjId), m_objId);

        if(recType == RecordType::Work)
        {
            auto work = dynamic_cast<SWorkModel*>(rec.get());
            if(m_repairType == Type::Cartridge)
            {
                work->set_salarySumm(m_cartridgeCardModel->material((SWorkModel::Type)index(i, Columns::WorkType).data().toInt())->salarySumm());
            }
            ret = work->commit();
            lastHandledWorkId = work->id();
        }
        else  // обработка записи товара
        {
            auto item = dynamic_cast<SRepairSaleItemModel*>(rec.get());
            SSaleTableModel::setData(index(i, Columns::WorkId), lastHandledWorkId);

            ret &= item->commit();
        }
    }

    m_editStrategy = editStrategyBackup;

    if(!ret)
        throw Global::ThrowType::QueryError;

    emit tableSaved();

    return ret;
}

/* Данный метод предназначен для сохранения данных таблицы автономно
 * (отличается от saveTables отправкой запросов начала и завершения транзакции).
 * ЭТО ИСПОЛЬЗУЕТСЯ В КАРТОЧКЕ РЕМОНТА!!! НУЖНО ПЕРЕНЕСТИ ТУДА И ЗАВЯЗАТЬ НА SWidget::manualSubmit()
*/
bool WorkshopSaleModel::commitStandalone()
{
    bool nErr = 1;

    QSqlQuery query(commitConnection());

#if defined QT_DEBUG || defined S_TEST
    SQueryLog *queryLog = new SQueryLog();
    queryLog->start(metaObject()->className());
#endif

    try
    {
        QUERY_EXEC_TH(&query,nErr,QUERY_BEGIN);

        commit();

#if defined QT_DEBUG || defined S_TEST
//        Global::throwDebug();
#endif

        QUERY_COMMIT_ROLLBACK(&query,nErr);
    }
    catch(Global::ThrowType type)
    {
        nErr = 0;

        if(type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(&query, nErr);
        }
    }
#if defined QT_DEBUG || defined S_TEST
    queryLog->stop();
    delete queryLog;
#endif

    if(nErr)
    {
        auto p = new shortlivedNotification(this,
                                            tr("Успешно"),
                                            tr("Список работ и деталей сохранён"),
                                            QColor(214,239,220),
                                            QColor(229,245,234));
        Q_UNUSED(p)
    }

    return nErr;
}

/*  В данном методе производится вызов метода commitStandalone()
 *  или эмиссия сигнала tableDataChanged(), в зависимости от режима сохранения
*/
bool WorkshopSaleModel::endModelChange()
{
    bool nErr = 1;

    if( m_editStrategy == OnFieldChange || m_editStrategy == OnRowChange )
    {
        nErr = commitStandalone();
    }
    else if(m_editStrategy == OnManualSubmit)
    {
        emit tableDataChanged();
    }

    return nErr;
}

Qt::ItemFlags WorkshopSaleModel::flags(const QModelIndex &index) const
{
    int condition;
    if(isModelEditable())
    {
        condition = recordType(index.row()) << 7 | index.column();
        switch (condition)
        {
            case SSaleTableModel::RecordType::Work << 7  | Columns::Name:
            case SSaleTableModel::RecordType::Work << 7  | Columns::Price:
                if((m_modelState != State::Adm) && !this->index(index.row(), Columns::UID).data().toString().isEmpty())
                    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
                Q_FALLTHROUGH();
            case SSaleTableModel::RecordType::Work << 7  | Columns::Count:
            case SSaleTableModel::RecordType::Work << 7  | Columns::Warranty:
            case SSaleTableModel::RecordType::Work << 7  | Columns::User:
            case SSaleTableModel::RecordType::Item << 7  | Columns::SN:
            case SSaleTableModel::RecordType::Item << 7  | Columns::Warranty:
                return Qt::ItemIsEnabled | Qt::ItemIsEditable;
            case SSaleTableModel::RecordType::Item << 7  | Columns::Price:
            case SSaleTableModel::RecordType::Item << 7  | Columns::User:
                if(m_modelState == State::Adm)
                    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
                Q_FALLTHROUGH();
            case SSaleTableModel::RecordType::Item << 7  | Columns::Count:
                if(m_modelState == State::Adm && permissions->addGoodsFromWarehouse)   // Устанавливать детали со склада
                    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
                Q_FALLTHROUGH();
            default:
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }
    }
    else
    {
        return SSaleTableModel::flags(index);
    }
}

bool WorkshopSaleModel::removeRows()
{
    if(m_recordsPendingRemoveMap.isEmpty())
        return 1;

    bool nErr = 1;

    // TODO: в идеале нужно реализовать работу с моделями с использованием фабрики
    QMap<SSingleRowJModel*, std::shared_ptr<SSingleRowJModel>>::const_iterator i;
    for (i = m_recordsPendingRemoveMap.constBegin(); i != m_recordsPendingRemoveMap.constEnd() && nErr; ++i)
    {
        auto item = dynamic_cast<SRepairSaleItemModel*>(i->get());
        if(!item)
            continue;

        nErr = item->unlinkRepair();
        if(m_repairType == Type::Cartridge)
            nErr = item->free(); // деталь/материал при работе с картриджами автоматически удаляется из корзины сотрудника.
    }
    for (i = m_recordsPendingRemoveMap.constBegin(); i != m_recordsPendingRemoveMap.constEnd() && nErr; ++i)
    {
        auto work = dynamic_cast<SWorkModel*>(i->get());
        if(!work)
            continue;

        work->remove();
    }

    for(int i = rowCount() - 1; i >= 0 && nErr; i--)
    {
        if(isRowMarkedRemove(i))
            nErr = SSaleTableModel::removeRows(i, 1);
    }

    if(nErr)
    {
        m_recordsPendingRemoveMap.clear();
        endResetModel();
    }

    return nErr;
}

bool WorkshopSaleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(SSaleTableModel::setData(index, value, role))
        return endModelChange();

    return 0;
}

int WorkshopSaleModel::editStrategy()
{
    return m_editStrategy;
}

void WorkshopSaleModel::setEditStrategy(const int strategy)
{
    m_editStrategy = strategy;
}

void WorkshopSaleModel::setRepairType(bool type)
{
    m_repairType = type;
}

void WorkshopSaleModel::setCartridgeCardModel(SCartridgeCardModel *cartridgeCardModel)
{
    m_cartridgeCardModel = cartridgeCardModel;
}

bool WorkshopSaleModel::isModelEditable() const
{
    return (m_modelState == State::RW) || (m_modelState == State::Adm);
}

bool WorkshopSaleModel::isRowMarkedRemove(const int row) const
{
    int state = this->state(row);
    if( (state == SRepairSaleItemModel::EngineerBasket) || \
        (state == SRepairSaleItemModel::Archive) )
        return 1;

    return 0;
}

/* Возвращает номер строки для добавления товара.
*/
int WorkshopSaleModel::itemInsertionRow()
{
    int rows = rowCount();

    // если таблица пуста, то добавляем произвольную работу
    if(!activeRowCount())
    {
        int editStrategyBackup = m_editStrategy;
        m_editStrategy = EditStrategy::Nop;
        addCustomWork();
        m_editStrategy = editStrategyBackup;
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

#if defined QT_DEBUG || defined S_TEST
void WorkshopSaleModel::dbgAddRandomItem()
{
    // в ремонт добавление одного случайного товара
    int i = 1;

    dbgAddRandomItems(i);
}

void WorkshopSaleModel::dbgAddRandomItemBasket()
{
    auto query = std::make_unique<QSqlQuery>(loadConnection());
    int id = 0;

    for(int j = 0; j < 4 && id == 0; j++)   // id может быть получен не с первого раза (какая-то особенность mysql, не разобрался).
    {
        query->exec(QString("SELECT `id` FROM (SELECT ROUND(@i * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM store_int_reserve AS t1 CROSS JOIN (SELECT @i := 0) AS dummy WHERE t1.`state` = 1) AS t1 ON t1.`num` = `rand`.`rand`;"));
        query->first();
        id = query->record().value(0).toInt();
        if(id)
            addItemFromBasket(id);
    }
}
#endif

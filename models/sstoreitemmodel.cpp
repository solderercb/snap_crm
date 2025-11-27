#include "sstoreitemmodel.h"
#include <QSqlQuery>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SStandardItemModel>
#include <SLogRecordModel>

SStoreItemModel::SStoreItemModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "store_items";
    i_obligatoryFields << "" << "articul" << "dealer" << "name" << "state" << "category" << "store" << "shop_description" << "description";
    setPrimaryKeyIndex(0);

    i_logRecord->set_type(SLogRecordModel::Part);
    initSqlQuery();
}

SStoreItemModel::~SStoreItemModel()
{
    delIntegrityCheckObjects();
}

SStandardItemModel *SStoreItemModel::priceOptionsList()
{
    SStandardItemModel *list;
    QMetaEnum types = SStoreItemModel::staticMetaObject.enumerator(SStoreItemModel::staticMetaObject.indexOfEnumerator("PriceOption"));
    QVector<QString> priceColDBFieldsList = {"`price`", "`price2`", "`price3`", "`price4`", "`price5`", "0"};
    QList<QStandardItem*> dbFields;

    list = SStandardItemModel::modelFromEnum(types, tr);
    for (int i=0; i<priceColDBFieldsList.size(); i++)
    {
        dbFields << new QStandardItem(priceColDBFieldsList.at(i));
    }
    list->appendColumn(dbFields);
    list->setObjectName("priceColModel");
    list->setHorizontalHeaderLabels({"name", "id", "dbColumn"});

    return list;
}

/* Ранее закоммиченое значение расхода (значение поля `store_int_reserve`.`count` или `store_sales`.`count`)
 * Оно используется для коррекции значений в таблице `store_items` при возврате товара, отмене резерва и т. п. операциях
 * По умолчанию оно равно нулю, что соответвтует новой продаже
*/
void SStoreItemModel::setCommitedExpense(int commitedExpense)
{
    m_commitedExpense = commitedExpense;
}

/* Значение расхода (т. е. кол-во продаваемого/списываемого товара)
 * Соответствует значению в таблице непосредственно перед коммитом
*/
void SStoreItemModel::setNewExpense(int newExpense)
{
    m_newExpense = newExpense;
}

void SStoreItemModel::setSaleEntityId(int id)
{
    m_saleObjId = id;
}

void SStoreItemModel::setSaleMode(SaleMode mode)
{
    m_saleMode = mode;
    if(m_saleMode == SaleMode::Store)
        i_logRecord->set_document(m_saleObjId);
}

/* Получение актуальных количеств из БД
 * Метод помечен как виртуальный для возможности подмены данных в тесте
*/
SStoreItemModel::itemQtys* SStoreItemModel::loadQtys()
{
    itemQtys *item = new itemQtys;
    bool nErr = 1;

    QUERY_EXEC(i_query, nErr)(QUERY_SEL_ITEM_ACTUAL_QTY(id()));
    if(nErr && i_query->first())
    {
        item->count = i_query->record().value(0).toInt();
        item->sold = i_query->record().value(1).toInt();
        item->reserved = i_query->record().value(2).toInt();
    }

    return item;
}

void SStoreItemModel::delIntegrityCheckObjects()
{
    if(m_itemQtysBefore)
    {
        delete m_itemQtysBefore;
        m_itemQtysBefore = nullptr;
    }
    if(m_itemQtysAfter)
    {
        delete m_itemQtysAfter;
        m_itemQtysAfter = nullptr;
    }
}

bool SStoreItemModel::setData(const int index, const QVariant &data)
{
    if(index < 0)
        return 1;

    return SSingleRowJModel::setData(index, data);
}

/*  Проверка доступности товара перед продажей
 *  возвращает 1 в случае недоступности запрошенного кол-ва
*/
bool SStoreItemModel::checkBeforeSale()
{
    m_itemQtysBefore = loadQtys();

    // При продаже ранее зарезервированного товара допускается изменение кол-ва (вдруг клиент решил купить меньше/больше);
    // в этом случае при проверке не будет учитываться старое кол-во
    if(m_newExpense > (m_itemQtysBefore->count - m_itemQtysBefore->reserved + m_commitedExpense) )
    {
        delIntegrityCheckObjects();
        return 1;
    }
    return 0;
}

bool SStoreItemModel::checkAfterSale()
{
    bool nIntegrity = 1;
    m_itemQtysAfter = loadQtys();

    try
    {
        switch(m_opOnItemType)
        {
            case Sale:
            case WriteOff:
            {
                if( m_itemQtysBefore->count - m_newExpense != m_itemQtysAfter->count )   // `count`
                    throw 1;
                if( m_itemQtysBefore->sold + m_newExpense != m_itemQtysAfter->sold )   // `sold`
                    throw 1;
                break;
            }
            case Reserve:
            {
                if( m_itemQtysBefore->reserved - m_commitedExpense + m_newExpense != m_itemQtysAfter->reserved )   // `reserved`
                    throw 1;
                break;
            }
            case SaleReserved:
            case SaleRepair:
            {
                if( m_itemQtysBefore->count - m_newExpense != m_itemQtysAfter->count )   // `count`
                    throw 1;
                if( m_itemQtysBefore->sold + m_newExpense != m_itemQtysAfter->sold )   // `sold`
                    throw 1;
                if( m_itemQtysBefore->reserved - m_commitedExpense != m_itemQtysAfter->reserved )   // `reserved`
                    throw 1;
                break;
            }
            case Unsale:
            case CancelWriteOff:
            {
                if( m_itemQtysBefore->count + m_commitedExpense != m_itemQtysAfter->count )   // `count`
                    throw 1;
                if( m_itemQtysBefore->sold - m_commitedExpense != m_itemQtysAfter->sold )   // `sold`
                    throw 1;
                break;
            }
            case Free:
            {
                if( m_itemQtysBefore->reserved - m_commitedExpense != m_itemQtysAfter->reserved )   // `reserved`
                    throw 1;
                break;
            }
            case UnsaleRepair:
            {
                if( m_itemQtysBefore->count + m_newExpense != m_itemQtysAfter->count )   // `count`
                    throw 1;
                if( m_itemQtysBefore->sold - m_newExpense != m_itemQtysAfter->sold )   // `sold`
                    throw 1;

                if( m_itemQtysBefore->reserved + m_commitedExpense != m_itemQtysAfter->reserved )   // `reserved`
                    throw 1;
                break;
            }
        }
    }
    catch(int)
    {
        nIntegrity = 0;
    }

    delIntegrityCheckObjects();
    return nIntegrity;
}

/*
 * Возвращает 0 в случае ошибки
*/
bool SStoreItemModel::sale(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Sale;
    set_count(m_itemQtysBefore->count - m_newExpense);
    set_sold(m_itemQtysBefore->sold + m_newExpense);
    appendLogText(logText, "0");

    if(!commit())
        return 0;

    m_commitedExpense = m_newExpense;
    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Document))
        return 0;

    return 1;
}

bool SStoreItemModel::reserve(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Reserve;
    set_reserved(m_itemQtysBefore->reserved - m_commitedExpense + m_newExpense);
    appendLogText(logText, "0");
    if(!commit())
        return 0;

    m_commitedExpense = m_newExpense;
    return 1;
}

bool SStoreItemModel::saleReserved(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::SaleReserved;
    set_count(m_itemQtysBefore->count - m_newExpense);
    set_sold(m_itemQtysBefore->sold + m_newExpense);
    set_reserved(m_itemQtysBefore->reserved - m_commitedExpense);
    appendLogText(logText, "0");

    if(!commit())
        return 0;

    m_commitedExpense = m_newExpense;
    return 1;
}

bool SStoreItemModel::saleStore(const QString &logText)
{
    if(!saleReserved(logText))
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Document))
        return 0;

    return 1;
}

bool SStoreItemModel::saleRepair(const QString &logText)
{
    if(!saleReserved(logText))
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Repair))
        return 0;

    return 1;
}

bool SStoreItemModel::free(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Free;
    set_reserved(m_itemQtysBefore->reserved - m_commitedExpense);
    appendLogText(logText, "0");
    if(!commit())
        return 0;

    m_commitedExpense = 0;
    return 1;
}

bool SStoreItemModel::unsale(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Unsale;
    set_count(m_itemQtysBefore->count + m_commitedExpense);
    set_sold(m_itemQtysBefore->sold - m_commitedExpense);
    appendLogText(logText, "0");
    if(!commit())
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Document))
        return 0;

    m_commitedExpense = 0; // после расчета dealerRoyalty

    return 1;
}

bool SStoreItemModel::unsaleRepair(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    // TODO: Возврат ремонта с возвратом запчастей и без возврата
    m_opOnItemType = OpOnItem::UnsaleRepair;
    set_count(m_itemQtysBefore->count + m_newExpense);
    set_sold(m_itemQtysBefore->sold - m_newExpense);
    set_reserved(m_itemQtysBefore->reserved + m_commitedExpense);
    appendLogText(logText, "0");
    if(!commit())
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Repair))
        return 0;

    return 1;
}

bool SStoreItemModel::dealerRoyalty(const SBalanceLogRecordModel::RoyaltyReason source)
{
    if(!isRealization())
        return 1;

    double royaltyForItem = inPrice() + (price() - inPrice())*returnPercent()/100;
    QString logText;

    if(royaltyForItem == 0)
        return 1;

    SClientModel dealer;
    dealer.setPrimaryKey(this->dealer());
    dealer.createBalanceObj();

    if(m_opOnItemType == Unsale || m_opOnItemType == UnsaleRepair)
    {
        double dealerRoyalty = m_commitedExpense*royaltyForItem;
        if(m_opOnItemType == Unsale)
            logText = tr("Списание %1 по причение возврата %2ед. товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_commitedExpense).arg(id());
        else
            logText = tr("Списание %1 за %2ед. товара %3, находившегося на реализации, по причение отмены выдачи ремонта").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_commitedExpense).arg(id());
        dealer.updateBalance(-dealerRoyalty, logText, source, m_saleObjId);
    }
    else
    {
        double dealerRoyalty = m_newExpense*royaltyForItem;
        dealer.updateBalance(dealerRoyalty, tr("Зачисление %1 за %2ед. проданного товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_newExpense).arg(id()), source, m_saleObjId);
    }

    return 1;
}

/* Метод помечен как виртуальный для проверки содержимого кэша в тесте
*/
bool SStoreItemModel::commit()
{
    Q_ASSERT_X(m_saleMode, "SStoreItemModel::commit()", "Не задан m_saleMode");
    if(isPrimaryKeyValid())
    {
        set_updated(QDateTime::currentDateTime());
    }

    SSingleRowJModel::commit();

    if(!checkAfterSale())
    {
        Global::throwError(Global::ThrowType::IntegrityError, tr("Ошибка целостности данных item_id = %1").arg(id()));
    }

    // TODO: если последняя единица товара, нужно удалить ячейку и сделать соотв. запись в журнал (но! нужно чтобы при вызове updateWidgets() в таблице место не обновлялось)
    // а может и не нужно; в АСЦ удалялась, но за годы пользования я так и не понял зачем это сделано
    return 1;
}

void SStoreItemModel::updateLogAssociatedRecId()
{
    if(isPrimaryKeyValid())
        i_logRecord->set_item(id());
}

void SStoreItemModel::translateNames()
{
    tr("PriceOptionService");
    tr("PriceOptionRetail");
    tr("PriceOptionWholesale");
    tr("PriceOptionWholesale2");
    tr("PriceOptionWholesale3");
    tr("PriceOptionWarranty");
}

// TODO: добавить разрешение для периодического переучета товаров, например, "Уведомлять о необходимости переучета товаров"
// Для этого завести таблицу очереди с товарами для переучета; товары заносить в очередь по истечении некоторого периода с
// момента последнего переучета.
// Генерировать уведомление пользователю, если в таблице есть не обработанные записи (т. е. должно быть только одно уведомление).
//QDate SStoreItemModel::lastStocktaking_date()
//{
//    return m_lastStocktaking_date;
//}

//void SStoreItemModel::setLastStocktakingDate(const QDate last_stocktaking_date)
//{
//    i_valuesMap.insert("last_stocktaking_date", last_stocktaking_date);
//}

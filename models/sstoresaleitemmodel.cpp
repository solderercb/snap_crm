#include "sstoresaleitemmodel.h"

SStoreSaleItemModel::SStoreSaleItemModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "store_sales";
    i_obligatoryFields << "dealer" << "item_id" << "document_id" << "user";
}

SStoreSaleItemModel::SStoreSaleItemModel(const QList<QStandardItem *> &record, QObject *parent) :
    SStoreSaleItemModel(parent)
{
    i_id = record.at(SStoreItemModel::SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();

    m_storeItem = new SStoreItemModel(record, count());
    m_storeItem->setSaleMode(SStoreItemModel::SaleMode::Store);

    m_itemId = record.at(SStoreItemModel::SaleOpColumns::ColItemId)->data(Qt::DisplayRole).toInt();
    m_itemName = record.at(SStoreItemModel::SaleOpColumns::ColName)->data(Qt::DisplayRole).toString();
    setDocumentId(record.at(SStoreItemModel::SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt());
    m_dealer = record.at(SStoreItemModel::SaleOpColumns::ColDealer)->data(Qt::DisplayRole).toInt();
    m_customer = record.at(SStoreItemModel::SaleOpColumns::ColBuyer)->data(Qt::DisplayRole).toInt();
    m_isRealization = record.at(SStoreItemModel::SaleOpColumns::ColRealization)->data(Qt::DisplayRole).toBool();
    m_returnPercent = record.at(SStoreItemModel::SaleOpColumns::ColRetPercent)->data(Qt::DisplayRole).toInt();
    m_inPrice = record.at(SStoreItemModel::SaleOpColumns::ColInPrice)->data(Qt::DisplayRole).toFloat();
    m_price = record.at(SStoreItemModel::SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toFloat();
    m_count = record.at(SStoreItemModel::SaleOpColumns::ColCount)->data(Qt::DisplayRole).toInt();
    m_avail = record.at(SStoreItemModel::SaleOpColumns::ColAvail)->data(Qt::DisplayRole).toInt();

    // TODO: сделать выборочную передачу значений: для не новой РН нужно передавать только изменённые данные
//    if(!i_id)
//    {
        i_valuesMap.insert("user", userDbData->value("id"));
        i_valuesMap.insert("item_id", m_itemId);
        i_valuesMap.insert("count", m_count);
        i_valuesMap.insert("in_price", m_inPrice);
        i_valuesMap.insert("price", m_price);
        i_valuesMap.insert("dealer", m_dealer);
        i_valuesMap.insert("warranty", record.at(SStoreItemModel::SaleOpColumns::ColWarranty)->data(Qt::DisplayRole).toInt());
        i_valuesMap.insert("is_realization", m_isRealization);
        i_valuesMap.insert("is_cancellation", record.at(SStoreItemModel::SaleOpColumns::ColState)->data(Qt::DisplayRole).toBool());
//        i_valuesMap.insert("dealer_payment", record.at()->data(Qt::DisplayRole).toBool());
        i_valuesMap.insert("return_percent", m_returnPercent);
        i_valuesMap.insert("sn", record.at(SStoreItemModel::SaleOpColumns::ColSN)->data(Qt::DisplayRole).toString());
//    }
//    else
//    {
//        i_valuesMap.insert("count", m_count);
//    }
}

SStoreSaleItemModel::~SStoreSaleItemModel()
{
    if(m_storeItem)
    {
        delete m_storeItem;
        m_storeItem = nullptr;
    }
}

int SStoreSaleItemModel::id()
{
    return i_id;
}

int SStoreSaleItemModel::dealer()
{
    return m_dealer;
}

void SStoreSaleItemModel::setDealer(const int dealer)
{
    i_valuesMap.insert("dealer", dealer);
}

int SStoreSaleItemModel::itemId()
{
    return m_itemId;
}

void SStoreSaleItemModel::setItemId(const int item_id)
{
    i_valuesMap.insert("item_id", item_id);
}

int SStoreSaleItemModel::documentId()
{
    return m_docId;
}

void SStoreSaleItemModel::setDocumentId(const int id)
{
    m_docId = id;
    i_valuesMap.insert("document_id", id);
}

/*  Возвращает значение `store_sales`.`count` из БД или 0 */
int SStoreSaleItemModel::count()
{
    if( i_id == 0 )
        return 0;

    QUERY_EXEC(i_query,i_nErr)( QUERY_SEL_STORE_SALES_QTY(i_id));
    i_query->first();
    m_savedCount = i_query->value(0).toInt();
    return m_savedCount;
}

void SStoreSaleItemModel::setCount(const int count)
{
    i_valuesMap.insert("count", count);
}

float SStoreSaleItemModel::inPrice()
{
    return m_inPrice;
}

void SStoreSaleItemModel::setInPrice(const float in_price)
{
    i_valuesMap.insert("in_price", in_price);
}

float SStoreSaleItemModel::price()
{
    return m_price;
}

void SStoreSaleItemModel::setPrice(const float price)
{
    i_valuesMap.insert("price", price);
}

int SStoreSaleItemModel::warranty()
{
    return m_warranty;
}

void SStoreSaleItemModel::setWarranty(const int warranty)
{
    i_valuesMap.insert("warranty", warranty);
}

bool SStoreSaleItemModel::realizatorPayed()
{
    return m_realizatorPayed;
}

void SStoreSaleItemModel::setRealizatorPayed(const bool realizator_payed)
{
    i_valuesMap.insert("realizator_payed", realizator_payed);
}

bool SStoreSaleItemModel::isRealization()
{
    return m_isRealization;
}

void SStoreSaleItemModel::setIsRealization(const bool is_realization)
{
    i_valuesMap.insert("is_realization", is_realization);
}

int SStoreSaleItemModel::dealerPayment()
{
    return m_dealerPayment;
}

void SStoreSaleItemModel::setDealerPayment(const int dealer_payment)
{
    i_valuesMap.insert("dealer_payment", dealer_payment);
}

int SStoreSaleItemModel::returnPercent()
{
    return m_returnPercent;
}

void SStoreSaleItemModel::setReturnPercent(const int return_percent)
{
    i_valuesMap.insert("return_percent", return_percent);
}

int SStoreSaleItemModel::user()
{
    return m_user;
}

void SStoreSaleItemModel::setUser(const int user)
{
    i_valuesMap.insert("user", user);
}

QString SStoreSaleItemModel::unsaleReason()
{
    return m_cancellationReason;
}

/*  Метод предназначен для передачи причины возврата, указанной польователем и записи её в журнал
*/
void SStoreSaleItemModel::setExtraUnsaleReason(const QString& reason)
{
    m_unsaleReason = reason;
    m_storeItem->setUnsaleReason(m_unsaleReason);
}

QString SStoreSaleItemModel::sn()
{
    return m_sn;
}

void SStoreSaleItemModel::setSN(const QString& sn)
{
    i_valuesMap.insert("sn", sn);
}

int SStoreSaleItemModel::dCategory()
{
    return m_dCategory;
}

void SStoreSaleItemModel::setDCategory(const int d_category)
{
    i_valuesMap.insert("d_category", d_category);
}

void SStoreSaleItemModel::setBuyer(int id)
{
    if(id == 0)
    {
        unsetBuyer();
        return;
    }

    i_valuesMap.insert("customer_id", id);
}

void SStoreSaleItemModel::unsetBuyer()
{
    i_valuesMap.insert("customer_id", QVariant());
}

/*
 * Возвращает 0 в случае ошибки
 */
bool SStoreSaleItemModel::sale()
{
    commit();
    if(i_id)
        i_nErr &= m_storeItem->saleReserved();
    else
        i_nErr &= m_storeItem->sale();
    return i_nErr;
}

bool SStoreSaleItemModel::reserve()
{
    commit();
    i_nErr &= m_storeItem->reserve();
    return i_nErr;
}

bool SStoreSaleItemModel::unsale()
{
    setState(State::Cancelled);
    i_valuesMap.insert("cancellation_reason", "unsale");
    commit();
    i_nErr &= m_storeItem->unsale();
    return i_nErr;
}

bool SStoreSaleItemModel::free()
{
    setState(State::Cancelled);
    i_valuesMap.insert("cancellation_reason", "cancel reserve");
    commit();
    i_nErr &= m_storeItem->free();
    return i_nErr;
}

void SStoreSaleItemModel::setState(const State state)
{
    i_valuesMap.insert("is_cancellation", state);
}

bool SStoreSaleItemModel::isProfitable()
{
    // TODO: проверка указанной цены; выдача предупреждения при продаже в минус
    return 1;
}

bool SStoreSaleItemModel::integrityStatus()
{
    return nIntegrityErr;
}

bool SStoreSaleItemModel::commit()
{
    if(i_id)    // продажа ранее зарезервированного товара, возврат или отмена резерва
    {
        update();
    }
    else
    {
        insert();
    }

    return i_nErr;
}

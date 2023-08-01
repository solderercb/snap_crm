#include "sstoreitemmodel.h"

SStoreItemModel::SStoreItemModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "store_items";
    i_obligatoryFields << "" << "articul" << "dealer" << "name" << "state" << "category" << "store" << "shop_description" << "description";
    i_idColumnName = "id";
    i_logRecord->setType(SLogRecordModel::Part);
}

/*  Конструктор класса, предназначенный для использования при продаже товара
*/
SStoreItemModel::SStoreItemModel(const QList<QStandardItem *> &record, const int savedSaleQty, QObject *parent) :
    SStoreItemModel(parent)
{
    i_id = record.value(SaleOpColumns::ColItemId)->data(Qt::DisplayRole).toInt();
    m_saleId = record.value(SaleOpColumns::ColId)->data(Qt::DisplayRole).toInt();
    m_inPrice = record.value(SaleOpColumns::ColInPrice)->data(Qt::DisplayRole).toDouble();
    m_price = record.value(SaleOpColumns::ColPrice)->data(Qt::DisplayRole).toDouble();
    m_returnPercent = record.value(SaleOpColumns::ColRetPercent)->data(Qt::DisplayRole).toInt();
    m_isRealization = record.value(SaleOpColumns::ColRealization)->data(Qt::DisplayRole).toBool();
    m_savedSaleQty = savedSaleQty;
    m_saleQty = record.value(SaleOpColumns::ColCount)->data(Qt::DisplayRole).toInt();
    m_saleObjId = record.value(SaleOpColumns::ColObjId)->data(Qt::DisplayRole).toInt();
    m_saleUser = record.value(SaleOpColumns::ColUser)->data(Qt::DisplayRole).toInt();
    m_dealer = record.value(SaleOpColumns::ColDealer)->data(Qt::DisplayRole).toInt();
    m_buyerId = record.value(SaleOpColumns::ColBuyer)->data(Qt::DisplayRole).toInt();
    i_logRecord->setItemId(i_id);
    if(m_buyerId)
        i_logRecord->setClient(m_buyerId);
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

int id();
bool SStoreItemModel::isHidden()
{
    return m_isHidden;
}

void SStoreItemModel::setHidden(const bool Hidden)
{
    i_valuesMap.insert("Hidden", Hidden);
}

int SStoreItemModel::articul()
{
    return m_articul;
}

void SStoreItemModel::setArticul(const int articul)
{
    i_valuesMap.insert("articul", articul);
}

int SStoreItemModel::dealer()
{
    return m_dealer;
}

void SStoreItemModel::setDealer(const int dealer)
{
    i_valuesMap.insert("dealer", dealer);
}

bool SStoreItemModel::isRealization()
{
    return m_isRealization;
}

void SStoreItemModel::setRealization(const bool is_realization)
{
    i_valuesMap.insert("is_realization", is_realization);
}

int SStoreItemModel::isDealerLock()
{
    return m_isDealerLock;
}

void SStoreItemModel::setDealerLock(const int dealer_lock)
{
    i_valuesMap.insert("dealer_lock", dealer_lock);
}

QString SStoreItemModel::name()
{
    return m_name;
}

void SStoreItemModel::setName(const QString& name)
{
    i_valuesMap.insert("name", name);
}

int SStoreItemModel::state()
{
    return m_state;
}

void SStoreItemModel::setState(const int state)
{
    i_valuesMap.insert("state", state);
}

int SStoreItemModel::category()
{
    return m_category;
}

void SStoreItemModel::setCategory(const int category)
{
    i_valuesMap.insert("category", category);
}

int SStoreItemModel::store()
{
    return m_store;
}

void SStoreItemModel::setStore(const int store)
{
    i_valuesMap.insert("store", store);
}

QDateTime SStoreItemModel::updated()
{
    return m_updated;
}

void SStoreItemModel::setUpdated(const QDateTime updated)
{
    i_valuesMap.insert("updated", updated);
}

int SStoreItemModel::count()
{
    return m_count;
}

void SStoreItemModel::setCount(const int count)
{
    i_valuesMap.insert("count", count);
}

int SStoreItemModel::reserved()
{
    return m_reserved;
}

void SStoreItemModel::setReserved(const int reserved)
{
    i_valuesMap.insert("reserved", reserved);
}

int SStoreItemModel::units()
{
    return m_units;
}

void SStoreItemModel::setUnits(const int units)
{
    i_valuesMap.insert("units", units);
}

int SStoreItemModel::box()
{
    return m_box;
}

void SStoreItemModel::setBox(const int box)
{
    i_valuesMap.insert("box", box);
}

QString SStoreItemModel::boxName()
{
    return m_boxName;
}

void SStoreItemModel::setBoxName(const QString& box_name)
{
    i_valuesMap.insert("box_name", box_name);
}

int SStoreItemModel::priceOption()
{
    return m_priceOption;
}

void SStoreItemModel::setPriceOption(const int price_option)
{
    i_valuesMap.insert("price_option", price_option);
}

double SStoreItemModel::currencyRate()
{
    return m_currencyRate;
}

void SStoreItemModel::setCurrencyRate(const double currency_rate)
{
    i_valuesMap.insert("currency_rate", currency_rate);
}

double SStoreItemModel::inPrice()
{
    return m_inPrice;
}

void SStoreItemModel::setInPrice(const double in_price)
{
    i_valuesMap.insert("in_price", in_price);
}

double SStoreItemModel::price()
{
    return m_price;
}

void SStoreItemModel::setPrice(const double price)
{
    i_valuesMap.insert("price", price);
}

double SStoreItemModel::price2()
{
    return m_price2;
}

void SStoreItemModel::setPrice2(const double price2)
{
    i_valuesMap.insert("price2", price2);
}

double SStoreItemModel::price3()
{
    return m_price3;
}

void SStoreItemModel::setPrice3(const double price3)
{
    i_valuesMap.insert("price3", price3);
}

double SStoreItemModel::price4()
{
    return m_price4;
}

void SStoreItemModel::setPrice4(const double price4)
{
    i_valuesMap.insert("price4", price4);
}

double SStoreItemModel::price5()
{
    return m_price5;
}

void SStoreItemModel::setPrice5(const double price5)
{
    i_valuesMap.insert("price5", price5);
}

int SStoreItemModel::document()
{
    return m_document;
}

void SStoreItemModel::setDocument(const int document)
{
    i_valuesMap.insert("document", document);
}

int SStoreItemModel::partRequest()
{
    return m_partRequest;
}

void SStoreItemModel::setPartRequest(const int part_request)
{
    i_valuesMap.insert("part_request", part_request);
}

QString SStoreItemModel::shopTitle()
{
    return m_shopTitle;
}

void SStoreItemModel::setShopTitle(const QString& shop_title)
{
    i_valuesMap.insert("shop_title", shop_title);
}

QString SStoreItemModel::shopDescription()
{
    return m_shopDescription;
}

void SStoreItemModel::setShopDescription(const QString& shop_description)
{
    i_valuesMap.insert("shop_description", shop_description);
}

QString SStoreItemModel::SN()
{
    return m_SN;
}

void SStoreItemModel::setSN(const QString& SN)
{
    i_valuesMap.insert("SN", SN);
}

QString SStoreItemModel::PN()
{
    return m_PN;
}

void SStoreItemModel::setPN(const QString& PN)
{
    i_valuesMap.insert("PN", PN);
}

int SStoreItemModel::description()
{
    return m_description;
}

void SStoreItemModel::setDescription(const int description)
{
    i_valuesMap.insert("description", description);
}

int SStoreItemModel::isShopEnable()
{
    return m_isShopEnable;
}

void SStoreItemModel::setShopEnable(const int shop_enable)
{
    i_valuesMap.insert("shop_enable", shop_enable);
}

QString SStoreItemModel::intBarcode()
{
    return m_intBarcode;
}

void SStoreItemModel::setIntBarcode(const QString& int_barcode)
{
    i_valuesMap.insert("int_barcode", int_barcode);
}

int SStoreItemModel::extBarcode()
{
    return m_extBarcode;
}

void SStoreItemModel::setExtBarcode(const int ext_barcode)
{
    i_valuesMap.insert("ext_barcode", ext_barcode);
}

int SStoreItemModel::inCount()
{
    return m_inCount;
}

void SStoreItemModel::setInCount(const int in_count)
{
    i_valuesMap.insert("in_count", in_count);
}

double SStoreItemModel::inSumm()
{
    return m_inSumm;
}

void SStoreItemModel::setInSumm(const double in_summ)
{
    i_valuesMap.insert("in_summ", in_summ);
}

QString SStoreItemModel::notes()
{
    return m_notes;
}

void SStoreItemModel::setNotes(const QString& notes)
{
    i_valuesMap.insert("notes", notes);
}

//int SStoreItemModel::img1()
//{
//    return m_img1;
//}

//void SStoreItemModel::setImg1(const int img1)
//{
//    i_valuesMap.insert("img1", img1);
//}

//int SStoreItemModel::img2()
//{
//    return m_img2;
//}

//void SStoreItemModel::setImg2(const int img2)
//{
//    i_valuesMap.insert("img2", img2);
//}

//int SStoreItemModel::img3()
//{
//    return m_img3;
//}

//void SStoreItemModel::setImg3(const int img3)
//{
//    i_valuesMap.insert("img3", img3);
//}

//int SStoreItemModel::img4()
//{
//    return m_img4;
//}

//void SStoreItemModel::setImg4(const int img4)
//{
//    i_valuesMap.insert("img4", img4);
//}

//int SStoreItemModel::img5()
//{
//    return m_img5;
//}

//void SStoreItemModel::setImg5(const int img5)
//{
//    i_valuesMap.insert("img5", img5);
//}

int SStoreItemModel::minimumInStock()
{
    return m_minimumInStock;
}

void SStoreItemModel::setMinimumInStock(const int minimum_in_stock)
{
    i_valuesMap.insert("minimum_in_stock", minimum_in_stock);
}

int SStoreItemModel::sold()
{
    return m_sold;
}

void SStoreItemModel::setSold(const int sold)
{
    i_valuesMap.insert("sold", sold);
}

int SStoreItemModel::returnPercent()
{
    return m_returnPercent;
}

void SStoreItemModel::setReturnPercent(const int return_percent)
{
    i_valuesMap.insert("return_percent", return_percent);
}

int SStoreItemModel::warranty()
{
    return m_warranty;
}

void SStoreItemModel::setWarranty(const int warranty)
{
    i_valuesMap.insert("warranty", warranty);
}

int SStoreItemModel::warrantyDealer()
{
    return m_warrantyDealer;
}

void SStoreItemModel::setWarrantyDealer(const int warranty_dealer)
{
    i_valuesMap.insert("warranty_dealer", warranty_dealer);
}

bool SStoreItemModel::isNotForSale()
{
    return m_isNotForSale;
}

void SStoreItemModel::setNotForSale(const bool not_for_sale)
{
    i_valuesMap.insert("not_for_sale", not_for_sale);
}

int SStoreItemModel::stState()
{
    return m_stState;
}

void SStoreItemModel::setStState(const int st_state)
{
    i_valuesMap.insert("st_state", st_state);
}

QString SStoreItemModel::stNotes()
{
    return m_stNotes;
}

void SStoreItemModel::setStNotes(const QString& st_notes)
{
    i_valuesMap.insert("st_notes", st_notes);
}

bool SStoreItemModel::isGeHighlight()
{
    return m_isGeHighlight;
}

void SStoreItemModel::setGeHighlight(const bool ge_highlight)
{
    i_valuesMap.insert("ge_highlight", ge_highlight);
}

void SStoreItemModel::load()
{
    m_isHidden = i_query->value("Hidden").toBool();
    m_articul = i_query->value("articul").toInt();
    m_dealer = i_query->value("dealer").toInt();
    m_isRealization = i_query->value("is_realization").toBool();
    m_isDealerLock = i_query->value("dealer_lock").toInt();
    m_name = i_query->value("name").toString();
    m_state = i_query->value("state").toInt();
    m_category = i_query->value("category").toInt();
    m_store = i_query->value("store").toInt();
    i_createdUtc = i_query->value("created").toDateTime();
    m_updated = i_query->value("updated").toDateTime();
    m_count = i_query->value("count").toInt();
    m_reserved = i_query->value("reserved").toInt();
    m_units = i_query->value("units").toInt();
    m_box = i_query->value("box").toInt();
    m_boxName = i_query->value("box_name").toString();
    m_priceOption = i_query->value("price_option").toInt();
    m_currencyRate = i_query->value("currency_rate").toDouble();
    m_inPrice = i_query->value("in_price").toDouble();
    m_price = i_query->value("price").toDouble();
    m_price2 = i_query->value("price2").toDouble();
    m_price3 = i_query->value("price3").toDouble();
    m_price4 = i_query->value("price4").toDouble();
    m_price5 = i_query->value("price5").toDouble();
    m_document = i_query->value("document").toInt();
    m_partRequest = i_query->value("part_request").toInt();
    m_shopTitle = i_query->value("shop_title").toString();
    m_shopDescription = i_query->value("shop_description").toString();
    m_SN = i_query->value("SN").toString();
    m_PN = i_query->value("PN").toString();
    m_description = i_query->value("description").toInt();
    m_isShopEnable = i_query->value("shop_enable").toInt();
    m_intBarcode = i_query->value("int_barcode").toString();
    m_extBarcode = i_query->value("ext_barcode").toInt();
    m_inCount = i_query->value("in_count").toInt();
    m_inSumm = i_query->value("in_summ").toDouble();
    m_notes = i_query->value("notes").toString();
//    m_img1 = i_query->value("img1").toInt();
//    m_img2 = i_query->value("img2").toInt();
//    m_img3 = i_query->value("img3").toInt();
//    m_img4 = i_query->value("img4").toInt();
//    m_img5 = i_query->value("img5").toInt();
    m_minimumInStock = i_query->value("minimum_in_stock").toInt();
    m_sold = i_query->value("sold").toInt();
    m_returnPercent = i_query->value("return_percent").toInt();
    m_warranty = i_query->value("warranty").toInt();
    m_warrantyDealer = i_query->value("warranty_dealer").toInt();
    m_isNotForSale = i_query->value("not_for_sale").toBool();
    m_stState = i_query->value("st_state").toInt();
    m_stNotes = i_query->value("st_notes").toString();
    m_isGeHighlight = i_query->value("ge_highlight").toBool();
    //    m_lastStocktaking_date = i_query->value("last_stocktaking_date").toDate();
}

void SStoreItemModel::setSaleMode(SaleMode mode)
{
    m_saleMode = mode;
    if(m_saleMode == SaleMode::Store)
        i_logRecord->setDocumentId(m_saleObjId);
}

QMap<int, int>* SStoreItemModel::loadQtys()
{
    QMap<int, int> *record = new QMap<int, int>();

    QUERY_EXEC(i_query,i_nErr)(QUERY_SEL_ITEM_ACTUAL_QTY(i_id));
    if(i_nErr && i_query->first())
        for(int i = 0; i < i_query->record().count(); i++)
            record->insert(i, i_query->record().value(i).toInt());

    return record;
}

void SStoreItemModel::delIntegrityCheckObjects()
{
    if(m_qtysBeforeSale)
    {
        delete m_qtysBeforeSale;
        m_qtysBeforeSale = nullptr;
    }
    if(m_qtysAfterSale)
    {
        delete m_qtysAfterSale;
        m_qtysAfterSale = nullptr;
    }
}

/*  Проверка доступности товара перед продажей
 *  возвращает 1 в случае недоступности запрошенного кол-ва
*/
bool SStoreItemModel::checkBeforeSale()
{
    m_qtysBeforeSale = loadQtys();

    // При продаже ранее зарезервированного товара допускается изменение кол-ва (вдруг клиент решил купить меньше/больше);
    // в этом случае при проверке не будет учитываться старое кол-во
    if(m_saleQty > (m_qtysBeforeSale->value(QtyField::Count) - m_qtysBeforeSale->value(QtyField::Reserved) + m_savedSaleQty) )
    {
        delIntegrityCheckObjects();
        return 1;
    }
    return 0;
}

bool SStoreItemModel::checkAfterSale()
{
    bool nIntegrity = 1;
    m_qtysAfterSale = loadQtys();

    try
    {
        switch(m_opOnItemType)
        {
            case Sale:
            case WriteOff:
            {
                if( m_qtysBeforeSale->value(QtyField::Count) - m_saleQty != m_qtysAfterSale->value(QtyField::Count) )   // `count`
                    throw 1;
                if( m_qtysBeforeSale->value(QtyField::Sold) + m_saleQty != m_qtysAfterSale->value(QtyField::Sold) )   // `sold`
                    throw 1;
                break;
            }
            case Reserve:
            {
                if( m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty + m_saleQty != m_qtysAfterSale->value(QtyField::Reserved) )   // `reserved`
                    throw 1;
                break;
            }
            case SaleReserved:
            case SaleRepair:
            {
                if( m_qtysBeforeSale->value(QtyField::Count) - m_saleQty != m_qtysAfterSale->value(QtyField::Count) )   // `count`
                    throw 1;
                if( m_qtysBeforeSale->value(QtyField::Sold) + m_saleQty != m_qtysAfterSale->value(QtyField::Sold) )   // `sold`
                    throw 1;
                if( m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty != m_qtysAfterSale->value(QtyField::Reserved) )   // `reserved`
                    throw 1;
                break;
            }
            case Unsale:
            case CancelWriteOff:
            {
                if( m_qtysBeforeSale->value(QtyField::Count) + m_savedSaleQty != m_qtysAfterSale->value(QtyField::Count) )   // `count`
                    throw 1;
                if( m_qtysBeforeSale->value(QtyField::Sold) - m_savedSaleQty != m_qtysAfterSale->value(QtyField::Sold) )   // `sold`
                    throw 1;
                break;
            }
            case Free:
            {
                if( m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty != m_qtysAfterSale->value(QtyField::Reserved) )   // `reserved`
                    throw 1;
                break;
            }
            case UnsaleRepair:
            {
                if( m_qtysBeforeSale->value(QtyField::Count) + m_saleQty != m_qtysAfterSale->value(QtyField::Count) )   // `count`
                    throw 1;
                if( m_qtysBeforeSale->value(QtyField::Sold) - m_saleQty != m_qtysAfterSale->value(QtyField::Sold) )   // `sold`
                    throw 1;

                if( m_qtysBeforeSale->value(QtyField::Reserved) + m_savedSaleQty != m_qtysAfterSale->value(QtyField::Reserved) )   // `reserved`
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
    setCount(m_qtysBeforeSale->value(QtyField::Count) - m_saleQty);
    setSold(m_qtysBeforeSale->value(QtyField::Sold) + m_saleQty);
    i_logRecord->setText(logText);

    if(!commit())
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Document))
        return 0;

    return i_nErr;
}

bool SStoreItemModel::reserve(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Reserve;
    setReserved(m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty + m_saleQty);
    i_logRecord->setText(logText);
    if(!commit())
        return 0;

    return i_nErr;
}

bool SStoreItemModel::saleReserved(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::SaleReserved;
    setCount(m_qtysBeforeSale->value(QtyField::Count) - m_saleQty);
    setSold(m_qtysBeforeSale->value(QtyField::Sold) + m_saleQty);
    setReserved(m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty);
    i_logRecord->setText(logText);

    if(!commit())
        return 0;

    return i_nErr;
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
    setReserved(m_qtysBeforeSale->value(QtyField::Reserved) - m_savedSaleQty);
    i_logRecord->setText(logText);
    if(!commit())
        return 0;

    return i_nErr;
}

bool SStoreItemModel::unsale(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    m_opOnItemType = OpOnItem::Unsale;
    setCount(m_qtysBeforeSale->value(QtyField::Count) + m_savedSaleQty);
    setSold(m_qtysBeforeSale->value(QtyField::Sold) - m_savedSaleQty);
    i_logRecord->setText(logText);
    if(!commit())
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Document))
        return 0;

    return i_nErr;
}

bool SStoreItemModel::unsaleRepair(const QString &logText)
{
    if(checkBeforeSale())
        return 0;

    // TODO: Возврат ремонта с возвратом запчастей и без возврата
    m_opOnItemType = OpOnItem::UnsaleRepair;
    setCount(m_qtysBeforeSale->value(QtyField::Count) + m_saleQty);
    setSold(m_qtysBeforeSale->value(QtyField::Sold) - m_saleQty);
    setReserved(m_qtysBeforeSale->value(QtyField::Reserved) + m_savedSaleQty);
    i_logRecord->setText(logText);
    if(!commit())
        return 0;

    if(!dealerRoyalty(SBalanceLogRecordModel::RoyaltyReason::Repair))
        return 0;

    return i_nErr;
}

bool SStoreItemModel::dealerRoyalty(const SBalanceLogRecordModel::RoyaltyReason source)
{
    if(!m_isRealization)
        return 1;

    double royaltyForItem = m_inPrice + (m_price - m_inPrice)*m_returnPercent/100;
    QString logText;

    if(royaltyForItem == 0)
        return 1;

    SClientModel *dealer = new SClientModel();
    dealer->setId(m_dealer);
    dealer->createBalanceObj();

    try
    {
        if(m_opOnItemType == Unsale || m_opOnItemType == UnsaleRepair)
        {
            double dealerRoyalty = m_savedSaleQty*royaltyForItem;
            if(m_opOnItemType == Unsale)
                logText = tr("Списание %1 по причение возврата %2ед. товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_savedSaleQty).arg(i_id);
            else
                logText = tr("Списание %1 за %2ед. товара %3, находившегося на реализации, по причение отмены выдачи ремонта").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_savedSaleQty).arg(i_id);
            i_nErr = dealer->updateBalance(-dealerRoyalty, logText, source, m_saleObjId);
        }
        else
        {
            double dealerRoyalty = m_saleQty*royaltyForItem;
            i_nErr = dealer->updateBalance(dealerRoyalty, tr("Зачисление %1 за %2ед. проданного товара %3, находившегося на реализации").arg(sysLocale.toCurrencyString(dealerRoyalty)).arg(m_saleQty).arg(i_id), source, m_saleObjId);
        }
    }
    catch (int)
    {
        i_nErr = 0;
    }
    delete dealer;

    return i_nErr;
}

void SStoreItemModel::showNotification(const QString &text)
{
    QMessageBox msgBox;

    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

bool SStoreItemModel::commit()
{
    Q_ASSERT_X(m_saleMode, "SStoreItemModel::commit()", "Не задан m_saleMode");
    if(i_id)
    {
        setUpdated(QDateTime::currentDateTime());
        update();
    }
    else
    {
        insert();
    }

    if(!checkAfterSale())
    {
        showNotification(tr("Ошибка целостности данных item_id = %1").arg(i_id));
        return 0;
    }

    i_logRecord->commit();

    // TODO: если последняя единица товара, нужно удалить ячейку и сделать соотв. запись в журнал (но! нужно чтобы при вызове updateWidgets() в таблице место не обновлялось)
    // а может и не нужно; в АСЦ удалялась, но за годы пользования я так и не понял зачем это сделано
    return i_nErr;
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


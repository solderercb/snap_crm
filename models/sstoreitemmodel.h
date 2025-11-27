#ifndef SSTOREITEMMODEL_H
#define SSTOREITEMMODEL_H

#include <SSingleRowJModel>
#include <SClientModel>

class SStandardItemModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(Hidden, Hidden, bool, 1)                                    \
    TABLE_FIELD(articul, articul, int, 1)                                   \
    TABLE_FIELD(dealer, dealer, int, 1)                                     \
    TABLE_FIELD(is_realization, isRealization, bool, 1)                     \
    TABLE_FIELD(dealer_lock, dealerLock, int, 1)                            \
    TABLE_FIELD(name, name, QString, 1)                                     \
    TABLE_FIELD(state, state, int, 1)                                       \
    TABLE_FIELD(category, category, int, 1)                                 \
    TABLE_FIELD(store, store, int, 1)                                       \
    TABLE_FIELD(created, created, QDateTime, 1)                             \
    TABLE_FIELD(updated, updated, QDateTime, 1)                             \
    TABLE_FIELD(count, count, int, 1)                                       \
    TABLE_FIELD(reserved, reserved, int, 1)                                 \
    TABLE_FIELD(units, units, int, 1)                                       \
    TABLE_FIELD(box, box, int, 1)                                           \
    TABLE_FIELD(box_name, boxName, QString, 1)                              \
    TABLE_FIELD(price_option, priceOption, int, 1)                          \
    TABLE_FIELD(currency_rate, currencyRate, double, 1)                     \
    TABLE_FIELD(in_price, inPrice, double, 1)                               \
    TABLE_FIELD(price, price, double, 1)                                    \
    TABLE_FIELD(price2, price2, double, 1)                                  \
    TABLE_FIELD(price3, price3, double, 1)                                  \
    TABLE_FIELD(price4, price4, double, 1)                                  \
    TABLE_FIELD(price5, price5, double, 1)                                  \
    TABLE_FIELD(document, document, int, 1)                                 \
    TABLE_FIELD(part_request, partRequest, int, 1)                          \
    TABLE_FIELD(shop_title, shopTitle, QString, 1)                          \
    TABLE_FIELD(shop_description, shopDescription, QString, 1)              \
    TABLE_FIELD(SN, SN, QString, 1)                                         \
    TABLE_FIELD(PN, PN, QString, 1)                                         \
    TABLE_FIELD(description, description, QString, 1)                       \
    TABLE_FIELD(shop_enable, shopEnable, bool, 1)                           \
    TABLE_FIELD(int_barcode, intBarcode, QString, 1)                        \
    TABLE_FIELD(ext_barcode, extBarcode, QString, 1)                        \
    TABLE_FIELD(in_count, inCount, int, 1)                                  \
    TABLE_FIELD(in_summ, inSumm, double, 1)                                 \
    TABLE_FIELD(notes, notes, QString, 1)                                   \
    TABLE_FIELD(img1, img1, int, 1)                                         \
    TABLE_FIELD(img2, img2, int, 1)                                         \
    TABLE_FIELD(img3, img3, int, 1)                                         \
    TABLE_FIELD(img4, img4, int, 1)                                         \
    TABLE_FIELD(img5, img5, int, 1)                                         \
    TABLE_FIELD(minimum_in_stock, minimumInStock, int, 1)                   \
    TABLE_FIELD(sold, sold, int, 1)                                         \
    TABLE_FIELD(return_percent, returnPercent, int, 1)                      \
    TABLE_FIELD(warranty, warranty, int, 1)                                 \
    TABLE_FIELD(warranty_dealer, warrantyDealer, int, 1)                    \
    TABLE_FIELD(not_for_sale, notForSale, bool, 1)                          \
    TABLE_FIELD(st_state, stState, int, 1)                                  \
    TABLE_FIELD(st_notes, stNotes, QString, 1)                              \
    TABLE_FIELD(ge_highlight, geHighlight, bool, 1)

class SStoreItemModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SRepairSaleItemModel;
public:
    enum OpOnItem { Nop = 0, Sale = 1, Reserve = 2, SaleReserved = 3, SaleRepair = 4, Unsale = 5, Free = 6, WriteOff = 7, CancelWriteOff = 8, UnsaleRepair = 9, ReserveRepair = 10 };
    enum QtyField { Count = 0, Sold = 1, Reserved = 2 };
    enum SaleMode {Store = 1, Repair = 2};
    enum PriceOption {PriceOptionService = 1, PriceOptionRetail = 2, PriceOptionWholesale = 3, PriceOptionWholesale2 = 4, PriceOptionWholesale3 = 5, PriceOptionWarranty = 255};
    Q_ENUM(PriceOption)
    explicit SStoreItemModel(QObject *parent = nullptr);
    ~SStoreItemModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SStandardItemModel *priceOptionsList();
    void setSaleMode(SaleMode mode);
    void delIntegrityCheckObjects();
    bool setData(const int index, const QVariant &data) override;
    bool checkBeforeSale();
    bool checkAfterSale();
    bool sale(const QString &logText);
    bool reserve(const QString &logText);
    bool saleReserved(const QString &logText);
    bool saleStore(const QString &logText);
    bool saleRepair(const QString &logText);
    bool free(const QString &logText);
    bool unsale(const QString &logText);
    bool unsaleRepair(const QString &logText);
    bool writeOff(const QString &logText);
    bool cancelWriteOff(const QString &logText);
    bool dealerRoyalty(const SBalanceLogRecordModel::RoyaltyReason);
    virtual bool commit() override;
protected:
    struct itemQtys {
        int count = 0;
        int sold = 0;
        int reserved = 0;
    };
public:
    virtual itemQtys* loadQtys();
    void setCommitedExpense(int commitedExpense);
    void setNewExpense(int newExpense);
    void setSaleEntityId(int saleObjId);
private:
    int m_commitedExpense = 0;
    int m_newExpense = 0;
    itemQtys *m_itemQtysBefore = nullptr;
    itemQtys *m_itemQtysAfter = nullptr;
    int m_saleMode = 0;
    int m_opOnItemType;
    int m_saleObjId = 0;
    void updateLogAssociatedRecId() override;
    void translateNames();
};

#endif // SSTOREITEMMODEL_H

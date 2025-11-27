#ifndef SSTORESALEITEMMODEL_H
#define SSTORESALEITEMMODEL_H

#include <SSingleRowJModel>

class SStoreItemModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(dealer, dealer, int, 0)                                     \
    TABLE_FIELD(item_id, itemId, int, 0)                                    \
    TABLE_FIELD(document_id, documentId, int, 0)                            \
    TABLE_FIELD(customer_id, buyer, int, 0)                                 \
    TABLE_FIELD(count, count, int, 0)                                       \
    TABLE_FIELD(in_price, inPrice, double, 0)                               \
    TABLE_FIELD(price, price, double, 0)                                    \
    TABLE_FIELD(warranty, warranty, int, 0)                                 \
    TABLE_FIELD(realizator_payed, realizatorPayed, bool, 0)                 \
    TABLE_FIELD(is_realization, isRealization, bool, 0)                     \
    TABLE_FIELD(dealer_payment, dealerPayment, int, 0)                      \
    TABLE_FIELD(return_percent, returnPercent, int, 0)                      \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(is_cancellation, state, bool, 0)                            \
    TABLE_FIELD(cancellation_reason, unsaleReason, QString, 0)              \
    TABLE_FIELD(sn, sn, QString, 0)                                         \
    TABLE_FIELD(d_category, dCategory, int, 0)

class SStoreSaleItemModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class SStoreSaleItemModelExt;
    friend class TClassTest;
signals:
    void qtyNotEnough();
public:
    enum State {Active = 0, Cancelled = 1};
    explicit SStoreSaleItemModel(QObject *parent = nullptr);
    ~SStoreSaleItemModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void initFieldWithPrevLoaded(const int index, const QVariant &value) override;
    bool setData(const int index, const QVariant &data) override;
    bool sale();
    bool reserve();
    bool unsale();
    bool free();
    bool isProfitable();
private:
    std::shared_ptr<SStoreItemModel> m_storeItem;
    bool commit() override;
protected slots:
    void setDataRework(const int index, QVariant &data);
    void logDataChange(const int, const QVariant &) override;
};

#endif // SSTORESALEITEMMODEL_H

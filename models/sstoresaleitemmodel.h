#ifndef SSTORESALEITEMMODEL_H
#define SSTORESALEITEMMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "global.h"
#include "scomrecord.h"
#include "sclientmodel.h"
#include "models/sstoreitemmodel.h"
#include "widgets/shortlivednotification.h"

class SStoreSaleItemModel : public SComRecord
{
    Q_OBJECT
signals:
    void qtyNotEnough();
public:
    enum State {Active = 0, Cancelled = 1};
    explicit SStoreSaleItemModel(QObject *parent = nullptr);
    explicit SStoreSaleItemModel(const QList<QStandardItem *> &record, QObject *parent = nullptr);
    ~SStoreSaleItemModel();
    int id();
    int dealer();
    void setDealer(const int);
    int itemId();
    void setItemId(const int);
    int documentId();
    void setDocumentId(const int);
    int buyer();
    void setBuyer(const int);
    void unsetBuyer();
    int count();
    void setCount(const int);
    float inPrice();
    void setInPrice(const float);
    float price();
    void setPrice(const float);
    int warranty();
    void setWarranty(const int);
    bool realizatorPayed();
    void setRealizatorPayed(const bool);
    bool isRealization();
    void setIsRealization(const bool);
    int dealerPayment();
    void setDealerPayment(const int);
    int returnPercent();
    void setReturnPercent(const int);
    bool state();
    void setState(const State);
    void setCancellationReason(const QString&);
    int user();
    void setUser(const int);
    QString unsaleReason();
    void setExtraUnsaleReason(const QString&);
    QString sn();
    void setSN(const QString&);
    int dCategory();
    void setDCategory(const int);
    void setMode(int);
    bool sale();
    bool reserve();
    bool unsale();
    bool free();
    bool isProfitable();
    void setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue = QVariant()) override;
private:
    SStoreItemModel *m_storeItem = nullptr;
    int m_itemId = 0;
    QString m_itemName;
    int m_buyer = 0;
    int m_docId = 0;
    int m_avail = 0;
    int m_dealer = 0;
    int m_savedCount = 0;
    int m_count = 0;
    float m_inPrice = 0;
    float m_price = 0;
    int m_warranty;
    bool m_realizatorPayed;
    bool m_isRealization = 0;
    int m_dealerPayment;
    int m_returnPercent = 0;
    int m_user;
    bool m_state;
    QString m_sn;
    int m_dCategory;
    bool commit();
};

#endif // SSTORESALEITEMMODEL_H

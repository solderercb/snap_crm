#ifndef SREPAIRSALEITEMMODEL_H
#define SREPAIRSALEITEMMODEL_H

#include "scomrecord.h"
#include <QObject>
#include <QSqlRecord>
#include "models/sstoreitemmodel.h"
#include "widgets/shortlivednotification.h"

class SRepairSaleItemModel : public SComRecord
{
    Q_OBJECT
public:
    enum State {Requested = 0, EngineerBasket = 1, RepairLinked = 2, Sold = 3, Archive = 4};
    explicit SRepairSaleItemModel(QObject *parent = nullptr);
    explicit SRepairSaleItemModel(const QList<QStandardItem *> &record, QObject *parent = nullptr);
    ~SRepairSaleItemModel();
    int id();
    void load(const int);
    int itemId();
    void setItemId(const int);
    QString name();
    void setName(const QString);
    int count();
    void setCount(const int, const QVariant oldValue = QVariant());
    void setCreated(const QDateTime);
    int fromUser();
    void setFromUser(const int);
    int toUser();
    void setToUser(const int, const QVariant oldValue = QVariant());
    QString notes();
    void setNotes(const QString);
    int state();
    void setState(const int);
    int repairId();
    void setRepairId(const int);
    int workId();
    void setWorkId(const int);
    double price();
    void setPrice(const double, const QVariant oldValue = QVariant());
    QString sn();
    void setSN(const QString, const QVariant oldValue = QVariant());
    int warranty();
    void setWarranty(const int, const QVariant oldValue = QVariant());
    bool rLock();
    void setRLock(const bool);
    bool request();
    bool reserve();
    bool commit();
    bool linkRepair(const int);
    bool sale();
    bool unsale();
    bool unlinkRepair();
    bool free();
    void setQueryField(const int fieldNum, const QVariant value, const QVariant oldValue) override;
private:
    SStoreItemModel *m_storeItem = nullptr;
    int m_itemId;
    QString m_name;
    int m_count;
    int m_fromUser;
    int m_toUser;
    QString m_notes;
    int m_state;
    int m_repairId;
    int m_workId;
    double m_price;
    QString m_sn;
    int m_warranty;
    bool m_rLock;
};

#endif // SREPAIRSALEITEMMODEL_H

#ifndef SREPAIRSALEITEMMODEL_H
#define SREPAIRSALEITEMMODEL_H

#include <SSingleRowJModel>

class SStoreItemModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(item_id, itemId, int, 0)                                    \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(count, count, int, 0)                                       \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(from_user, fromUser, int, 0)                                \
    TABLE_FIELD(to_user, toUser, int, 0)                                    \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(state, state, int, 0)                                       \
    TABLE_FIELD(repair_id, repairId, int, 0)                                \
    TABLE_FIELD(work_id, workId, int, 0)                                    \
    TABLE_FIELD(price, price, double, 0)                                    \
    TABLE_FIELD(sn, sn, QString, 0)                                         \
    TABLE_FIELD(warranty, warranty, int, 0)                                 \
    TABLE_FIELD(r_lock, rLock, bool, 0)

class SRepairSaleItemModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class SRepairSaleItemModelExt;
    friend class TClassTest;
public:
    enum State {Requested = 0, EngineerBasket = 1, RepairLinked = 2, Sold = 3, Archive = 4, New = 255};
    explicit SRepairSaleItemModel(QObject *parent = nullptr);
    ~SRepairSaleItemModel();
    int count_();
    void setToUser(const int, const QVariant oldValue = QVariant());
    void repairChanged(const QVariant &data);
    void priceChanged(const QVariant &value);
    void serialChanged(const QVariant &value);
    void warrantyChanged(const QVariant &value);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void initFieldWithPrevLoaded(const int index, const QVariant &value) override;
    bool setData(const int index, const QVariant &value) override;
    void load() override;
    bool request();
    bool reserve();
    bool commit() override;
    bool sale();
    bool unsale();
    bool unlinkRepair();
    bool free();
private:
    std::shared_ptr<SStoreItemModel> m_storeItem;
    void updateLogAssociatedRecId() override;
    void engineerChanged();
    void countChanged(const QVariant &value);
    void addRemoveHook();
    void constructNote();
protected slots:
    void logDataChange(const int index, const QVariant &data) override;
    void setDataRework(const int index, QVariant &data);
};

#endif // SREPAIRSALEITEMMODEL_H

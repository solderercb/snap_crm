#ifndef SWORKMODEL_H
#define SWORKMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(repair, repair, int, 0)                                     \
    TABLE_FIELD(document_id, documentId, int, 0)                            \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(price, price, double, 0)                                    \
    TABLE_FIELD(count, count, int, 0)                                       \
    TABLE_FIELD(warranty, warranty, int, 0)                                 \
    TABLE_FIELD(price_id, priceId, int, 0)                                  \
    TABLE_FIELD(is_pay, isPay, bool, 0)                                     \
    TABLE_FIELD(added, created, QDateTime, 0)                               \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(pay_repair, payRepair, int, 0)                              \
    TABLE_FIELD(pay_repair_quick, payRepairQuick, int, 0)                   \
    TABLE_FIELD(salary_summ, salarySumm, double, 0)

class SWorkModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class SWorkModelExt;
    friend class TClassTest;
public:
    enum Type{Regular = 0, CartridgeRefill, CartridgeChipReplace, CartridgeDrumReplace, CartridgeBladeReplace, CartridgeReplaceOfWorn,
              CartridgeRefillPlus, CartridgeMagRollerReplace,  CartridgeProphilaxy, CartridgePrimRollerReplace,
              CartridgeMarkerReplace, CartridgeScraperReplace, CartridgeBushingsReplace};
    Q_ENUM(Type)
    explicit SWorkModel(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool setData(const int index, const QVariant &data) override;
    bool remove();
    bool commit() override;
private:
    void updateLogAssociatedRecId() override;
    void initFieldWithPrevLoaded(const int index, const QVariant &value) override;
    void addAdminModeMarkToLogText(QString &text);
    void userBeforeChange(const int userId);
    void constructInitialLogMsg(const int column);
    void nameChanged();
    void repairChanged(const QVariant &data);
    void countChanged();
    void priceChanged();
    void priceIdChanged();
    void warrantyChanged();
    void userChanged();
private slots:
    void setDataRework(const int index, QVariant &data);
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SWORKMODEL_H

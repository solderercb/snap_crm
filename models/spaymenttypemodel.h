#ifndef SPAYMENTTYPEMODEL_H
#define SPAYMENTTYPEMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(client, client, int, 0)                                     \
    TABLE_FIELD(periodic, periodicEnabled, bool, 0)                         \
    TABLE_FIELD(pay_date, pariodicDate, QDateTime, 0)                       \
    TABLE_FIELD(def_summ, defAmount, double, 0)                             \
    TABLE_FIELD(reason, reason, QString, 0)                                 \
    TABLE_FIELD(is_archive, isArchive, bool, 0)                             \
    TABLE_FIELD(updated_at, updated, QDateTime, 0)                          \
    TABLE_FIELD(payment_system, account, int, 0)

class SPaymentTypeModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SPaymentTypeModel(QObject *parent = nullptr);
    ~SPaymentTypeModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(const int);
    bool commit() override;
    int accountIndex();
private:
    void typeChanged(const int type);
    void nameChanged(const QString name);
    void clientChanged(const int id);
    void periodicStateChanged(const bool state);
    void periodicDateChanged(const QDateTime dateTime);
    void defAmountChanged(const double amount);
    void reasonChanged(const QString reason);
    void isArchiveChanged(const bool isArchive);
    void accountChanged(const int id);
protected slots:
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SPAYMENTTYPEMODEL_H

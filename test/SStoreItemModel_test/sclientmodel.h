/* Это минимальный набор глобальных переменных и методов, необходимый для теста
*/
#ifndef SCLIENTMODEL_H
#define SCLIENTMODEL_H

#include <QObject>

class SBalanceLogRecordModel
{
public:
    enum RoyaltyReason {Repair = 0, Document, CashOrder};
    explicit SBalanceLogRecordModel(QObject *parent = nullptr);
    explicit SBalanceLogRecordModel(int client, QObject *parent = nullptr);
    ~SBalanceLogRecordModel();
};

class SClientModel
{
public:
    enum Categories {All, Companies, Brokers, Suppliers, Regulars, Problematic, Resellers};
    enum BinaryOption{Company = 1, Regular = 2, Broker = 4, IgnoreCalls = 8, PreferCashless = 16, TakeLong = 32, Supplier = 64, SaleOrReturn = 128, BalanceEnabled = 256, Bad = 512, Archived = 1024};
    enum EditStrategy { OnFieldChange, OnManualSubmit };
    enum OptionsOutputForm {Standard = 1, Short = 2};
    explicit SClientModel(int id = 0, QObject *parent = nullptr);
    ~SClientModel();
    void setPrimaryKey(const int);
    void createBalanceObj();
    bool updateBalance(const double amount, const QString &text);
    bool updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason, const int doc_id);
};

#endif // SCLIENTMODEL_H

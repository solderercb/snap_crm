/* Это минимальный набор глобальных переменных и методов, необходимый для теста
*/
#ifndef SCLIENTMODEL_H
#define SCLIENTMODEL_H

#include <QObject>
#include <../../models/sphonesmodel.h>

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
    int id(){return 1952;};
    void setId(const int);
    void load(int);
    void createBalanceObj();
    bool updateBalance(const double amount, const QString &text);
    bool updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason, const int doc_id);
    bool balanceEnough(const double amount){return 0;};
    bool isNew(){return 0;};
    int type(){return 1;};
    int options(){return 0;};
    int priceColumnIndex(){return 0;};
    int adTypeIndex(){return 0;};
    double balance(){return 0;};
    void setFirstName(const QString &);
    QString firstName(){return "Иван";};
    void setLastName(const QString &);
    QString lastName(){return "Иванов";};
    void setPatronymicName(const QString &);
    QString patronymicName(){return "Иванович";};
    void appendLogText(const QString &);
    void setAdType(int);
    void addPhone(const QString &, int);
    void clear();
    bool commit(){return 1;};
    void updatePurchases(int);
    SPhonesModel* phones(){return new SPhonesModel;};
};

#endif // SCLIENTMODEL_H

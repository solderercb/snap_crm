#ifndef SCLIENTMODEL_H
#define SCLIENTMODEL_H

#include <SSingleRowJModel>
#include <QMessageBox>

class SStandardItemModel;
class SPhonesModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(client, client, int, 0)                                     \
    TABLE_FIELD(summ, amount, double, 0)                                    \
    TABLE_FIELD(direction, direction, bool, 0)                              \
    TABLE_FIELD(reason, reason, QString, 0)                                 \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(office, office, int, 0)                                     \
    TABLE_FIELD(uid, user, int, 0)                                          \
    TABLE_FIELD(dealer_payment, dealerPayment, int, 0)

class SBalanceLogRecordModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SClientModel;
public:
    enum RoyaltyReason {Repair = 0, Document, CashOrder};
    explicit SBalanceLogRecordModel(QObject *parent = nullptr);
    explicit SBalanceLogRecordModel(int client, QObject *parent = nullptr);
    ~SBalanceLogRecordModel();
    void setRepair(const int);
    void setDocumentId(const int);
    void setCashOrderId(const int);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void setOfficeIndex(const int index);
    bool commit() override;
    bool commit(const double);
    bool commit(const double, const QString &);
private:
    QString m_reason;
    int m_client;
    void setAllState(ModifiedField::State state) override;
    void updateLogAssociatedRecId() override;
protected slots:
    void logDataChange(const int, const QVariant &) override;
};

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(creator, creator, int, 0)                                   \
    TABLE_FIELD(name, firstName, QString, 1)                                \
    TABLE_FIELD(surname, lastName, QString, 1)                              \
    TABLE_FIELD(patronymic, patronymicName, QString, 1)                     \
    TABLE_FIELD(agent_phone_mask, agentPhoneMask, int, 0)                   \
    TABLE_FIELD(agent2_phone_mask, agent2PhoneMask, int, 0)                 \
    TABLE_FIELD(address, address, QString, 1)                               \
    TABLE_FIELD(post_index, postIndex, QString, 1)                          \
    TABLE_FIELD(passport_num, passportNum, QString, 1)                      \
    TABLE_FIELD(passport_date, passportIssuedDate, QDate, 1)                \
    TABLE_FIELD(passport_organ, passportIssuedBy, QString, 1)               \
    TABLE_FIELD(state, state, int, 1)                                       \
    TABLE_FIELD(type, isCompany, int, 1)                                    \
    TABLE_FIELD(birthday, birthday, QDate, 1)                               \
    TABLE_FIELD(memorial, memorial, QString, 1)                             \
    TABLE_FIELD(notes, notes, QString, 1)                                   \
    TABLE_FIELD(is_regular, isRegular, bool, 1)                             \
    TABLE_FIELD(is_dealer, isSupplier, bool, 1)                             \
    TABLE_FIELD(balance_enable, isBalanceEnabled, bool, 1)                  \
    TABLE_FIELD(prefer_cashless, isPreferCashless, bool, 1)                 \
    TABLE_FIELD(take_long, isTakeLong, bool, 1)                             \
    TABLE_FIELD(ignore_calls, isIgnoreCalls, bool, 1)                       \
    TABLE_FIELD(is_bad, isBad, bool, 1)                                     \
    TABLE_FIELD(is_realizator, isGivesItemsForSale, bool, 1)                \
    TABLE_FIELD(is_agent, isBroker, bool, 1)                                \
    TABLE_FIELD(visit_source, adType, int, 0)                               \
    TABLE_FIELD(photo_id, photoId, int, 0)                                  \
    TABLE_FIELD(INN, INN, QString, 1)                                       \
    TABLE_FIELD(KPP, KPP, QString, 1)                                       \
    TABLE_FIELD(OGRN, OGRN, QString, 1)                                     \
    TABLE_FIELD(web_password, web_password, QString, 1)                     \
    TABLE_FIELD(ur_name, urName, QString, 1)                                \
    TABLE_FIELD(email, email, QString, 1)                                   \
    TABLE_FIELD(icq, icq, QString, 0)                                       \
    TABLE_FIELD(skype, skype, QString, 0)                                   \
    TABLE_FIELD(viber, viber, QString, 0)                                   \
    TABLE_FIELD(telegram, telegram, QString, 0)                             \
    TABLE_FIELD(site, site, QString, 1)                                     \
    TABLE_FIELD(whatsapp, whatsapp, QString, 0)                             \
    TABLE_FIELD(agent_name, agentName, QString, 0)                          \
    TABLE_FIELD(agent_surname, agentSurname, QString, 0)                    \
    TABLE_FIELD(agent_patronymic, agentPatronymic, QString, 0)              \
    TABLE_FIELD(agent_phone, agentPhone, QString, 0)                        \
    TABLE_FIELD(agent_phone_clean, agentPhoneClean, QString, 0)             \
    TABLE_FIELD(agent2_name, agent2Name, QString, 0)                        \
    TABLE_FIELD(agent2_surname, agent2Surname, QString, 0)                  \
    TABLE_FIELD(agent2_patronymic, agent2Patronymic, QString, 0)            \
    TABLE_FIELD(agent2_phone, agent2Phone, QString, 0)                      \
    TABLE_FIELD(agent2_phone_clean, agent2PhoneClean, QString, 0)           \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(balance, balance, double, 1)                                \
    TABLE_FIELD(price_col, priceColumn, int, 1)                             \
    TABLE_FIELD(repairs, repairs, int, 1)                                   \
    TABLE_FIELD(purchases, purchases, int, 1)                               \
    TABLE_FIELD(token, token, QString, 0)                                   \
    TABLE_FIELD(employee, employeeId, int, 0)                               \
    TABLE_FIELD(short_name, shortName, QString, 1)

// Список дополнительных полей для отчетов
// Описание смотри в sdatabaserecord_new.h
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(photo, photo)                                   \
    ADDITIONAL_REPORT_FIELD(fullLongName, fullLongName)                     \
    ADDITIONAL_REPORT_FIELD(fullShortName, fullShortName)                   \
    ADDITIONAL_REPORT_FIELD(isArchived, isArchived)                         \
    ADDITIONAL_REPORT_FIELD(adTypeStr, adTypeStr)

class SClientModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
public:
    enum Categories {All, Companies, Brokers, Suppliers, Regulars, Problematic, Resellers};
    Q_ENUM(Categories)
    enum BinaryOption{Company = 1, Regular = 2, Broker = 4, IgnoreCalls = 8, PreferCashless = 16, TakeLong = 32, Supplier = 64, SaleOrReturn = 128, BalanceEnabled = 256, Bad = 512, Archived = 1024};
    enum EditStrategy { OnFieldChange, OnManualSubmit };
    enum OptionsOutputForm {Standard = 1, Short = 2};
    explicit SClientModel(int id = 0, QObject *parent = nullptr);
    ~SClientModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SStandardItemModel* categoriesList();
    bool isNew();
    void load() override;
    void load(int id);
    void clear();
    void setEditStrategy(SClientModel::EditStrategy strategy);
    bool isStandAlone();
    SPhonesModel* phones();
    void setPhonesModel(SPhonesModel*);
    void addPhone(const QString &number, int numberMask = 0, int messangers = 0);
    bool removePhone(const QString&);
    QStringList optionsList(int format = OptionsOutputForm::Standard);
    void setAdTypeIndex(const int index);
    int adTypeIndex();
    bool setBalanceEnabled(bool state = 1);
    void loadBalance();
    QString balanceStr();
    void createBalanceObj();
    SBalanceLogRecordModel *balanceObj();
    void deleteBalanceObj();
    bool updateBalance(const double amount, const QString &text);
    bool updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason, const int doc_id);
    bool balanceEnough(const double amount);
    bool receiptBalance();
    bool paymentBalance();
    int priceColumnIndex();
    QString priceColumnStr();
    void updateRepairs(const int val);
    void updatePurchases(int);
    void showNotification(const QString &, const QMessageBox::Icon);
    QString genWebPass();
    bool commit() override;
    void initDemo() override;
    bool printBOQDefaultState();
private:
    bool m_standAlone = 0;
    int m_editStrategy = OnManualSubmit;
    SPhonesModel *m_phones;
    SBalanceLogRecordModel *balanceLog = nullptr;
    bool m_notificationShown = 0;
    QString fieldsForSelectQuery() override;
    void translateNames();
    void updateLogAssociatedRecId() override;
public Q_SLOTS:
    QVariant fullLongName();   // Фамилия Имя Отчество
    QVariant fullShortName();  // Фамилия И. О.
    QVariant isArchived();
    QVariant adTypeStr();
    QVariant photo();
protected slots:
    void setDataRework(const int index, QVariant &data);
};

#endif // SCLIENTMODEL_H

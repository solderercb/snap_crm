#ifndef SCLIENTMODEL_H
#define SCLIENTMODEL_H

#include <QObject>
#include <QMessageBox>
#include <QRandomGenerator>
#include "ssqlquerymodel.h"
#include "com_sql_queries.h"
#include "scomrecord.h"
#include "scashregistermodel.h"
#include "slogrecordmodel.h"
#include "sphonemodel.h"

class SPhonesModel : public QObject
{
    Q_OBJECT
public:
    explicit SPhonesModel(int client = 0, QObject *parent = nullptr);
    ~SPhonesModel();
    SPhoneModel* primary();
    void setPrimary(SPhoneModel*);
    QList<SPhoneModel*> phonesList();
    bool load(int);
    void add(SPhoneModel*);
    void remove(int id);
    bool isEmpty();
    void setClient(const int id);
    bool commit();
    void clear();

private:
    QSqlQuery *query;
    SLogRecordModel *logRecord;
    QList<SPhoneModel*> m_phonesList;
    SPhoneModel* phoneItemHandler(const QSqlRecord &phone = QSqlRecord());
};

class SBalanceLogRecordModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SBalanceLogRecordModel(QObject *parent = nullptr);
    explicit SBalanceLogRecordModel(int, QObject *parent = nullptr);
    ~SBalanceLogRecordModel();
    void setClient(int);
    void setText(const QString &);
    void setDirection(float);
    void setDocumentId(int);
    bool commit(const float);
    bool commit(const float, const QString &);
private:
    QString m_reason;
    int m_client;
};

class SClientModel : public SComRecord
{
    Q_OBJECT
public:
    enum Option{Regular = 1, Dealer = 2, BalanceEnabled = 4, PreferCashless = 8, TakeLong = 16, IgnoreCalls = 32, Bad = 64, Realizator = 128, Agent = 256};
    enum EditStrategy { OnFieldChange, OnManualSubmit };
    explicit SClientModel(int id = 0, QObject *parent = nullptr);
    ~SClientModel();
    bool isClear();
    void load(int id);
    void setEditStrategy(SClientModel::EditStrategy strategy);
    bool isStandAlone();
    int id();
    void setId(const int);
    QString firstName();
    void setFirstName(const QString&);
    QString lastName();
    void setLastName(const QString&);
    QString patronymicName();
    void setPatronymicName(const QString&);
    QString fullLongName();   // Фамилия Имя Отчество
    QString fullShortName();  // Фамилия И. О.
    SPhonesModel* phones();
    bool setPhonesModel(SPhonesModel*);
    bool addPhone(const QString &number, int numberMask = 0, int messangers = 0);
    bool removePhone(QString&);
    QString address();
    bool setAddress(QString&);
    QString postIndex();
    bool setPostIndex(QString&);
    QString passportNum();
    bool setPassportNum(QString&);
    QString passportIssuedDate();
    bool setPassportIssuedDate(QString&);
    QString passportIssuedBy();
    bool setPassportIssuedBy(QString&);
    bool state();
    bool setState(bool);
    bool type();
    bool setType();
    QString birthday();
    bool setBirthday(QString&);
    QString memorial();
    bool setMemorial(QString&);
    QString notes();
    bool setNotes(QString&);
    int options();
    bool setOptions(int);
    int adType();
    void setAdType(const int);
    int adTypeIndex();
    QByteArray* photo();
    bool setPhoto(QByteArray*);
    QString INN();
    bool setINN(QString&);
    QString KPP();
    bool setKPP(QString&);
    QString OGRN();
    bool setOGRN(QString&);
    QString web_password();
    bool setWeb_Password(QString&);
    QString ur_name();
    bool setUr_Name(QString&);
    QString email();
    bool setEmail(QString&);
    QString icq();
    bool setIcq(QString&);
    QString skype();
    bool setSkype(QString&);
    QString viber();
    bool setViber(QString&);
    QString telegram();
    bool setTelegram(QString&);
    QString site();
    bool setSite(QString&);
    QString whatsapp();
    bool setWhatsapp(QString&);
    QString agentName();
    bool setAgentName(QString&);
    QString agentSurname();
    bool setAgentSurname(QString&);
    QString agentPatronymic();
    bool setAgentPatronymic(QString&);
    QString agentPhone();
    bool setAgentPhone(QString&);
    QString agentPhoneClean();
    bool setAgentPhoneClean(QString&);
    QString agent2Name();
    bool setAgent2Name(QString&);
    QString agent2Surname();
    bool setAgent2Surname(QString&);
    QString agent2Patronymic();
    bool setAgent2Patronymic(QString&);
    QString agent2Phone();
    bool setAgent2Phone(QString&);
    QString agent2PhoneClean();
    bool setAgent2PhoneClean(QString&);
    float balance();
    void createBalanceObj();
    bool updateBalance(const float amount, const QString &text);
    bool updateBalance(const float amount, const QString &text, const int doc_id);
    bool balanceEnough(const QString summ);
    bool receiptBalance();
    bool paymentBalance();
    int priceColumn();
    bool setPriceColumn(int);
    int priceColumnIndex();
    int repairs();
    bool setRepairs(int);
    int purchases();
    bool setPurchases(int);
    bool updatePurchases(int);
    QString token();
    bool setToken(QString&);
    void showNotification();
    QString genWebPass();
    void appendLogText(const QString&);
    bool commit();
    bool integrityStatus();

private:
    bool nIntegrityErr = 1;
    bool m_standAlone = 0;
    int m_editStrategy = OnManualSubmit;
    SSqlQueryModel *clientModel;
    SPhonesModel *m_phones;
    QMap<QString, QVariant> updateBuffer;
    int     m_id = 0;   // ID клиента
    QString m_firstName;
    QString m_lastName;
    QString m_patronymicName;
    QString m_address;
    QString m_postIndex;
    QString m_passportNum;
    QString m_passportIssuedDate;
    QString m_passportIssuedBy;
    bool    m_state = 1;
    bool    m_type = 0;
    QString m_birthday;
    QString m_memorial;
    QString m_notes;
    int     m_options = 0;
    QByteArray* m_photo;
    int     m_photo_id = 0;
    int     m_visitSource = 0;
    QString m_INN;
    QString m_KPP;
    QString m_OGRN;
    QString m_web_password;
    QString m_ur_name;
    QString m_email;
    QString m_icq;
    QString m_skype;
    QString m_viber;
    QString m_telegram;
    QString m_site;
    QString m_whatsapp;
    QString m_agentName;
    QString m_agentSurname;
    QString m_agentPatronymic;
    QString m_agentPhone;
    QString m_agentPhoneClean;
    QString m_agent2Name;
    QString m_agent2Surname;
    QString m_agent2Patronymic;
    QString m_agent2Phone;
    QString m_agent2PhoneClean;
    float   m_balance = 0;
    int     m_priceColumn = 2;
    int     m_repairs = 0;
    int     m_purchases = 0;
    QString m_token;
    SBalanceLogRecordModel *balanceLog = nullptr;
};

#endif // SCLIENTMODEL_H

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
#include "sphonesmodel.h"

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
    bool isNew();
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
    void setPhonesModel(SPhonesModel*);
    bool addPhone(const QString &number, int numberMask = 0, int messangers = 0);
    bool removePhone(const QString&);
    QString address();
    void setAddress(const QString&);
    QString postIndex();
    void setPostIndex(const QString&);
    QString passportNum();
    void setPassportNum(const QString&);
    QString passportIssuedDate();
    void setPassportIssuedDate(const QString&);
    QString passportIssuedBy();
    void setPassportIssuedBy(const QString&);
    bool state();
    void setState(bool);
    bool type();
    void setType(const int);
    QString birthday();
    void setBirthday(const QString&);
    QString memorial();
    void setMemorial(const QString&);
    QString notes();
    void setNotes(const QString&);
    int options();
    void setOptions(int);
    int adType();
    void setAdType(const int);
    int adTypeIndex();
    QByteArray* photo();
    void setPhoto(QByteArray*);
    QString INN();
    void setINN(const QString&);
    QString KPP();
    void setKPP(const QString&);
    QString OGRN();
    void setOGRN(const QString&);
    QString web_password();
    void setWeb_Password(const QString&);
    QString ur_name();
    void setUr_Name(const QString&);
    QString email();
    void setEmail(const QString&);
    QString icq();
    void setIcq(const QString&);
    QString skype();
    void setSkype(const QString&);
    QString viber();
    void setViber(const QString&);
    QString telegram();
    void setTelegram(const QString&);
    QString site();
    void setSite(const QString&);
    QString whatsapp();
    void setWhatsapp(const QString&);
    QString agentName();
    void setAgentName(const QString&);
    QString agentSurname();
    void setAgentSurname(const QString&);
    QString agentPatronymic();
    void setAgentPatronymic(const QString&);
    QString agentPhone();
    void setAgentPhone(const QString&);
    QString agentPhoneClean();
    void setAgentPhoneClean(const QString&);
    QString agent2Name();
    void setAgent2Name(const QString&);
    QString agent2Surname();
    void setAgent2Surname(const QString&);
    QString agent2Patronymic();
    void setAgent2Patronymic(const QString&);
    QString agent2Phone();
    void setAgent2Phone(const QString&);
    QString agent2PhoneClean();
    void setAgent2PhoneClean(const QString&);
    float balance();
    void createBalanceObj();
    bool updateBalance(const float amount, const QString &text);
    bool updateBalance(const float amount, const QString &text, const int doc_id);
    bool balanceEnough(const QString summ);
    bool receiptBalance();
    bool paymentBalance();
    int priceColumn();
    void setPriceColumn(int);
    int priceColumnIndex();
    int repairs();
    void setRepairs(int);
    bool updateRepairs();
    int purchases();
    void setPurchases(int);
    bool updatePurchases(int);
    QString token();
    void setToken(const QString&);
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

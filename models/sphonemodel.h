#ifndef SPHONEMODEL_H
#define SPHONEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "global.h"
#include "com_sql_queries.h"
#include "scomrecord.h"
#include "slogrecordmodel.h"

class SPhoneModel : public SComRecord
{
    Q_OBJECT
signals:
    void markedPrimary(SPhoneModel*);
    void typeChanged(int type);

public:
    enum Types {Additional = 0, Primary = 1};
    enum Messengers {Viber = 1, Telegram = 2, Whatsapp = 4};
    explicit SPhoneModel(QObject *parent = nullptr);
    explicit SPhoneModel(const QSqlRecord &record, QObject *parent = nullptr);
    ~SPhoneModel();
    bool commit();
    void setClient(const int id);
    QString phone();
    void setPhone(const QString&);
    int mask();
    void setMask(const int index);
    int maskIndex();
    int type();
    QString note();
    void setNote(const QString&);
    int messengers();
    void setMessengers(const int opt);
    bool isEmpty();
    void setPrimary(int primary = Primary);
private:
    int m_id = 0;
    int m_client_id = 0;
    QString m_phone;
    QString m_phoneClean;
    QString m_note;
    int m_type = Additional;
    int m_mask;
    int m_messengers;
    const QSqlRecord* m_record = nullptr;
    QString cleanPhone(const QString &);
    void setType(const int type);

public slots:
    void setPrimaryUi();
};

#endif // SPHONEMODEL_H

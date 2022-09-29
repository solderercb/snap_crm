#ifndef SPAYMENTTYPEMODEL_H
#define SPAYMENTTYPEMODEL_H

#include "scomrecord.h"
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QSqlQuery>

class SPaymentTypeModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SPaymentTypeModel(QObject *parent = nullptr);
    ~SPaymentTypeModel();
    int id();
    void setId(const int);
    void load();
    void load(const int);
    int type();
    void setType(const int);
    QString name();
    void setName(const QString&);
    int client();
    void setClient(const int);
    bool periodic();
    void setPeriodic(const bool);
    QDateTime payDate();
    void setPayDate(const QDateTime);
    float defSumm();
    void setDefSumm(const float);
    QString reason();
    void setReason(const QString&);
    bool isArchive();
    void markArchive(const bool);
    int paymentSystem();
    int paymentSystemIndex();
    void setPaymentSystem(const int);
    bool commit();
private:
    int i_id;
    int m_type;
    QString m_name;
    int m_client;
    bool m_periodic;
    QDateTime m_payDate;
    QDateTime m_updateTime;
    float m_defSumm;
    QString m_reason;
    bool m_isArchive;
    int m_paymentSystem;
    int m_paymentSystemIndex;
};

#endif // SPAYMENTTYPEMODEL_H

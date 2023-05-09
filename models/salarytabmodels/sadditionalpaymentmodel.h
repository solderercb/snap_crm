#ifndef SADDITIONALPAYMENTMODEL_H
#define SADDITIONALPAYMENTMODEL_H

#include "../scomrecord.h"
#include <QObject>

class SAdditionalPaymentModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SAdditionalPaymentModel(QObject *parent = nullptr);
    int id();
    QString name();
    void setName(const QString);
    QDateTime paymentDate();
    void setPaymentDate(const QDateTime);
    int user();
    void setUser(const int);
    int employee();
    void setEmployee(const int);
    double summ();
    void setSumm(const double);
    bool commit();
private:
    QString m_name;
    QDateTime m_paymentDate;
    int m_user;
    int m_employee;
    double m_summ;
};

#endif // SADDITIONALPAYMENTMODEL_H

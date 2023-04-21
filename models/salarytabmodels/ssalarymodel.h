#ifndef SSALARYMODEL_H
#define SSALARYMODEL_H

#include "../sdatabaserecord.h"
#include <QObject>

class SSalaryModel : public SDatabaseRecord
{
    Q_OBJECT
public:
    explicit SSalaryModel(QObject *parent = nullptr);
    void load(const int id);
    void load(const QSqlRecord &record);
    int id();
    int userId();
    void setUserId(const int);
    double summ();
    void setSumm(const double);
    double balance();
    void setBalance(const double);
    QString paymentDate();
    void setPaymentDate(const QDateTime);
    QString notes();
    void setNotes(const QString&);
    QDate periodFrom();
    void setPeriodFrom(const QDate);
    QDate periodTo();
    void setPeriodTo(const QDate);
    int fromUser();
    void setFromUser(const int);
    bool type();
    void setType(const bool);
private:
    int m_userId;
    double m_summ;
    double m_balance;
    QDateTime m_paymentDate;
    QString m_notes;
    QDate m_periodFrom;
    QDate m_periodTo;
    int m_fromUser;
    bool m_type;
};

#endif // SSALARYMODEL_H

#ifndef SSALARYMODEL_H
#define SSALARYMODEL_H

#include "../sdatabaserecord.h"
#include <QObject>

class SSalaryModel : public SDatabaseRecord
{
    Q_OBJECT
public:
    enum Type{Salary = 0, Subsistence = 1};
    explicit SSalaryModel(QObject *parent = nullptr);
    void load(const int id);
    void load(const QSqlRecord &record);
    int id();
    int employee();
    void setEmployee(const int);
    double summ();
    void setSumm(const double);
    double balance();
    void setBalance(const double);
    QString paymentDate();
    void setPaymentDate(const QDateTime);
    QString notes();
    void setNotes(const QString&);
    QDate periodFrom();
    void setPeriodFrom(const QDateTime);
    QDate periodTo();
    void setPeriodTo(const QDateTime);
    int user();
    void setUser(const int);
    bool type();
    void setType(const bool);
    bool commit();
    void setBalanceRecord(const int id);
private:
    int m_employee;
    double m_summ;
    double m_balance;
    QDateTime m_paymentDate;
    QString m_notes;
    QDate m_periodFrom;
    QDate m_periodTo;
    int m_user;
    bool m_type;
};

#endif // SSALARYMODEL_H

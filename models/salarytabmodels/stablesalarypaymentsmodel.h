#ifndef STABLESALARYPAYMENTSMODEL_H
#define STABLESALARYPAYMENTSMODEL_H

#include <STableModelBase>

class STableSalaryPaymentsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum PaymentType {Salary = 0, Subsistance = 1};
    enum Columns {Id = 0, PaymentDate = 1, PeriodFrom = 2, PeriodTo = 3, CashAmount = 4, BalanceAmount = 5, Employee = 6, Issuer = 7, Notes = 8, Type = 9};
    explicit STableSalaryPaymentsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    QString balanceChangeValue(const QModelIndex &item) const;
    double total(const PaymentType type);
};

#endif // STABLESALARYPAYMENTSMODEL_H

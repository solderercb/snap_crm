#ifndef STABLESALARYPAYMENTSMODEL_H
#define STABLESALARYPAYMENTSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryPaymentsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum PaymentType {Salary = 0, Subsistance = 1};
    explicit STableSalaryPaymentsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    double total(const PaymentType type);
};

#endif // STABLESALARYPAYMENTSMODEL_H

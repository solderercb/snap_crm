#ifndef STABLESALARYPAYMENTSMODEL_H
#define STABLESALARYPAYMENTSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryPaymentsModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryPaymentsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYPAYMENTSMODEL_H

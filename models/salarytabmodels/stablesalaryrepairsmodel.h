#ifndef STABLESALARYREPAIRSMODEL_H
#define STABLESALARYREPAIRSMODEL_H

#include "../stablebasemodel.h"

class STableSalaryRepairsModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryRepairsModel(QObject *parent = nullptr);
    double notIssuedTotal(int column);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRSMODEL_H

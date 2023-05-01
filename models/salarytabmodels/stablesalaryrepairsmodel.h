#ifndef STABLESALARYREPAIRSMODEL_H
#define STABLESALARYREPAIRSMODEL_H

#include "../stablebasemodel.h"

class STableSalaryRepairsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum IncludePayedInTotal {IncludePayed = 0, ExcludePayed = 1};
    explicit STableSalaryRepairsModel(QObject *parent = nullptr);
    double total(int column, bool excludePayed = ExcludePayed);
    double notIssuedTotal(int column);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRSMODEL_H

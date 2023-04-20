#ifndef STABLESALARYREPAIRWORKSMODEL_H
#define STABLESALARYREPAIRWORKSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryRepairWorksModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryRepairWorksModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRWORKSMODEL_H

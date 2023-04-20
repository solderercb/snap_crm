#ifndef STABLESALARYREPAIRPARTSMODEL_H
#define STABLESALARYREPAIRPARTSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryRepairPartsModel : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryRepairPartsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRPARTSMODEL_H

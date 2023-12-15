#ifndef STABLESALARYREPAIRPARTSMODEL_H
#define STABLESALARYREPAIRPARTSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryRepairPartsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {UID = 0, Name = 1, Count = 2, Price = 3, Amount = 4, SN = 5, Warranty = 6, Profit = 7, SalaryPart = 8, ItemId = 9, IsRealization = 10};
    explicit STableSalaryRepairPartsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRPARTSMODEL_H

#ifndef STABLESALARYREPAIRWORKSMODEL_H
#define STABLESALARYREPAIRWORKSMODEL_H

#include <STableModelBase>

class STableSalaryRepairWorksModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {PriceId = 0, Name = 1, Count = 2, Price = 3, Amount = 4, Warranty = 5, Percent = 6, SalaryPart = 7};
    explicit STableSalaryRepairWorksModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYREPAIRWORKSMODEL_H

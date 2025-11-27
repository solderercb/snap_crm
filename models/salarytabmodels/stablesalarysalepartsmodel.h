#ifndef STABLESALARYSALEPARTSMODEL_H
#define STABLESALARYSALEPARTSMODEL_H

#include <STableModelBase>

class STableSalarySalePartsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {UID = 0, Name = 1, Count = 2, Price = 3, Amount = 4, SN = 5, Warranty = 6, Profit = 7, SalaryPart = 8, ItemId = 9};
    explicit STableSalarySalePartsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLESALARYSALEPARTSMODEL_H

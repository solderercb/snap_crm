#ifndef STABLESALARYITEMSMODEL_H
#define STABLESALARYITEMSMODEL_H

#include <STableModelBase>

class STableSalaryItemsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Columns {UID = 0, Name = 1, Created = 2, Qty = 3, Price = 4, Amount = 5, State = 6, ItemId = 7};
    explicit STableSalaryItemsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    int totalUsedItemsByArticul();
    int totalUsedItemsPeaces();
    double totalUsedItemsCost();
    int totalUnusedItemsByArticul();
    int totalUnusedItemsPeaces();
    double totalUnusedItemsCost();

};

#endif // STABLESALARYITEMSMODEL_H

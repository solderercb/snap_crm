#ifndef STABLESALARYITEMSMODEL_H
#define STABLESALARYITEMSMODEL_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryItemsModel : public STableBaseModel
{
    Q_OBJECT
public:
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

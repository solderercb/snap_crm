#include "stablesalaryitemsmodel.h"
#include <ProjectGlobals>
#include <SRepairSaleItemModel>
#include <SStoreItemModel>
#include <SStandardItemModel>

STableSalaryItemsModel::STableSalaryItemsModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryItemsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(item.column())
        {
            case Columns::Created: return timestampLocal(item);
            case Columns::Price:
            case Columns::Amount: return dataLocalizedFromDouble(item);
            case Columns::State: return storeItemsIntReserveStatesModel->getDisplayRole(STableBaseModel::data(item).toInt());
            default: ;
        }
    }
    if(role == Qt::BackgroundRole)
    {
        if(STableBaseModel::data(item.siblingAtColumn(Columns::State)).toInt() == SRepairSaleItemModel::RepairLinked)
            return QColor(144,238,144);
    }

    return STableBaseModel::data(item, role);
}

int STableSalaryItemsModel::totalUsedItemsByArticul()
{
    int count = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::RepairLinked)
            count++;
    }
    return count;
}

int STableSalaryItemsModel::totalUsedItemsPeaces()
{
    int count = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::RepairLinked)
            count += STableBaseModel::data(index(i, Columns::Qty)).toInt();
    }
    return count;
}

double STableSalaryItemsModel::totalUsedItemsCost()
{
    double cost = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::RepairLinked)
            cost += STableBaseModel::data(index(i, Columns::Amount)).toDouble();
    }
    return cost;
}

int STableSalaryItemsModel::totalUnusedItemsByArticul()
{
    int count = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::EngineerBasket)
            count++;
    }
    return count;
}

int STableSalaryItemsModel::totalUnusedItemsPeaces()
{
    int count = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::EngineerBasket)
            count += STableBaseModel::data(index(i, Columns::Qty)).toInt();
    }
    return count;
}

double STableSalaryItemsModel::totalUnusedItemsCost()
{
    double cost = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::State)).toInt() == SRepairSaleItemModel::EngineerBasket)
            cost += STableBaseModel::data(index(i, Columns::Amount)).toDouble();
    }
    return cost;
}

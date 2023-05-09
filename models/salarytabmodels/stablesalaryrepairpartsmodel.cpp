#include "global.h"
#include "stablesalaryrepairpartsmodel.h"

STableSalaryRepairPartsModel::STableSalaryRepairPartsModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryRepairPartsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 3:
            case 4:
            case 7:
            case 8: return dataLocalizedFromDouble(item);
            case 6: return warrantyFromId(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

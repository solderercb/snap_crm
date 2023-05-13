#include "global.h"
#include "stablesalaryrepairworksmodel.h"

STableSalaryRepairWorksModel::STableSalaryRepairWorksModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryRepairWorksModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 3:
            case 4:
            case 7: return dataLocalizedFromDouble(item);
            case 5: return warrantyFromId(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

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
            case Columns::Price:
            case Columns::Amount:
            case Columns::SalaryPart: return dataLocalizedFromDouble(item);
            case Columns::Warranty: return warrantyFromId(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

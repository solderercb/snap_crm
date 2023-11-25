#include "global.h"
#include "stablesalaryrepairpartsmodel.h"

STableSalaryRepairPartsModel::STableSalaryRepairPartsModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryRepairPartsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case Columns::Price:
            case Columns::Amount:
            case Columns::Profit:
            case Columns::SalaryPart: return dataLocalizedFromDouble(item);
            case Columns::Warranty: return warrantyFromId(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

#include <ProjectGlobals>
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
    if(role == Qt::BackgroundRole)
    {
        if(item.column() == Columns::UID && STableBaseModel::data(item.siblingAtColumn(Columns::IsRealization)).toBool())
            return QColor(100,149,237);
    }
    return STableBaseModel::data(item, role);
}

#include "stablesalarysalesmodel.h"

STableSalarySalesModel::STableSalarySalesModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalarySalesModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case Columns::Created: return timestampLocal(item);
            case Columns::Amount:
            case Columns::Profit:
            case Columns::SalaryPart: return dataLocalizedFromDouble(item);
            default: ;
        }
    }

    return STableBaseModel::data(item, role);
}

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
            case 1: return timestampLocal(item);
            case 3:
            case 4:
            case 5: return dataLocalizedFromDouble(item);
            default: ;
        }
    }

    return STableBaseModel::data(item, role);
}

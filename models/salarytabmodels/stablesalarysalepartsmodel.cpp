#include "global.h"
#include "stablesalarysalepartsmodel.h"

STableSalarySalePartsModel::STableSalarySalePartsModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalarySalePartsModel::data(const QModelIndex &item, int role) const
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

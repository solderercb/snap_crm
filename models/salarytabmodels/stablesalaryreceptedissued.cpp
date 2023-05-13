#include "stablesalaryreceptedissued.h"

STableSalaryReceptedIssued::STableSalaryReceptedIssued(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryReceptedIssued::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole) // ранее оплаченные сотруднику ремонты выделены зелёным
    {
        if(item.column() == 2)
            return dataLocalizedFromDouble(item);
    }

    return STableBaseModel::data(item, role);
}

#include "global.h"
#include "stablesalarypaymentsmodel.h"

STableSalaryPaymentsModel::STableSalaryPaymentsModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryPaymentsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 1: return timestampLocal(item);
            case 2: return dataLocalizedFromDouble(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

double STableSalaryPaymentsModel::total(const PaymentType type)
{
    double total = 0;
    for(int i=0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, 6)).toInt() == type)
            total += STableBaseModel::data(index(i, 2)).toDouble();
    }

    return total;
}

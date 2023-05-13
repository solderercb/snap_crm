#include "global.h"
#include "stablesalaryrepairsmodel.h"

STableSalaryRepairsModel::STableSalaryRepairsModel(QObject *parent) : STableBaseModel(parent)
{

}

double STableSalaryRepairsModel::total(int column, bool excludePayed)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(excludePayed == ExcludePayed && payedSumm(i)) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += unformattedData(index(i, column)).toDouble();
    }

    return total;
}

double STableSalaryRepairsModel::notIssuedTotal(int column)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        switch(index(i, 9).data().toInt())
        {
            case 8:     // TODO: заменить числа!
            case 12:
            case 16: continue;
            default: ;
        }

        if(payedSumm(i)) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += sysLocale.toDouble(index(i, column).data().toString());
    }

    return total;
}

/* Форматирование данных модели
 * Данный метод (и одноимённые в других классах) для штатных ролей, например DisplayRole, вызывают
 * методы класса STableModelsCommonMethods; те, в свою очередь, повторно вызывают этот метод, но роль
 * отличается значением в 8-м разряде (role | 0x0100), что приводит к вызову одноименного метода из базового
 * класса (STableBaseModel). Такое решение позволяет использовать класс STableModelsCommonMethods в т. ч. для
 * редактируемых моделей данных и при этом избежать проблемы ромбовидного наследования.
*/
QVariant STableSalaryRepairsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7: return dataLocalizedFromDouble(item);
            case 8: return timestampLocal(item);
            default: ;
        }
    }
    else if(role == Qt::BackgroundColorRole) // ранее оплаченные сотруднику ремонты выделены зелёным
    {
        if(index(item.row(), 10).data().toInt())
            return QColor(192,192,192);
    }

    return STableBaseModel::data(item, role);
}

int STableSalaryRepairsModel::id(const int row)
{
    return unformattedData(index(row, 0)).toInt();
}

int STableSalaryRepairsModel::payedSumm(const int row)
{
    return unformattedData(index(row, 10)).toInt();
}

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
        if(excludePayed == ExcludePayed && index(i, 10).data().toInt()) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += sysLocale.toDouble(index(i, column).data().toString());
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

        if(index(i, 10).data().toInt()) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += sysLocale.toDouble(index(i, column).data().toString());
    }

    return total;
}

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
        }

        return STableBaseModel::data(item, role);
    }
    else if(role == Qt::BackgroundColorRole) // ранее оплаченные сотруднику ремонты выделены зелёным
    {
        if(index(item.row(), 10).data().toInt())
            return QColor(192,192,192);
    }

    return STableBaseModel::data(item, role);
}

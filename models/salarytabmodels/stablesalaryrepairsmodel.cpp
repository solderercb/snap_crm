#include "global.h"
#include "stablesalaryrepairsmodel.h"

STableSalaryRepairsModel::STableSalaryRepairsModel(QObject *parent) : STableBaseModel(parent)
{

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
            case 7: return sysLocale.toString(STableBaseModel::data(item, role).toDouble(), 'f', comSettings->value("classic_kassa").toBool()?2:0);
        }

        return STableBaseModel::data(item, role);
    }
    return STableBaseModel::data(item, role);
}

#include "global.h"
#include "stablesalarypaymentsmodel.h"

STableSalaryPaymentsModel::STableSalaryPaymentsModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryPaymentsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 2: return sysLocale.toString(STableBaseModel::data(item, role).toDouble(), 'f', comSettings->value("classic_kassa").toBool()?2:0);
            default: return STableBaseModel::data(item, role);
        }
    }
    return STableBaseModel::data(item, role);
}

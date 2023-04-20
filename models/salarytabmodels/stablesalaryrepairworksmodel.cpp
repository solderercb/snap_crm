#include "global.h"
#include "stablesalaryrepairworksmodel.h"

STableSalaryRepairWorksModel::STableSalaryRepairWorksModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryRepairWorksModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 3:
            case 4: return sysLocale.toString(STableBaseModel::data(item).toDouble(), 'f', comSettings->value("classic_kassa").toBool()?2:0);
            case 5: return warrantyTermsModel->getDisplayRole(STableBaseModel::data(item).toInt(), 1);
            default: return STableBaseModel::data(item);
        }
    }
    return STableBaseModel::data(item, role);
}

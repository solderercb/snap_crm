#include "global.h"
#include "stablesalaryrepairpartsmodel.h"

STableSalaryRepairPartsModel::STableSalaryRepairPartsModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryRepairPartsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 3:
            case 4: return sysLocale.toString(STableBaseModel::data(item).toDouble(), 'f', comSettings->value("classic_kassa").toBool()?2:0);
            case 6: return warrantyTermsModel->getDisplayRole(STableBaseModel::data(item).toInt(), 1);
            default: return STableBaseModel::data(item);
        }
    }
    return STableBaseModel::data(item, role);
}

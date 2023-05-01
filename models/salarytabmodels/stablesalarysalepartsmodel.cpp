#include "global.h"
#include "stablesalarysalepartsmodel.h"

STableSalarySalePartsModel::STableSalarySalePartsModel(QObject *parent) : STableBaseModel(parent)
{

}

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
            case 6: return warrantyTermsModel->getDisplayRole(STableBaseModel::data(item).toInt(), 1);
            default: return STableBaseModel::data(item);
        }
    }
    return STableBaseModel::data(item, role);
}

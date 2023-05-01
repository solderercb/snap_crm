#include "stablesalarysalesmodel.h"

STableSalarySalesModel::STableSalarySalesModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalarySalesModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 1: return timestampLocal(item);
            case 3:
            case 4:
            case 5: return dataLocalizedFromDouble(item);
        }
    }

    return STableBaseModel::data(item, role);
}

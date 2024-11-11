#include "stableclientsmodel.h"

STableClientsModel::STableClientsModel(QObject *parent) :
    STableBaseModel{parent}
{

}

QVariant STableClientsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case Columns::FullName: return dataShort(index);
            default: ;
        }
    }

    return STableBaseModel::data(index, role);
}

QModelIndex STableClientsModel::indexForShortData(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Columns::FullName: return index.siblingAtColumn(Columns::ShortName);
    }

    return QModelIndex();
}

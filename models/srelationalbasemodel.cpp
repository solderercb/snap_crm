#include "srelationalbasemodel.h"

SRelationalBaseModel::SRelationalBaseModel(QObject *parent, QSqlDatabase db)
    : QSqlRelationalTableModel(parent, db),
    STableModelsCommonMethods(parent)
{
    m_sqlQueryModel = this;
}

//Qt::ItemFlags SRelationalBaseModel::flags(const QModelIndex &index) const
//{
//    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//}

QVariant SRelationalBaseModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::ToolTipRole)
    {
        return data(item, Qt::DisplayRole);
    }

    return QSqlRelationalTableModel::data(item, role & 0xFF);
}

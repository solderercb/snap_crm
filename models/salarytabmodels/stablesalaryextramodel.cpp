#include "global.h"
#include "stablesalaryextramodel.h"

STableSalaryExtraModel::STableSalaryExtraModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryExtraModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(item.column())
        {
            case 3: return timestampLocal(item);
            case 4: return usersModel->getDisplayRole(QSqlQueryModel::data(item, role).toInt());
            default: ;
        }
    }
//    if(role == Qt::BackgroundColorRole)
//    {
//    }

    return STableBaseModel::data(item, role);
}

#include "stabletechreportsmodel.h"

STableTechReportsModel::STableTechReportsModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableTechReportsModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case Columns::Company: return companyFromId(index);
            case Columns::User: return userFromId(index);
            case Columns::Created: return timestampLocal(index);
            default: break;
        }
    }
    return STableBaseModel::data(index, role);
}

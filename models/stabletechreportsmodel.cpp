#include "stabletechreportsmodel.h"
#include "stechreportmodel.h"

STableTechReportsModel::STableTechReportsModel(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableTechReportsModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case Columns::Created: return timestampLocal(index);
            case Columns::User: return userFromId(index);
            case Columns::Company: return companyFromId(index);
            case Columns::Client: return clientName(index);
            case Columns::ProductionDate: return deviceDate(index);
            case Columns::PurchaseDate: return deviceDate(index);
            case Columns::InitCost: return dataLocalizedFromDouble(index);
            case Columns::ResidualCost: return dataLocalizedFromDouble(index);
            default: break;
        }
    }
    return STableBaseModel::data(index, role);
}

QModelIndex STableTechReportsModel::indexForShortData(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Columns::Client: return index.siblingAtColumn(Columns::ClientShortName);
    }

    return QModelIndex();
}

QVariant STableTechReportsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions");

    return dataShort(idx);
}

QVariant STableTechReportsModel::deviceDate(const QModelIndex &idx) const
{
    QString date = unformattedData(idx).toString();
    return STechReportModel::dateToString(STechReportModel::yearFromString(date), STechReportModel::monthFromString(date), STechReportModel::dayFromString(date));
}

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

QVariant STableTechReportsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions");

    QString value;
    int width = i_columnWidths[idx.column()];

    // полное имя (название организации)
    value = STableBaseModel::data(idx).toString();
    if(value.length() < width)
        return value;

    // короткое имя
    value = STableBaseModel::data(idx.siblingAtColumn(Columns::ClientShortName)).toString();
    if(!value.isEmpty() && value.length() < width)
        return value;

    return STableBaseModel::data(idx);
}

QVariant STableTechReportsModel::deviceDate(const QModelIndex &idx) const
{
    QString date = unformattedData(idx).toString();
    return STechReportModel::dateToString(STechReportModel::yearFromString(date), STechReportModel::monthFromString(date), STechReportModel::dayFromString(date));
}

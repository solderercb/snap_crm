#include "srequestsmodel.h"
#include "modules/purchasemanager/srequeststable.h"

SPartsRequestsModel::SPartsRequestsModel(QObject *parent, QSqlDatabase db)
    : SEditableBaseModel{parent, db}
{
}

SPartsRequestsModel::~SPartsRequestsModel()
{

}

int SPartsRequestsModel::checkBoxState(const QModelIndex &index) const
{
    if(m_itemsToInvoice.contains(index))
        return 2;

    return 0;
}
QVariant SPartsRequestsModel::data(const QModelIndex &item, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case 0: return QVariant();
            case Columns::Employee: return userFromId(item);
            case Columns::State: return partRequestStatesListModel->value(unformattedData(item).toInt());
            case Columns::Priority: return partRequestPrioritiesListModel->value(unformattedData(item).toInt());
            case Columns::Client: return clientName(item);
            case Columns::Created: return dateTime(item);
            case Columns::Progress: return unformattedData(item.siblingAtColumn(Columns::State));
            default: ;
        }
    }
    if(role == Qt::CheckStateRole)
    {
        switch (item.column())
        {
            case 0: return (Qt::CheckState)checkBoxState(item);
            default: break;
        }
    }

    return SEditableBaseModel::data(item, role);
}

bool SPartsRequestsModel::toggleCheckBox(const QModelIndex &index)
{
    if(m_itemsToInvoice.contains(index))
    {
        m_itemsToInvoice.removeOne(index);
    }
    else
    {
        m_itemsToInvoice.append(index);
    }

    return true;
}

bool SPartsRequestsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        switch (index.column())
        {
            case 0: return toggleCheckBox(index);
            case Columns::State: emit stateChanged((SPartRequest::State)value.toInt()); break;
            default: break;
        }
    }

    return SEditableBaseModel::setData(index, value, role);
}

Qt::ItemFlags SPartsRequestsModel::flags(const QModelIndex &item) const
{
    switch (item.column())
    {
        case Check: return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
        case Name:
        case Count:
        case State:
        case Tracking:
        case Priority: return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled)^flagsMask(item);
        default: break;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Qt::ItemFlags SPartsRequestsModel::flagsMask(const QModelIndex &item) const
{
    // Завершенные и отменённые нельзя редактировать
    switch(unformattedData(item.siblingAtColumn(State)).toInt())
    {
        case SPartRequest::State::Finished:
        case SPartRequest::State::Cancelled: return Qt::ItemIsEditable;
        default: return Qt::ItemFlags();
    }
}

bool SPartsRequestsModel::removeRow(const int row, const QModelIndex &)
{
    if(removeRows(row, 1)) // сначала удаление, потом установка значения -1
    {
        setData(index(row, STableViewPartsRequests::Column::State), -1, Qt::EditRole);
        return 1;
    }
    return 0;
}

void SPartsRequestsModel::clear()
{
    m_itemsToInvoice.clear();
    SEditableBaseModel::clear();
}

void SPartsRequestsModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    setSelectStatement(query);
    QSqlQueryModel::setQuery(query, db);

    select();
}

bool SPartsRequestsModel::select()
{
    return SEditableBaseModel::select();
    return 1;
}

bool SPartsRequestsModel::selectRow(int row)
{
    return SEditableBaseModel::select();
}

void SPartsRequestsModel::translateNames()
{
    tr("Employee");
    tr("Repair");
    tr("Client");
    tr("State");
    tr("EndDate");
    tr("PlanEndDate");
    tr("Summ");
    tr("Tracking");
    tr("ItemId");
    tr("Name");
    tr("Notes");
    tr("Url");
    tr("Priority");
    tr("Count");
    tr("Dealer");
}

/*
    TODO: аналогичный код используется в STableTechReportsModel и STableRepairsModel; нужно сделать общий
*/
QVariant SPartsRequestsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions");

    QString value;
    int width = i_columnWidths[idx.column()];

    // полное имя (название организации)
    value = SEditableBaseModel::data(idx).toString();
    if(value.length() < width)
        return value;

    // короткое имя
    value = SEditableBaseModel::data(idx.siblingAtColumn(Columns::ClientShortName)).toString();
    if(!value.isEmpty() && value.length() < width)
        return value;

    return SEditableBaseModel::data(idx);
}

QVariant SPartsRequestsModel::dateTime(const QModelIndex &idx) const
{
    int width = i_columnWidths[idx.column()];

    if(width > 16)
        return timestampLocal(idx).toString("dd.MM.yyyy hh:mm");
    else if(width > 10)
        return timestampLocal(idx).toString("dd.MM.yyyy");
    else
        return timestampLocal(idx).toString("dd.MM.yy");
}

bool SPartsRequestsModel::updateRowInTable(int row, const QSqlRecord &values)
{
    QSqlRecord whereValues;
    QSqlField whereField = record(row).field(Columns::Id);
    whereField.setGenerated(true);
    whereValues.append(whereField);
    QString query = QSqlDatabase::database("connThird").driver()->sqlStatement(QSqlDriver::UpdateStatement, "parts_request", values, 0) + " " +
                    QSqlDatabase::database("connThird").driver()->sqlStatement(QSqlDriver::WhereStatement, "parts_request", whereValues, 0);

    if(QSqlDatabase::database("connThird").exec(query).lastError().isValid())
    {
        return 0;
    }

    return 1;
}

bool SPartsRequestsModel::commit()
{

    if(!submitAll())
    {
        throw Global::ThrowType::QueryError;
    }

    return 1;
}

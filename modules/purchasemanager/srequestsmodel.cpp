#include "srequestsmodel.h"
#include <QSqlDriver>
#include <QSqlField>
#include <SPartsRequestsView>
#include <SPermissions>
#include <SStandardItemModel>
#include <SLogRecordModel>

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
    if(m_modelRO)
        return Qt::ItemIsEditable;
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

void SPartsRequestsModel::setReadOnly(bool state)
{
    m_modelRO = state;
}

bool SPartsRequestsModel::select()
{
    if(m_postSubmitAction == PostSubmitAction::NoSelect)
    {
        m_postSubmitAction = PostSubmitAction::DefaultSelect;
        return true;
    }

    return SEditableBaseModel::select();
}

bool SPartsRequestsModel::selectRow(int)
{
    return SEditableBaseModel::select();
}

QModelIndex SPartsRequestsModel::indexForShortData(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Columns::Client: return index.siblingAtColumn(Columns::ClientShortName);
    }

    return QModelIndex();
}

QVariant SPartsRequestsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions"); // TODO: заменить "no permissions" на "Имя Ф." (как при переводе денег на карту)

    return dataShort(idx);
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

    SLogRecordModel logRecord;
    logRecord.set_type(SLogRecordModel::RecordTypes::PartRequest);
    logRecord.set_partRequest(whereField.value().toInt());

    QString logText;
    QMap<QString, QString> logTexts;
    for(int i = 0; i < values.count(); i++)
    {
        if(!values.isGenerated(i))
            continue;

        if(values.fieldName(i).compare("state") == 0)
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgStateChange(values.value(i).toInt()));
        if(values.fieldName(i).compare("plan_end_date") == 0)
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgPlanEndDateChange(values.value(i).toDate()));
        if(values.fieldName(i).compare("summ") == 0)
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgAmountChange(values.value(i).toDouble()));
        if(values.fieldName(i).compare("tracking") == 0)
        {
            QModelIndex indx = QSqlQueryModel::index(row,Columns::Tracking);
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgTrackingChange(values.value(i).toString(), QSqlQueryModel::data(indx).toString()));
        }
        if(values.fieldName(i).compare("pririty") == 0)
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgPriorityChange(values.value(i).toInt()));
        if(values.fieldName(i).compare("count") == 0)
            logTexts.insert(QString::number(logTexts.size()), SPartRequest::logMsgCountChange(values.value(i).toInt()));
    }
    foreach(logText, logTexts)
    {
        logRecord.set_text(logText);
        logRecord.commit();
    }

    return 1;
}

bool SPartsRequestsModel::submitAll()
{
    m_postSubmitAction = PostSubmitAction::NoSelect;    // нужно пропустить вызов метода select() из родительского класса и вызвать его самостоятельно по окончании (для сохранения положения и выделенной строки)

    SSingleRowModel::checkSystemTime();

    if(!SEditableBaseModel::submitAll())
    {
        Global::throwError(lastError(), tr("Не удалось сохранить список заявок на закупку | %1").arg(lastError().text()));
    }

    return 1;
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

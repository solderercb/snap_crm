#include "ssuppliersmodel.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SPartRequest>
#include <SPartsRequestsGroupingModel>

SPartSuppliersModel::SPartSuppliersModel(QObject *parent, QSqlDatabase db) :
    SRelationalBaseModel{parent, db},
    m_requestState(SPartRequest::Created)
{
    setTable("parts_request_suppliers");
    setRelation(SPartSuppliersModel::Columns::Supplier, QSqlRelation("clients", "id", "ur_name"));
    setJoinMode(QSqlRelationalTableModel::LeftJoin);
    connect(this, &SPartSuppliersModel::primeInsert, this, &SPartSuppliersModel::slotPrimeInsert);
}

SPartSuppliersModel::~SPartSuppliersModel()
{

}

QVariant SPartSuppliersModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case Columns::Supplier: return supplierName(index);
            case Columns::ItemUrl:
            case Columns::SupplierUrl: return url(index);
            case Columns::Select: return QVariant();
            default: break;
        }
    }
    if(role == Qt::CheckStateRole && index.column() == Columns::Select)
    {
        bool state = 0;
        if(m_newCheckedId)
            state = (index.siblingAtColumn(Columns::Id).data().toInt() == m_newCheckedId);
        else
            state = unformattedData(index).toBool();
        return state?Qt::Checked:Qt::Unchecked;
    }
    if(role == Qt::BackgroundRole)
    {
        if (m_highlightRows)
        {
            if(index.column() == 0)
            {
                if( index.siblingAtColumn(Columns::Supplier).data().toString().compare(m_highlightName) == 0 ||
                        SPartsRequestsGroupingModel::urlFormat(index.siblingAtColumn(Columns::SupplierUrl).data().toString()).compare(m_highlightName) == 0 )
                    m_highlightedRow = index.row();
            }

            if(m_highlightedRow >= 0 && index.row() == m_highlightedRow)
            {
                return QColor(190,255,190);
            }
        }
    }

    return SRelationalBaseModel::data(index, role);
}

Qt::ItemFlags SPartSuppliersModel::flags(const QModelIndex &index) const
{
    int mask = 0xFFFF;
    switch(m_requestState)
    {
        case SPartRequest::State::Cancelled:
        case SPartRequest::State::Finished: mask = ~(Qt::ItemIsEditable); break;
        default: break;
    }

    switch (index.column())
    {
        case Columns::RequestId: return (Qt::ItemIsSelectable | (m_itemsEditableFlagOverride?Qt::ItemIsEditable:Qt::ItemFlags()) | Qt::ItemIsEnabled) & mask;
        case Columns::SupplierUrl:
        case Columns::ItemUrl:
        case Columns::Moq:
        case Columns::Price:
        case Columns::Notes: return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled) & mask;
        case Columns::Select: return (Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled) & mask;
        default: break;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool SPartSuppliersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!(index.flags() & Qt::ItemIsEditable))
        return 0;

    if(role == Qt::CheckStateRole && value == Qt::Checked)
    {
        switch (index.column())
        {
            case Columns::Select: return setCheckStateExclusive(index);
            default: break;
        }
    }

    return SRelationalBaseModel::setData(index, value, role);
}

bool SPartSuppliersModel::appendRow()
{
    return insertRow(rowCount());
}

void SPartSuppliersModel::updateFilter(const int id)
{
    if(id)
        setFilter(QString("`request_id` = %1").arg(id));
    else
        setFilter("0");

    m_highlightedRow = -1;
}

bool SPartSuppliersModel::select()
{
    if(m_postSubmitAction == PostSubmitAction::SelectById)
    {
        m_newCheckedId = 0;
        m_postSubmitAction = PostSubmitAction::DefaultSelect;
        updateFilter(m_requestId);
        return true;
    }
    else if(m_postSubmitAction == PostSubmitAction::NoSelect)
    {
        m_postSubmitAction = PostSubmitAction::DefaultSelect;
        return true;
    }

    return QSqlRelationalTableModel::select();
}

bool SPartSuppliersModel::select(const int requestId)
{
    bool ret = 1;
    m_requestId = requestId;
    m_newCheckedId = 0;
    updateFilter(m_requestId);
    if(!query().isActive()) // select() вызывается в QSqlTableModel::setFilter() если isActive() возвращает true, второй вызов не нужен
        ret = select();
    return ret;
}

int SPartSuppliersModel::requestId() const
{
    return m_requestId;
}

/* Установка id созданного запроса
 * Т. к. столбец с id не предназначен для редактирования пользователем, используется
 * специальный флаг, переопределяющий значение возвращаемое методом flags().
*/
void SPartSuppliersModel::setRequestId(int id)
{
    m_requestId = id;
    m_itemsEditableFlagOverride = true;
    for(int i = 0; i < rowCount(); i++)
    {
        setData(index(i, Columns::RequestId), id);
    }
    m_itemsEditableFlagOverride = false;

}

void SPartSuppliersModel::setRequestState(int state)
{
    m_requestState = state;
}

/* определение действия по окончании выполнения запроса
*/
bool SPartSuppliersModel::postSubmitDispatcher(const bool &queryResult)
{
    m_postSubmitAction = queryResult?PostSubmitAction::SelectById:PostSubmitAction::NoSelect;
    return queryResult;
}

bool SPartSuppliersModel::updateRowInTable(int row, const QSqlRecord &values)
{
    return postSubmitDispatcher(SRelationalBaseModel::updateRowInTable(row, values));
}

bool SPartSuppliersModel::insertRowIntoTable(const QSqlRecord &values)
{
    return postSubmitDispatcher(QSqlRelationalTableModel::insertRowIntoTable(values));
}

bool SPartSuppliersModel::deleteRowFromTable(int row)
{
    return postSubmitDispatcher(SRelationalBaseModel::deleteRowFromTable(row));
}

bool SPartSuppliersModel::isDirty() const
{
    return SRelationalBaseModel::isDirty() | (m_newCheckedId > 0);
}

bool SPartSuppliersModel::submit()
{
    return SRelationalBaseModel::submit();
}

void SPartSuppliersModel::submitAll()
{
    if(m_requestId)
        m_postSubmitAction = PostSubmitAction::NoSelect;

    SSingleRowModel::checkSystemTime();

    if(m_newCheckedId)
    {
        QString q;
        q = QUERY_UPD_PRT_RQST_SUPPLIERS_SEL(m_newCheckedId, m_requestId);
        QSqlQuery query(this->database());
        QUERY_EXEC_TH(&query,1,q);
    }

    if(!SRelationalBaseModel::submitAll())
        Global::throwError(lastError(), tr("Не удалось сохранить список ссылок к заявке на закупку | %1").arg(lastError().text()));

    if(m_newCheckedId)
    {
        QModelIndexList il = match(index(0, 0), Qt::DisplayRole, m_newCheckedId);
        emit dataChanged(il.first(), il.first().siblingAtColumn(Supplier));
    }

    m_newCheckedId = 0;
}

int SPartSuppliersModel::predefSupplierId()
{
    return m_predefSupplierId;
}

void SPartSuppliersModel::setPredefSupplierId(const int id)
{
    m_predefSupplierId = id;
}

/* Добавление указанного поставщика в список.
 * См. описание метода SPartSuppliers::addSupplierRecord(const int id)
*/
void SPartSuppliersModel::addSupplierRecord(int id)
{
    setPredefSupplierId(id);
    for(int i = 0; i < rowCount(); i++)
    {
        if(unformattedData(index(i, Columns::Supplier)).toInt() == id)
        {
            return;
        }
    }

    appendRow();
}

void SPartSuppliersModel::setRowHighlightingClause(const int id, const QString &name)
{
    m_highlightRows = !(id <= 0 && name.isEmpty());
    if(!m_highlightRows)
    {
        m_highlightedRow = -1;
    }

    m_highlightId = id;
    m_highlightName = name;
}

QVariant SPartSuppliersModel::supplierName(const QModelIndex &index) const
{
    QVariant name = unformattedData(index);

    if(!name.isValid() || (name.type() == QVariant::Int && name.toInt() == 0))
        return QVariant();

    return name;
}

QString SPartSuppliersModel::url(const QModelIndex &index) const
{
    const QString data = unformattedData(index).toString();

//    if(data.isEmpty() || !data.startsWith("http"))
        return data;

        return QString("<a href='%1'>%1</a>").arg(data);
}

/* Установка значения в указанном индексе равным 1 и 0 во всех остальных строках
*/
bool SPartSuppliersModel::setCheckStateExclusive(const QModelIndex &checkedIndex)
{
    bool ret = 1;

    m_newCheckedId = checkedIndex.siblingAtColumn(Columns::Id).data().toInt();
    for(int i = 0; i < rowCount(); i++)
    {
        QModelIndex idx = index(i, Columns::Select);
        int id = index(i, Columns::Id).data().toInt();

        if(id)
            emit QSqlTableModel::dataChanged(idx, idx);
        else
            ret &= setData(idx, (i == checkedIndex.row())?Qt::Checked:Qt::Unchecked, Qt::EditRole);
    }

    return ret;
}

void SPartSuppliersModel::slotPrimeInsert(int row, QSqlRecord &record)
{
    Q_UNUSED(row);
    setRecordValue(record, Columns::RequestId, m_requestId);
    setRecordValue(record, Columns::ItemUrl, "");
    setRecordValue(record, Columns::Moq, 1);
    setRecordValue(record, Columns::Select, 0);
    if(m_predefSupplierId)
        setRecordValue(record, Columns::Supplier, m_predefSupplierId);
}

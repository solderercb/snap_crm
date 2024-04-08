#include "ssqlfetchingmodel.h"
#include "qsqldriver.h"

SSqlFetchingModel::SSqlFetchingModel(QObject *parent) : QSqlQueryModel(parent)
{
}

SSqlFetchingModel::~SSqlFetchingModel()
{
}

void SSqlFetchingModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_atEnd = 0;
    m_query = query;

    if(!m_db.isValid() || db.connectionName().compare(m_db.connectionName()) != 0)
    {
        m_db = db;
    }


    beginResetModel();
    fetchMore(QModelIndex());
    endResetModel();
}

int SSqlFetchingModel::queryPointer(const int row) const
{
    if(m_sqlQueries.isEmpty())
       return -1;

    int key;
    for (auto i = m_sqlQueries.cbegin(), end = m_sqlQueries.cend(); i != end; ++i)
    {
        key = i.key();
        if( row >= key && row < (key + i.value()->size()) )
        return key;
    }

    return -1;
}

/* копипаста из QSqlQueryModel */
QModelIndex SSqlFetchingModel::indexInQuery(const QModelIndex &item) const
{
    int modelColumn = item.column();

    if (modelColumn < 0 || modelColumn >= m_rec.count() || !m_rec.isGenerated(modelColumn) /*|| modelColumn >= colOffsets.size()*/)
        return QModelIndex();

    if (modelColumn < 0)
        return QModelIndex();
    return createIndex(item.row(), modelColumn, item.internalPointer());
}

QVariant SSqlFetchingModel::data(const QModelIndex &item, int role) const
{
    if (!item.isValid())
        return QVariant();

    if ((role&0xFF) & ~(Qt::DisplayRole | Qt::EditRole))
        return QVariant();

    int modelRow = item.row();
    int key = 0;
    key = queryPointer(modelRow);
    if(key < 0)
        return QVariant();

    QSqlQuery *query = m_sqlQueries.value(key);

    QModelIndex dItem = indexInQuery(item);

    if (!query->seek(dItem.row() - key))
    {
        return QVariant();
    }

    return query->value(dItem.column());
}

QModelIndex SSqlFetchingModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex i;
    if(hasIndex(row, column, parent))
    {
        i = createIndex(row, column);
    }

    return i;
}

QModelIndex SSqlFetchingModel::sibling(int row, int column, const QModelIndex &idx) const
{
    return QAbstractItemModel::sibling(row, column, idx);
}

bool SSqlFetchingModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return false;
    return row < rowCount(parent) && column < columnCount(parent);
}

int SSqlFetchingModel::rowCount(const QModelIndex&) const
{
    return m_rowCount;
}

void SSqlFetchingModel::fetchMore(const QModelIndex &parent)
{
    fetchMore(m_fetchSize, parent);
}

//  TODO: реализовать асинхронную загрузку
void SSqlFetchingModel::fetchMore(const int fetchSize, const QModelIndex &parent)
{
    if(!canFetchMore(parent))
        return;

    QString query = QString("%1 LIMIT %2, %3").arg(m_query).arg(m_rowCount).arg(fetchSize);
    QSqlQuery *part = new QSqlQuery(m_db.driver()->createResult());
    int rows = 0;

    part->exec(query);
    rows = part->size();
    if(m_sqlQueries.isEmpty())
    {
        m_rec = part->record();
        m_columnCount = m_rec.count();
    }
    if(rows == 0)
    {
        m_atEnd = 1;
        return;
    }

    beginInsertRows(parent, m_rowCount, m_rowCount + rows);
    m_sqlQueries.insert(m_rowCount, part);
    m_rowCount += rows;
    endInsertRows();
    if(rows < fetchSize)
        m_atEnd = 1;
}

bool SSqlFetchingModel::canFetchMore(const QModelIndex&) const
{
    return !m_atEnd;
}

int SSqlFetchingModel::columnCount(const QModelIndex&) const
{
    return m_columnCount;
}

void SSqlFetchingModel::clear()
{
    beginResetModel();
    while(!m_sqlQueries.isEmpty())
    {
        int k = m_sqlQueries.lastKey();
        QSqlQuery *q = m_sqlQueries.last();
        delete q;
        m_sqlQueries.remove(k);
    }
    m_rowCount = 0;
    m_columnCount = 0;
    m_atEnd = 1;
    m_query = QString();
    m_rec = QSqlRecord();
    QSqlQueryModel::clear();
    endResetModel();
}

int SSqlFetchingModel::fetchSize() const
{
    return m_fetchSize;
}

void SSqlFetchingModel::setFetchSize(int fetchSize)
{
    m_fetchSize = fetchSize;
}

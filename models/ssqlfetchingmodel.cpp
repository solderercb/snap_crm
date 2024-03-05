#include "ssqlfetchingmodel.h"
#include "qsqldriver.h"

SSqlFetchingModel::SSqlFetchingModel(QObject *parent) : QStandardItemModel(parent)
{
    m_proxyQuery = new QSqlQuery();
}

SSqlFetchingModel::~SSqlFetchingModel()
{
    if(m_proxyQuery)
        delete m_proxyQuery;
}

void SSqlFetchingModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_query = query;

    if(!m_db.isValid() || db.connectionName().compare(m_db.connectionName()) != 0)
    {
        m_db = db;
        *m_proxyQuery = QSqlQuery(m_db.driver()->createResult());
    }

    queryRowCount();

    fetchMore(QModelIndex());
}

QVariant SSqlFetchingModel::data(const QModelIndex &item, int role) const
{
    return QStandardItemModel::data(item, role & 0xFF);
}

int SSqlFetchingModel::rowCount(const QModelIndex&) const
{
    return m_rowCount;
}

void SSqlFetchingModel::fetchMore(const QModelIndex &parent)
{
    fetchMore(m_fetchSize, parent);
}

void SSqlFetchingModel::fetchMore(const int fetchSize, const QModelIndex &parent)
{
    if(!canFetchMore(parent))
        return;

    QString query = QString("%1 LIMIT %2, %3").arg(m_query).arg(m_rowCount).arg(fetchSize);

    m_proxyQuery->exec(query);
    copyRowsFromSource();
}

bool SSqlFetchingModel::canFetchMore(const QModelIndex&) const
{
    if(!m_query.isEmpty() && m_rowCount < m_wholeRowCount)
        return true;

    return false;
}

bool SSqlFetchingModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool ret;
    ret = QStandardItemModel::insertRows(row, count, parent);
    m_rowCount = QStandardItemModel::rowCount();
    return ret;
}

QModelIndex SSqlFetchingModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

int SSqlFetchingModel::columnCount(const QModelIndex&) const
{
    return m_columnCount;
}

bool SSqlFetchingModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool ret;
    ret = QStandardItemModel::removeRows(row, count, parent);
    m_rowCount = QStandardItemModel::rowCount();
    return ret;
}

void SSqlFetchingModel::clear()
{
    beginResetModel();
    QStandardItemModel::blockSignals(true);
    QStandardItemModel::removeRows(0, QStandardItemModel::rowCount());
    m_rowCount = 0;
    QStandardItemModel::removeColumns(0, QStandardItemModel::columnCount());
    m_columnCount = 0;
    m_wholeRowCount = 0;
    m_query = QString();
    QStandardItemModel::blockSignals(false);
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

void SSqlFetchingModel::queryRowCount()
{
    QSqlQuery q(m_db);
    QString query =  QString("SELECT COUNT(1) AS 'C1' FROM ( \n"\
                             "  %1                           \n"\
                             ") AS `t0`").arg(m_query);
    q.exec(query);
    if(!q.first())
    {
        m_wholeRowCount = 0;
        return;
    }

    m_wholeRowCount = q.value(0).toInt();
}

void SSqlFetchingModel::copyRowsFromSource()
{
    if(!m_proxyQuery->first())
        return;

    int destRow = m_rowCount;
    int column = 0;

    beginResetModel();
    if(m_columnCount < m_proxyQuery->record().count())
    {
        m_columnCount = m_proxyQuery->record().count();
    }

    do
    {
        QList<QStandardItem*> newRow;
        for(; column < m_proxyQuery->record().count(); column++)
        {
            newRow << new QStandardItem(m_proxyQuery->record().value(column).toString());
        }

        appendRow(newRow);
        m_rowCount++;
        destRow++;
        column = 0;
    } while(m_proxyQuery->next());

    endResetModel();
    m_proxyQuery->clear();
}

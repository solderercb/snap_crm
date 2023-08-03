#include "ssqlfetchingmodel.h"

SSqlFetchingModel::SSqlFetchingModel(QObject *parent) : QStandardItemModel(parent)
{
}

SSqlFetchingModel::~SSqlFetchingModel()
{
}

void SSqlFetchingModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    clear();
    QRegularExpression re("( +\n)|((;?) +$)|(;$)");  // удаление пробелов-заполнителей в конце строк, а также точки с запятой в конце запроса (при наличии ; не будет работать сортировка)

    m_query = query;
    m_query.replace(re, "\n");

    m_db = db;

    m_proxyQuery = new QSqlQuery(m_db);

    queryRowCount();

    if(canFetchMore(QModelIndex()))
        fetchMore(QModelIndex());
}

int SSqlFetchingModel::rowCount(const QModelIndex&) const
{
    return m_rowCount;
}

void SSqlFetchingModel::fetchMore(const QModelIndex&)
{
    QString query = QString("%1 LIMIT %2, %3").arg(m_query).arg(m_rowCount).arg(m_fetchSize);

    m_proxyQuery->exec(query);
    copyRowsFromSource();
}

bool SSqlFetchingModel::canFetchMore(const QModelIndex&) const
{
    if(m_rowCount < m_wholeRowCount)
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
    QStandardItemModel::clear();
    m_rowCount = QStandardItemModel::rowCount();
    m_columnCount = QStandardItemModel::rowCount();
    m_wholeRowCount = 0;
    m_query = "";
    if(m_proxyQuery)
    {
        delete m_proxyQuery;
        m_proxyQuery = nullptr;
    }
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
        insertColumns(m_columnCount, m_proxyQuery->record().count() - m_columnCount, QModelIndex());
        m_columnCount = m_proxyQuery->record().count();
    }

    insertRows(destRow, m_proxyQuery->size(), QModelIndex());
    do
    {

        for(; column < m_proxyQuery->record().count(); column++)
        {
            setData(index(destRow, column), m_proxyQuery->record().value(column), Qt::EditRole);
        }
        destRow++;
        column = 0;
    } while(m_proxyQuery->next());
    endResetModel();
    m_proxyQuery->clear();
}

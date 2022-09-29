#include "scommentsmodel.h"

SCommentsModel::SCommentsModel(QObject *parent) :
    SStandardItemModel(parent),
    queryData(new QSqlQueryModel)
{
    fields = new QMap<QString, int>();
}

SCommentsModel::~SCommentsModel()
{
    delete fields;
}

void SCommentsModel::load(const int repairId)
{
    setQuery(QUERY_SEL_REPAIR_COMMENTS(repairId));
}

QVariant SCommentsModel::data(const QModelIndex &index, int role) const
{
    return SStandardItemModel::data(index, role);
}

void SCommentsModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    queryData->setQuery(query, db);
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для загрузки комментариев будет достаточно вызвать метод this->setQuery()
 */
void SCommentsModel::sqlDataChanged()
{
    setRowCount(queryData->rowCount());
    setColumnCount(queryData->columnCount());
    fields->clear();
    for(int j = 0; j < queryData->columnCount(); j++)
    {
        fields->insert(queryData->headerData(j, Qt::Horizontal).toString(), j);
    }
    for(int i = 0; i < queryData->rowCount(); i++)
    {
        for(int j = 0; j < queryData->columnCount(); j++)
        {
            QStandardItemModel::setData(index(i, j), queryData->index(i, j).data());
        }
    }
    queryData->blockSignals(true);
    queryData->clear();
    queryData->blockSignals(false);
    emit modelReset();
}

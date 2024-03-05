#include "global.h"
#include "stablebasemodel.h"
#include <QtSql>
#include <QString>

STableBaseModel::STableBaseModel(QObject *parent):
    QSqlQueryModel(parent),
    STableModelsCommonMethods(parent)
{
    m_sqlQueryModel = this;
}

void STableBaseModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    beginResetModel();
    blockSignals(true);

    QSqlQueryModel::setQuery(query, db);

    blockSignals(false);
    endResetModel();
}

QVariant STableBaseModel::data(const QModelIndex &item, int role) const
{
    return QSqlQueryModel::data(item, role & 0xFF);
}

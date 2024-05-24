#include "global.h"
#include "stablebasemodel.h"
#include <QtSql>
#include <QString>

const QRegularExpression STableBaseModel::queryPrepareRegexpPattern = QRegularExpression("( +\n)|((;?) +$)|(;$)");

STableBaseModel::STableBaseModel(QObject *parent):
    QSqlQueryModel(parent),
    STableModelsCommonMethods(parent)
{
    m_sqlQueryModel = this;
}

STableBaseModel::~STableBaseModel()
{
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
    if (!item.isValid())
        return QVariant();

    if(role == Qt::ToolTipRole)
    {
        return data(item, Qt::DisplayRole);
    }

    return QSqlQueryModel::data(item, role & 0xFF);
}

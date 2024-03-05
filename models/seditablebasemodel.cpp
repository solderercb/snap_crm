#include "seditablebasemodel.h"

SEditableBaseModel::SEditableBaseModel(QObject *parent, QSqlDatabase db):
    QSqlTableModel(parent, db),
    STableModelsCommonMethods(parent)
{
    m_sqlQueryModel = this;
}

QVariant SEditableBaseModel::data(const QModelIndex &item, int role) const
{
    return QSqlTableModel::data(item, role & 0xFF);
}

QString SEditableBaseModel::selectStatement() const
{
    if(m_selectStatement.isEmpty())
        return QSqlTableModel::selectStatement();

    return QString("%1 %2 %3 %4").arg(m_selectStatement, "WHERE ", filter(), orderByClause());
}

void SEditableBaseModel::setSelectStatement(const QString &statement)
{
    m_selectStatement = statement;
}

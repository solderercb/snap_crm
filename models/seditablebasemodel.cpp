#include "seditablebasemodel.h"

SEditableBaseModel::SEditableBaseModel(QObject *parent, QSqlDatabase db):
    QSqlTableModel(parent, db),
    STableModelsCommonMethods(parent)
{
    m_sqlQueryModel = this;
}

QVariant SEditableBaseModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::ToolTipRole)
    {
        return data(item, Qt::DisplayRole);
    }

    return QSqlTableModel::data(item, role & 0xFF);
}

QString SEditableBaseModel::selectStatement() const
{
    if(m_selectStatement.isEmpty())
        return QSqlTableModel::selectStatement();

    QString qr = m_selectStatement;
    QString wh = filter();
    QString ord = orderByClause();
    if(!wh.isEmpty())
        qr.append("WHERE " + wh);
    if(!ord.isEmpty())
        qr.append("\r\n" + ord);

    return qr;
}

void SEditableBaseModel::setSelectStatement(const QString &statement)
{
    m_selectStatement = statement;
}

void SEditableBaseModel::clear()
{
    beginResetModel();

    /* Вызов родительского метода clear() в т. ч. очистит название таблицы, первичный ключ и названия полей
    * из-за чего не будет возможно редактирвоание (будут выводится сообщения об ошибке запроса); поэтому
    * производим отмену изменений.
    * TODO: нужно придумать что-то более элегантное, т. к. это — костыль
    */
    QSqlTableModel::revertAll();
    endResetModel();
}

bool SEditableBaseModel::insertRowIntoTable(const QSqlRecord &values)
{
    return QSqlTableModel::insertRowIntoTable(values);
}

bool SEditableBaseModel::submit()
{
    return QSqlTableModel::submit();
}

bool SEditableBaseModel::submitAll()
{
    return QSqlTableModel::submitAll();
}

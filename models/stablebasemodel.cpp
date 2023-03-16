#include "stablebasemodel.h"
#include <QtSql>
#include <QString>

STableBaseModel::STableBaseModel(QObject *parent) : QSqlQueryModel(parent)
{
    connect(this, &STableBaseModel::modelReset, this, &STableBaseModel::cashFieldsNames);
}

int STableBaseModel::field(const QString &field)
{
    return i_fields[field];
}

QDateTime STableBaseModel::timestampUtc(const QModelIndex &index) const
{
    QDateTime date = QSqlQueryModel::data(index).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date;
}

QDateTime STableBaseModel::timestampLocal(const QModelIndex &index) const
{
    QDateTime date = QSqlQueryModel::data(index).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date.toLocalTime();
}

void STableBaseModel::cashFieldsNames()
{
    i_fields.clear();

    if(rowCount() == 0)
        return;

    for(int i = 0; i < record(0).count(); i++)
    {
        i_fields.insert(record(0).fieldName(i), i);
    }
}


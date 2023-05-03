#include "global.h"
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

double STableBaseModel::total(int column, int sign)
{
    double totalPositive = 0, totalNegative = 0;
    double current;
    for(int i = 0; i < rowCount(); i++)
    {
        current = STableBaseModel::data(index(i, column)).toDouble();
        if(sign != 0)
        {
            totalPositive += (current>0)?current:0;
            totalNegative += (current<0)?current:0;
        }
        else
            totalPositive += current;
    }

    if(sign >= 0)
        return totalPositive;
    else
        return totalNegative;
}

QString STableBaseModel::dataLocalizedFromDouble(const QModelIndex &item) const
{
    return sysLocale.toString(STableBaseModel::data(item).toDouble(), 'f', comSettings->value("classic_kassa").toBool()?2:0);
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


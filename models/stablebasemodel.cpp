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

void STableBaseModel::beginResetModel()
{
    QSqlQueryModel::beginResetModel();
}

void STableBaseModel::endResetModel()
{
    QSqlQueryModel::endResetModel();
}

void STableBaseModel::cashFieldsNames()
{
    i_fields.clear();
    for(int i = 0; i < record(0).count(); i++)
    {
        i_fields.insert(record(0).fieldName(i), i);
    }
}


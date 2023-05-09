#include "global.h"
#include "stablebasemodel.h"
#include <QtSql>
#include <QString>

STableBaseModel::STableBaseModel(QObject *parent):
    QSqlQueryModel(parent),
    STableModelsCommonMethods(parent)
{
    derivedModel = this;
    connect(this, &STableBaseModel::modelReset, this, &STableBaseModel::slotModelReset);
}

QVariant STableBaseModel::data(const QModelIndex &item, int role) const
{
    return QSqlQueryModel::data(item, role & 0xFF);
}

void STableBaseModel::slotModelReset()
{
    STableModelsCommonMethods::cashFieldsNames();
}


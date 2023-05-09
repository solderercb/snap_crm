#include "seditablebasemodel.h"

SEditableBaseModel::SEditableBaseModel(QObject *parent, QSqlDatabase db):
    QSqlTableModel(parent, db),
    STableModelsCommonMethods(parent)
{
    derivedModel = this;
    connect(this, &SEditableBaseModel::modelReset, this, &SEditableBaseModel::slotModelReset);
}

QVariant SEditableBaseModel::data(const QModelIndex &item, int role) const
{
    return QSqlTableModel::data(item, role & 0xFF);
}

void SEditableBaseModel::slotModelReset()
{
    STableModelsCommonMethods::cashFieldsNames();
}

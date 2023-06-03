#include "stablerepairsmodel.h"

STableRepairsModel::STableRepairsModel(QObject *parent) : STableBaseModel(parent)
{
}

QVariant STableRepairsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // TODO: изучить разницу в скорости выполнения:
    //  1) определение нужного столбца путём вызова кучи методов
    //  2) кэширование названий полей БД по сигналу resetModel() (слот, подключенный в классе STableBaseModel вызывается раньше слота, подключенного в классе виджета STableViewBase)
    if (role == Qt::BackgroundRole)
    {
        QString fieldName = record(index.row()).fieldName(index.column());
        if(fieldName == QString("status"))
            return QColor(statusesModel->value(QSqlQueryModel::data(index, Qt::DisplayRole).toInt(), 1, 2).toString());

        QString rowColor = record(index.row()).value("color").toString();
        if(!rowColor.isEmpty())
            return QColor(rowColor);
    }
    if (role == Qt::DisplayRole)
    {
        QString fieldName = record(index.row()).fieldName(index.column());
        if(fieldName == QString("status"))
            return QVariant(statusesModel->getDisplayRole(QSqlQueryModel::data(index, role).toInt()));
        else if(fieldName == QString("repair_cost"))
            return dataLocalizedFromDouble(index);
        else if(fieldName == QString("in_date"))
            return timestampLocal(index);
        else if(fieldName == QString("out_date"))
            return timestampLocal(index);
        else if(fieldName == QString("master") || fieldName == QString("manager"))
            return userFromId(index);
        else if(fieldName == QString("box"))
            return QVariant(repairBoxesModel->getDisplayRole(QSqlQueryModel::data(index, role).toInt()));
        else if(fieldName == QString("id"))
            return QString("%1-%2").arg(record(index.row()).value("office").toString().rightJustified(3, '0')).arg(QSqlQueryModel::data(index).toString().rightJustified(6, '0'));
        else if(!permissions->viewClients && fieldName == QString("client"))
            return tr("no permissions");
        else if(!permissions->viewClients &&fieldName == QString("phone"))
            return tr("no permissions");
    }
    return STableBaseModel::data(index, role);
}

Qt::ItemFlags STableRepairsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

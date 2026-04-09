#include <ProjectGlobals>
#include "stablecashboxmodel.h"
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <SComSettings>
#include <SPaymentTypesModel>
#include <SCashRegisterModel>
#include <QDebug>

STableCashboxModel::STableCashboxModel(QObject *parent) : STableBaseModel(parent)
{
    m_allOrdersTypes = SCashRegisterModel::paymentTypesList();
}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableCashboxModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case Columns::AmountsByCash:
            case Columns::AmountsCashless:
            case Columns::AmountsByCard:
            case Columns::AmountsOtherType: return dataLocalizedFromDouble(item);
            case Columns::Created: return timestampLocal(item);
            case Columns::Office: return officesModel->getDisplayRole(unformattedData(item).toInt(), 2);
            case Columns::Employee: return userFromId(item);
            case Columns::Covenantor: return dataShort(item);
            case Columns::PaymentType: return "pixmap";
            default: ;
        }
    }
    else if(role == Qt::BackgroundRole) // Операции, проведённые "задним" числом, выделены красным
    {
        if(item.siblingAtColumn(Columns::IsBackdate).data().toBool())
            return QColor(255,99,63);
    }
    else if (role == Qt::ToolTipRole)
    {
        switch (item.column())
        {
            case Columns::PaymentType: return m_allOrdersTypes->getDisplayRole(unformattedData(item.siblingAtColumn(Columns::PaymentType)).toInt());
            default: ;
        }
    }

    return STableBaseModel::data(item, role);
}

int STableCashboxModel::id(const int row)
{
    return unformattedData(index(row, Columns::Id)).toInt();
}

QString STableCashboxModel::dataLocalizedFromDouble(const QModelIndex &item) const
{
    double val = unformattedData(item, Qt::DisplayRole).toDouble();
    if(val == 0)
        return "";

    return STableBaseModel::dataLocalizedFromDouble(val);
}

QModelIndex STableCashboxModel::indexForShortData(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Columns::Covenantor: return index.siblingAtColumn(Columns::ShortName);
    }

    return QModelIndex();
}

double STableCashboxModel::total(int column, int direction)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        double value = unformattedData(index(i, column)).toDouble();
        if((direction == SPaymentTypesModel::PKO) && (value <= 0))
            continue;
        else if((direction == SPaymentTypesModel::RKO) && (value >= 0))
            continue;

        total += value;
    }

    return total;
}

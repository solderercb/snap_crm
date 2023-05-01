#include "stablesalaryreceptedissued.h"

STableSalaryReceptedIssued::STableSalaryReceptedIssued(QObject *parent) : STableBaseModel(parent)
{

}

QVariant STableSalaryReceptedIssued::data(const QModelIndex &item, int role) const
{
    if(role == Qt::BackgroundColorRole) // ранее оплаченные сотруднику ремонты выделены зелёным
    {
        if(index(item.row(), 4).data().toInt())
            return QColor(192,192,192);
    }

    return STableBaseModel::data(item, role);
}

double STableSalaryReceptedIssued::total(bool excludePayed)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(excludePayed == ExcludePayed && index(i, 4).data().toInt()) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += index(i, 5).data().toInt();
    }

    return total;
}

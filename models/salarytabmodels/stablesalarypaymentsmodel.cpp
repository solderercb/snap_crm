#include "global.h"
#include "stablesalarypaymentsmodel.h"

STableSalaryPaymentsModel::STableSalaryPaymentsModel(QObject *parent) : STableBaseModel(parent)
{

}

/* Форматирование данных модели
 * Смотри описание к методу STableSalaryRepairsModel::data(QModelIndex&, int)
*/
QVariant STableSalaryPaymentsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case Columns::PaymentDate:
            case Columns::PeriodFrom:
            case Columns::PeriodTo: return timestampLocal(item);
            case Columns::CashAmount:
            case Columns::BalanceAmount: return dataLocalizedFromDouble(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

/* Сумма выплаченных денег и записанных на баланс разниц
*/
double STableSalaryPaymentsModel::total(const PaymentType type)
{
    double total = 0;
    double cash = 0, balance = 0;
    for(int i=0; i < rowCount(); i++)
    {
        if(STableBaseModel::data(index(i, Columns::Type)).toInt() == type)
        {
            cash = STableBaseModel::data(index(i, Columns::CashAmount)).toDouble();
            balance = STableBaseModel::data(index(i, Columns::BalanceAmount)).toDouble();
            total += cash + ((balance > 0)?balance:0);  // учитываются только поступления на баланс
        }
    }

    return total;
}

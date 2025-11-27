#include <ProjectGlobals>
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
            case Columns::CashAmount: return dataLocalizedFromDouble(item);
            case Columns::BalanceAmount: return balanceChangeValue(item);
            default: ;
        }
    }
    return STableBaseModel::data(item, role);
}

/* Форматирование записи о зачислении на баланс или списании с него
 * Метод добавляет "+" к зачислениям
*/
QString STableSalaryPaymentsModel::balanceChangeValue(const QModelIndex &item) const
{
    double value = unformattedData(item, Qt::DisplayRole).toDouble();
    if(value > 0)
        return "+" + dataLocalizedFromDouble(value);

    return dataLocalizedFromDouble(value);
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
            if(cash == 0 && balance < 0)
                total += balance;
            else
                total += cash + balance;
        }
    }

    return total;
}

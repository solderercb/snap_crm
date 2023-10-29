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
            case 1:
            case 2:
            case 3: return timestampLocal(item);
            case 4:
            case 5: return dataLocalizedFromDouble(item);
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
        if(STableBaseModel::data(index(i, 7)).toInt() == type)
        {
            cash = STableBaseModel::data(index(i, 4)).toDouble();
            balance = STableBaseModel::data(index(i, 5)).toDouble();
            total += cash + ((balance > 0)?balance:0);  // учитываются только поступления на баланс
        }
    }

    return total;
}

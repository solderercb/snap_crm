#include <ProjectGlobals>
#include "stablesalaryrepairsmodel.h"

STableSalaryRepairsModel::STableSalaryRepairsModel(QObject *parent) : STableBaseModel(parent)
{

}

double STableSalaryRepairsModel::total(int column, bool excludePayed)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(excludePayed == ExcludePayed && paidAmount(i)) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += unformattedData(index(i, column)).toDouble();
    }

    return total;
}

double STableSalaryRepairsModel::notIssuedTotal(int column)
{
    double total = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        switch(index(i, Columns::RepState).data().toInt())
        {
            case Global::RepStateIds::Returned:
            case Global::RepStateIds::ReturnedNoRepair:
            case Global::RepStateIds::ReturnedInCredit: continue;
            default: ;
        }

        if(paidAmount(i)) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        total += unformattedData(index(i, column)).toDouble();
    }

    return total;
}

/* Сумма за картриджи отдельной графой */
double STableSalaryRepairsModel::totalForCartridges()
{
    double total = 0;

    for(int i = 0; i < rowCount(); i++)
    {
        if(index(i, Columns::CartridgeId).data().toInt())
            total += unformattedData(index(i, Columns::EmployeeSalaryWorks)).toDouble() + unformattedData(index(i, Columns::EmployeeSalaryParts)).toDouble();
    }

    return total;
}

/* Форматирование данных модели
 * Данный метод (и одноимённые в других классах) для штатных ролей, например DisplayRole, вызывают
 * методы класса STableModelsCommonMethods; те, в свою очередь, повторно вызывают этот метод, но роль
 * отличается значением в 8-м разряде (role | 0x0100), что приводит к вызову одноименного метода из базового
 * класса (STableBaseModel). Такое решение позволяет использовать класс STableModelsCommonMethods в т. ч. для
 * редактируемых моделей данных и при этом избежать проблемы ромбовидного наследования.
*/
QVariant STableSalaryRepairsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (item.column())
        {
            case Columns::RealRepCost:
            case Columns::EmployeeWorks:
            case Columns::AllParts:
            case Columns::EmployeeParts:
            case Columns::EmployeeSalaryWorks:
            case Columns::EmployeeSalaryParts: return dataLocalizedFromDouble(item);
            case Columns::IssueDate: return timestampLocal(item);
            default: ;
        }
    }
    else if(role == Qt::BackgroundRole) // ранее оплаченные сотруднику ремонты выделены серым
    {
        if(item.siblingAtColumn(Columns::Amount).data().toInt())
            return QColor(192,192,192);
    }

    return STableBaseModel::data(item, role);
}

int STableSalaryRepairsModel::id(const int row)
{
    return unformattedData(index(row, Columns::Id)).toInt();
}

int STableSalaryRepairsModel::paidAmount(const int row)
{
    return unformattedData(index(row, Columns::Amount)).toInt();
}

int STableSalaryRepairsModel::visibleRowCount(bool excludePayed)
{
    int visibleRows = 0;
    for(int i = 0; i < rowCount(); i++)
    {
        if(excludePayed && paidAmount(i)) // ранее оплаченные сотруднику ремонты не суммируются
            continue;

        visibleRows++;
    }

    return visibleRows;
}

#ifndef STABLESALARYREPAIRSMODEL_H
#define STABLESALARYREPAIRSMODEL_H

#include "../stablebasemodel.h"

class STableSalaryRepairsModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum IncludePayedInTotal {IncludePayed = 0, ExcludePayed = 1};
    enum Columns {Id = 0, Title = 1, RealRepCost = 2, EmployeeWorks = 3, AllParts = 4, EmployeeParts = 5,
                  EmployeeSalaryWorks = 6, EmployeeSalaryParts = 7, IssueDate = 8, RepState = 9, Amount = 10,
                  RepairState = 11, Filter1 = 12, CartridgeId = 13};
    explicit STableSalaryRepairsModel(QObject *parent = nullptr);
    double total(int column, bool excludePayed = ExcludePayed);
    double notIssuedTotal(int column);
    double totalForCartridges();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    int id(const int row);
    int paidAmount(const int row);
    int visibleRowCount(bool excludePayed);
};

#endif // STABLESALARYREPAIRSMODEL_H

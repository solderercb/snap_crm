#ifndef STABLEVIEWSALARYREPAIRPARTS_H
#define STABLEVIEWSALARYREPAIRPARTS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryRepairParts : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {UID = 0, Name = 1, Qty = 2, Price = 3, Amount = 4, SN = 5, Warranty = 6, Profit = 7, Salary = 8};
    Q_ENUM(Column)
    enum ColumnWidth {UID_w = 100, Name_w = 250, Qty_w = 60, Price_w = 60, Amount_w = 60, SN_w = 60, Warranty_w = 60, Profit_w = 60, Salary_w = 60};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryRepairParts(QWidget *parent = nullptr);
    ~STableViewSalaryRepairParts();
private:
    void translateNames();
};

#endif // STABLEVIEWSALARYREPAIRPARTS_H

#ifndef STABLEVIEWSALARYREPAIRWORKS_H
#define STABLEVIEWSALARYREPAIRWORKS_H

#include <STableViewBase>
#include <QObject>

class STableViewSalaryRepairWorks : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {UID = 0, Name = 1, Qty = 2, Price = 3, Amount = 4, Warranty = 5, Percent = 6, Salary = 7};
    Q_ENUM(Column)
    enum ColumnWidth {UID_w = 50, Name_w = 250, Qty_w = 60, Price_w = 60, Amount_w = 60, Warranty_w = 60, Percent_w = 60, Salary_w = 60};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryRepairWorks(QWidget *parent = nullptr);
    ~STableViewSalaryRepairWorks();
private:
    void translateNames();
};

#endif // STABLEVIEWSALARYREPAIRWORKS_H

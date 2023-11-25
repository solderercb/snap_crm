#ifndef STABLEVIEWSALARYSALEPARTS_H
#define STABLEVIEWSALARYSALEPARTS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalarySaleParts : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {UID = 0, Name = 1, Qty = 2, Price = 3, Amount = 4, SN = 5, Warranty = 6, Profit = 7, Salary = 8};
    Q_ENUM(Column)
    enum ColumnWidth {UID_w = 100, Name_w = 250, Qty_w = 120, Price_w = 120, Amount_w = 120, SN_w = 120, Warranty_w = 120, Profit_w = 120, Salary_w = 120};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalarySaleParts(QWidget *parent = nullptr);
    ~STableViewSalarySaleParts();
private:
    void translateNames();
};

#endif // STABLEVIEWSALARYSALEPARTS_H

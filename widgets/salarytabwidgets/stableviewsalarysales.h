#ifndef STABLEVIEWSALARYSALES_H
#define STABLEVIEWSALARYSALES_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalarySales : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {Doc = 0, Date = 1, Client = 2, Amount = 3, Profit = 4, Salary = 5};
    Q_ENUM(Column)
    enum ColumnWidth {Doc_w = 60, Date_w = 120, Client_w = 250, Amount_w = 160, Profit_w = 160, Salary_w = 160};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalarySales(QWidget *parent = nullptr);
    ~STableViewSalarySales();
private:
    void translateNames();
};

#endif // STABLEVIEWSALARYSALES_H

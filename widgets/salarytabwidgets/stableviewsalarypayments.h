#ifndef STABLEVIEWSALARYPAYMENTS_H
#define STABLEVIEWSALARYPAYMENTS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryPayments : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    enum Column {Doc = 0, Date = 1, PeriodFrom = 2, PeriodTo = 3, CashAmount = 4, BalanceAmount = 5, Employee = 6, User = 7, Notes = 8};
    Q_ENUM(Column)
    enum ColumnWidth {Doc_w = 90, Date_w = 120, PeriodFrom_w = 120, PeriodTo_w = 120, CashAmount_w = 90, BalanceAmount_w = 140, Employee_w = 90, User_w = 90, Notes_w = 250};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryPayments(QWidget *parent = nullptr);
    ~STableViewSalaryPayments();
    XtraSerializer *gridLayout();
private:
    void translateNames();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYPAYMENTS_H

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
    explicit STableViewSalaryPayments(QWidget *parent = nullptr);
    ~STableViewSalaryPayments();
    XtraSerializer *gridLayout();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYPAYMENTS_H

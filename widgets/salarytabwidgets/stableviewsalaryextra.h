#ifndef STABLEVIEWSALARYEXTRA_H
#define STABLEVIEWSALARYEXTRA_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryExtra : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    explicit STableViewSalaryExtra(QWidget *parent = nullptr);
    ~STableViewSalaryExtra();
    XtraSerializer *gridLayout();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYEXTRA_H

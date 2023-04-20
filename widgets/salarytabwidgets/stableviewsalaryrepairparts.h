#ifndef STABLEVIEWSALARYREPAIRPARTS_H
#define STABLEVIEWSALARYREPAIRPARTS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryRepairParts : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryRepairParts(QWidget *parent = nullptr);
    ~STableViewSalaryRepairParts();
};

#endif // STABLEVIEWSALARYREPAIRPARTS_H

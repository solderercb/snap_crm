#ifndef STABLEVIEWSALARYRECEPTEDISSUED_H
#define STABLEVIEWSALARYRECEPTEDISSUED_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryRecepted : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryRecepted(QWidget *parent = nullptr);
    ~STableViewSalaryRecepted();
};


class STableViewSalaryIssued : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryIssued(QWidget *parent = nullptr);
    ~STableViewSalaryIssued();
};
#endif // STABLEVIEWSALARYRECEPTEDISSUED_H

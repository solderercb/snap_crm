#ifndef STABLEVIEWSALARYRECEPTEDISSUED_H
#define STABLEVIEWSALARYRECEPTEDISSUED_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryRecepted : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {Id = 0, Title = 1, RepairCost = 2, ReceptDate = 3};
    Q_ENUM(Column)
    enum ColumnWidth {Id_w = 50, Title_w = 250, RepairCost_w = 115, ReceptDate_w = 115};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryRecepted(QWidget *parent = nullptr);
    ~STableViewSalaryRecepted();
private:
    void translateNames();
};


class STableViewSalaryIssued : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {Id = 0, Title = 1, RepairCost = 2, IssueDate = 3};
    Q_ENUM(Column)
    enum ColumnWidth {Id_w = 50, Title_w = 250, RepairCost_w = 115, IssueDate_w = 115};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryIssued(QWidget *parent = nullptr);
    ~STableViewSalaryIssued();
private:
    void translateNames();
};
#endif // STABLEVIEWSALARYRECEPTEDISSUED_H

#ifndef STABLEVIEWSALARYREPAIRS_H
#define STABLEVIEWSALARYREPAIRS_H

#include <QObject>
#include <STableViewBase>

class STableViewSalaryRepairs : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    enum Column {Id = 0, Title = 1, RealRepCost = 2, EmployeeWorks = 3, AllParts = 4, EmployeeParts = 5, EmployeeSalaryWorks = 6, EmployeeSalaryParts = 7, IssueDate = 8};
    Q_ENUM(Column)
    enum ColumnWidth {Id_w = 100, Title_w = 390, RealRepCost_w = 115, EmployeeWorks_w = 115, AllParts_w = 115, EmployeeParts_w = 115, EmployeeSalaryWorks_w = 115, EmployeeSalaryParts_w = 115, IssueDate_w = 115};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryRepairs(QWidget *parent = nullptr);
    ~STableViewSalaryRepairs();
    XtraSerializer *gridLayout();
    void showRowsPayed(bool state = false);
private:
    bool m_showPayedRepairs = 0;
    void translateNames();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth) override;
public slots:
    void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset) override;
};

#endif // STABLEVIEWSALARYREPAIRS_H

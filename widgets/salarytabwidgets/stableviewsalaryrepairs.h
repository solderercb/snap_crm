#ifndef STABLEVIEWSALARYREPAIRS_H
#define STABLEVIEWSALARYREPAIRS_H

#include "../stableviewbase.h"
#include "widgets/saletableitemdelegates.h"

class STableViewSalaryRepairs : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    explicit STableViewSalaryRepairs(QWidget *parent = nullptr);
    ~STableViewSalaryRepairs();
    XtraSerializer *gridLayout();
    void showRowsPayed(bool state = false);
private:
    bool m_showPayedRepairs = 0;
protected slots:
    void columnResized(int column, int oldWidth, int newWidth) override;
public slots:
    void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset) override;
};

#endif // STABLEVIEWSALARYREPAIRS_H

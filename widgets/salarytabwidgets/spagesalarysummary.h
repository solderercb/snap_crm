#ifndef SPAGESALARYSUMMARY_H
#define SPAGESALARYSUMMARY_H

#include <QWidget>
#include "spagesalarybase.h"

namespace Ui {
class SPageSalarySummary;
}

class SPageSalarySummary : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalarySummary(QWidget *parent = nullptr);
    ~SPageSalarySummary();
private:
    Ui::SPageSalarySummary *ui;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    double m_monthCharge = 0;
public slots:
    void updateModels();
    void userModelReset();
    void updateWidgets();
private slots:
    void setMonthCharge();
};

#endif // SPAGESALARYSUMMARY_H

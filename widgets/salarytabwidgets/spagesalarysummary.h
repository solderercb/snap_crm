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
    bool m_fillMonthChargeOnUpdate = 0;
public slots:
    void updateModels();
    void updateWidgets();
private slots:
    void setMonthCharge();
    void setGroupBoxSubsistanceVisible(bool visible);
    void setFillMonthChargeOnUpdate(const bool state);
};

#endif // SPAGESALARYSUMMARY_H

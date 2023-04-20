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
public slots:
    void updateModels();
    void userModelReset();
    void updateWidgets();
};

#endif // SPAGESALARYSUMMARY_H

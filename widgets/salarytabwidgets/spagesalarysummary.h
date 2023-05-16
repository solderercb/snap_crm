#ifndef SPAGESALARYSUMMARY_H
#define SPAGESALARYSUMMARY_H

#include <QWidget>
#include "spagesalarybase.h"
#include "models/salarytabmodels/ssalarymodel.h"
#include "models/salarytabmodels/ssalaryrepairsmodel.h"
#include "../../squerylog.h"
#include "models/scashregistermodel.h"

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
    SSalaryModel *salaryModel = nullptr;
    SSalaryRepairsModel *salaryRepairsModel = nullptr;
    SQueryLog *m_queryLog;
    SCashRegisterModel *cashRegister;
    SClientModel *m_userClient = nullptr;
    double m_earningSinceLastPay = 0;
    void createUserClientCard();
    void setDbRecordModelsData(const int type, const int system, const double amount, const QString &reason, const QDate date = QDate::currentDate());
public slots:
    void updateModels();
    void updateWidgets();
private slots:
    void setMonthCharge();
    void setGroupBoxSubsistanceVisible(bool visible);
    void setFillMonthChargeOnUpdate(const bool state);
    void pay();
    void paySubsistence();
    void markRepairsPayed();
    void paySalary();
};

#endif // SPAGESALARYSUMMARY_H

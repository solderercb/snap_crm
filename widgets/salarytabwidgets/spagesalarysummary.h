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
    enum CreateUserClientCard{CancelOperation = 0, CreateAutomaticaly, SelectExist, DontCreate, ClientExists};
    Ui::SPageSalarySummary *ui;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    bool m_fillMonthChargeOnUpdate = 0;
    SSalaryModel *salaryModel = nullptr;
    SSalaryRepairsModel *salaryRepairsModel = nullptr;
    SQueryLog *m_queryLog;
    SCashRegisterModel *cashRegister;
    SClientModel *m_userClient = nullptr;
    double m_earningSinceLastPay = 0;
    bool m_commitUserClientModelsPending = 0;
    double m_employeeBalanceToConvert = 0;
    int createUserClientCardMsgBox();
    void createUserClientCard();
    void setDbRecordModelsData(const int type, const int system, const double amount, const QString &reason, const QDate date = QDate::currentDate());
    void addOldBalanceValue();
public slots:
    void updateModels() override;
    void updateWidgets() override;
    void fillClientCreds(int id);
private slots:
    void setMonthCharge();
    void setGroupBoxSubsistanceVisible(bool visible);
    void setFillMonthChargeOnUpdate(const bool state);
    void pay();
    void paySubsistence();
    void markRepairsPayed();
    void paySalary();
    void guiFontChanged() override;
};

#endif // SPAGESALARYSUMMARY_H

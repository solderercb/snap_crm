#ifndef SPAGESALARYSUMMARY_H
#define SPAGESALARYSUMMARY_H

#include <QObject>
#include <SSalaryModel>
#include <SSalaryBasePage>


class QWidget;
class SSortFilterProxyModel;
class SClientModel;
class SQueryLog;
class SCashRegisterModel;

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
    std::unique_ptr<SSalaryModel> salaryModel;
    std::unique_ptr<SCashRegisterModel> cashRegister;
    SQueryLog *m_queryLog;
    SClientModel *m_userClient = nullptr;
    double m_earningSinceLastPay = 0;
    bool m_commitUserClientModelsPending = 0;
    bool m_enableBalancePending = 0;
    double m_employeeBalanceToConvert = 0;
    int m_paymentType = SSalaryModel::Salary;
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
    void setGroupBoxSubsistenceVisible(bool visible);
    void setFillMonthChargeOnUpdate(const bool state);
    void paySubsistence();
    void markRepairsPayed();
    void paySalary();
    void guiFontChanged() override;

    // SWidget interface
public:
    void commit(const int) override;
protected:
    void commitClientUser();
    void enableBalance();
    void commitPayment();
    int commitStages() override;
    int checkInput() override;
    QString queryLogFile() override;
    void configureSalaryModels();
    void configureSubsistenceModels();
    void beginCommit() override;
    bool skip(const int) override;
    void endCommit(const int) override;
    void endCommit() override;
};

#endif // SPAGESALARYSUMMARY_H

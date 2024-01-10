#ifndef SDIALOGISSUEREPAIR_H
#define SDIALOGISSUEREPAIR_H

#include "widgets/smodalwidget.h"
#include <QLocale>
#include "global.h"
#include "shortlivednotification.h"
#include "models/srepairmodel.h"
#include "models/sclientmodel.h"
#include "models/scashregistermodel.h"
#include "models/ssaletablemodel.h"
#include "models/ssortfilterproxymodel.h"
#include "models/sworkshopissuedmodel.h"
#include "models/srepairstatuslog.h"
#include "reports/sprintposreport.h"

namespace Ui {
class SDialogIssueRepair;
}

class SDialogIssueRepair : public SModalWidget
{
    Q_OBJECT

signals:
    void issueSuccessfull();
    void issueFailed();
    void printWorksLists();
public:
    explicit SDialogIssueRepair(QList<SRepairModel*> repairs, Qt::WindowFlags flags = Qt::WindowFlags(), QWidget *parent = nullptr);
    explicit SDialogIssueRepair(QList<SRepairModel*> repairs, QWidget *parent = nullptr); // контруктор для быстрого ремонта (без диалогового окна)
    ~SDialogIssueRepair();
    void setRepairModel(SRepairModel *model);
    void setClientModel(SClientModel *model);
    void setSaleTableModel(SSaleTableModel *model);
    void deleteRepairModels();
    void setListOwner(bool state);
    void setPayFromBalance(bool state = true);
    void setIssuingInCredit(bool state);
    void setPaymentSystemId(int id);
    void issueRepairs();
private:
    enum StateSwitchConditionBits {BitAmountZero = 15, BitInCredit = 14, BitQuickRepair = 13};
    enum StateSwitchConditions {AmountZero = (1 << BitAmountZero), InCredit = (1 << BitInCredit), QuickRepair = (1 << BitQuickRepair)};
    Ui::SDialogIssueRepair *ui = nullptr;
    bool m_summsNotEq = 0;
    bool m_balanceNotEnough = 1;
    bool m_singleRepairWidgetsVisible = 1;
    bool m_rejectReasonWidgetsVisible;
    SClientModel *m_clientModel;
    SSortFilterProxyModel *m_paymentSystemsProxyModel;
    QList<SRepairModel*> m_repairsModels;
    bool m_pushButtonIssueEnabled = 1;
    double m_totalPrepayAmount = 0;
    double m_totalPaymentsAmount = 0;
    double m_totalAgreedAmount = 0;
    double m_totalAmount = 0;
    double m_totalAmountToPay = 0;
    bool m_isCartridgeIssue = 0;
    bool m_isListOwner = 0; // индикатор для удаления моделей ремонтов после закрытия диалога
    bool m_isPayFromBalance = 0;
    bool m_isIssuingInCredit = 0;
    int m_paymentSystemId = 0;
    QString m_rejectReason = "";
    QMap<QString, double> m_repairsWithPayment; // QString для удобства обработки
    void setDefaultStyleSheets();
    void initWidgets();
    void updateWidgets();
    void initPaymentSystems();
    bool checkInput();
    bool checkAmounts();
    void collectRepairsData();
    void setRepairReady(SRepairModel *model);
    void getWidgetsValues();
private slots:
    void buttonIssueClicked();
    void buttonCancelClicked();
    void createLooseDoc();
    void otheRejectReasonShow(QString);
    void textEditTextChanged();
    void paymentSystemChanged(int);
    void checkBoxInCreditToggled(bool);
    void checkBoxIssueInCreditToggled(bool);
};

#endif // SDIALOGISSUEREPAIR_H

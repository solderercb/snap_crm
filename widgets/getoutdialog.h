#ifndef GETOUTDIALOG_H
#define GETOUTDIALOG_H

#include "widgets/swidget.h"
#include <QLocale>
#include "global.h"
#include "shortlivednotification.h"
#include "amountToWords.h"
#include "models/srepairmodel.h"
#include "models/sclientmodel.h"
#include "models/scashregistermodel.h"
#include "models/ssaletablemodel.h"
#include "models/ssortfilterproxymodel.h"
#include "models/sworkshopissuedmodel.h"
#include "models/srepairstatuslog.h"

namespace Ui {
class getOutDialog;
}

class getOutDialog : public SWidget
{
    Q_OBJECT

signals:
    void close();
    void issueSuccessfull();
    void issueFailed();
public:
    explicit getOutDialog(QList<SRepairModel*> repairs, Qt::WindowFlags flags = Qt::WindowFlags(), QWidget *parent = nullptr);
    ~getOutDialog();
    void setRepairModel(SRepairModel *model);
    void setClientModel(SClientModel *model);
    void setSaleTableModel(SSaleTableModel *model);
    void deleteRepairModels();
private:
    Ui::getOutDialog *ui;
    bool m_summsNotEq = 0;
    bool m_balanceNotEnough = 1;
    bool m_singleRepairWidgetsVisible = 1;
    bool m_rejectReasonWidgetsVisible;
    SClientModel *m_clientModel;
    SSortFilterProxyModel *m_paymentSystemsProxyModel;
    QList<SRepairModel*> m_repairsModels;
    bool m_pushButtonIssueEnabled = 1;
    double m_totalPrepayAmount = 0;
    double m_totalAgreedAmount = 0;
    double m_totalAmount = 0;
    double m_totalAmountToPay = 0;
    void setDefaultStyleSheets();
    void initPaymentSystems();
    bool checkInput();
    bool checkAmounts();
    void collectRepairsData();
    void issueRepairs();
private slots:
    void buttonIssueClicked();
    void buttonCancelClicked();
    void createLooseDoc();
    void otheRejectReasonShow(QString);
    void textEditTextChanged();
    void paymentSystemChanged(int);
    void checkBoxInCreditToggled(bool);
};

#endif // GETOUTDIALOG_H

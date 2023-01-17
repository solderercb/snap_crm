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
    void getOutOk();
public:
    explicit getOutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~getOutDialog();
    void setRepairModel(SRepairModel *model);
    void setClientModel(SClientModel *model);
    void setSaleTableModel(SSaleTableModel *model);

private:
    Ui::getOutDialog *ui;
    bool summsNotEq = 0;
    double summToPay;
    bool balanceNotEnough = 1;
    int newState;
    int m_repairId;
    SRepairModel *repairModel;
    SClientModel *clientModel;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    SSaleTableModel *saleTableModel;
    void setDefaultStyleSheets();
    void initPaymentSystems();
    bool checkAmounts();
private slots:
    void getOut();
    void cancel();
    void createLooseDoc();
    void otheRejectReasonShow(QString);
    void textEditTextChanged();
    void paymentSystemChanged(int);
    void checkBoxInCreditToggled(bool);
};

#endif // GETOUTDIALOG_H

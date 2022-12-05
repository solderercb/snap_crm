#ifndef GETOUTDIALOG_H
#define GETOUTDIALOG_H

#include <QWidget>
#include <QLocale>
#include "global.h"
#include "shortlivednotification.h"
#include "amountToWords.h"
#include "squerylog.h"
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

class getOutDialog : public QWidget
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
    bool balanceNotEnough = 1;
    QWidget *parent;
    int newState;
    int m_repairId;
    void setDefaultStyleSheets();
    SRepairModel *repairModel;
    SClientModel *clientModel;
    float summToPay;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    void initPaymentSystems();
    bool checkAmounts();
    SSaleTableModel *saleTableModel;
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

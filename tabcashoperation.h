#ifndef TABCASHOPERATION_H
#define TABCASHOPERATION_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include "tabcommon.h"
#include "models/slogrecordmodel.h"
#include "models/scashregistermodel.h"
#include "models/sclientmodel.h"
#include "models/srepairmodel.h"
#include "models/sinvoicemodel.h"
#include "models/sdocumentmodel.h"
#include "models/ssortfilterproxymodel.h"
#include "widgets/shortlivednotification.h"
#ifdef QT_DEBUG
#include "querylog.h"
#endif

namespace Ui {
class tabCashOperation;
}

class tabCashOperation : public tabCommon
{
    Q_OBJECT
signals:
    void createTabClient(int);
    void createTabSelectExistingClient(int, QWidget *);
    void createTabSelectRepair(int, QWidget *);
    void createTabSelectDocument(int, QWidget *);
    void createTabSelectInvoice(int, QWidget *);
    void createTabOpenRepair(int);
    void createTabOpenDocument(int);
    void createTabOpenInvoice(int);
public:
    enum Type {RKO = -1, PKO = -2};
    enum ClientFieldsAccess {Full = 0, SelNClr = 1, Denied = 2};
    explicit tabCashOperation(int orderId = PKO, MainWindow *parent = nullptr);
    static tabCashOperation* getInstance(int orderId, MainWindow *parent);
    ~tabCashOperation();
    QString tabTitle();
private:
    enum LinkedObjectType {NoLink = 0, Document = 1, Repair = 2, Invoice = 3};
    enum Client {NotVisible = 0, Client = 1, Employee = 2};
    static QMap<int, tabCashOperation*> p_instance;
    SCashRegisterModel *cashRegister;
    SPaymentTypesModel *paymentTypesModel;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    int m_orderId = 0;
    int m_orderType = 0;
    float m_amount = 0;
    int m_client = 0;
    int m_employee = 0;
    QString m_tabTitle;
    int m_linkedObjId = 0;
    QString m_linkedObjIdStr;
    int m_linkedObjType = LinkedObjectType::NoLink;
    void (SCashRegisterModel::*method)(int) = nullptr;
    int m_showClient = Client::Client;
    int m_clientRO = ClientFieldsAccess::Denied;
    QString m_reason;
    bool m_reasonRO = 1;
    bool m_amountRO = 0;
    bool m_reasonManEdited = 0;
    bool m_showBalance = 0;
    bool m_skipAutoLogRecord = 0;
    bool m_backdate = 0;
    SRepairModel *repair = nullptr;
    SDocumentModel *document = nullptr;
    SInvoiceModel *invoice = nullptr;
    Ui::tabCashOperation *ui;
    SClientModel *clientModel;
    void updateWidgets();
    bool checkInput();
    void load(const int);
    void initPKO();
    void initRKO();
    void clearLinkedObjectFields();
    void showLinkedObject();
    void showClient();
    void updateLineEditButtons(int state = ClientFieldsAccess::Full);
    void setAmountReadOnly(bool state = 1);
    void setDefaultStylesheets();
    bool commitSimple();
    bool commitBalance(float amount);
    bool commitDocument();
    bool commitInvoice();
    bool commitPrepayRepair();
    void print();
    QString constructReason();
    void setAmount(const float);
    void updateOrderIdLineEdit();
#ifdef QT_DEBUG
    enum RandomFillerStep {OpType = 1, LinkedObj, RClient, Amount, PaymentSys, End};
    void randomFill();
    void dbgRandomRepair();
    void dbgRandomDocument(int type);
    void dbgRandomInvoice();
    void dbgRandomClient(bool balanceEnabled = 0);
#endif

private slots:
    void clearClientCreds(bool);
    void fillClientCreds(int);
    void fillRepairCreds(int);
    void fillDocumentCreds(int);
    void fillInvoiceCreds(int);
    void orderTypeChanged(int);
    void dateChanged(QDate);
    void findClientByLastname(QString);
    void autoReasonEnabled(int);
    void reasonEdited(QString);
    void paymentSystemChanged(int);
    void lineEditClientLastNameButtonClickHandler(int);
    void buttonOpenDocClicked();
    void buttonSaveClicked();
    void buttonPrintClicked();
    void buttonRevertClicked();
    void amountChanged(double);
    void applyPaymentSystem();
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif
};

#endif // TABCASHOPERATION_H

#ifndef TABCASHOPERATION_H
#define TABCASHOPERATION_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QMap>
#include "tabcommon.h"
#include "models/slogrecordmodel.h"
#include "models/scashregistermodel.h"
#include "models/sclientmodel.h"
#include "models/srepairmodel.h"
#include "models/sinvoicemodel.h"
#include "models/sdocumentmodel.h"
#include "models/ssortfilterproxymodel.h"
#include "widgets/shortlivednotification.h"
#include "widgets/sclientselectform.h"

#define DATE_EDIT_REFRESH_TIMER 30000

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
    void createTabUndoOperation(int, QMap<int, QVariant>);
    void createTabOpenRepair(int);
    void createTabOpenDocument(int);
    void createTabOpenInvoice(int);
    void generatePrintout(QMap<QString, QVariant>);
public:
    enum Type {RKO = -1, PKO = -2};
    explicit tabCashOperation(int orderId = PKO, MainWindow *parent = nullptr);
    static tabCashOperation* getInstance(int orderId, MainWindow *parent);
    ~tabCashOperation();
    QString tabTitle() override;
    void prepareTemplate(QMap<int, QVariant>);
private:
    enum LinkType {NoLink = 0, Document = 1, Repair = 2, Invoice = 3};
    enum ClientMode {NotVisible = 0, Client = 1, Employee = 2};
    enum TemplateDataType {TmplType = 0, TmplObjId, TmplClient, TmplAmount};    // QMap сортирует данные по ключу, поэтому первым должен быть тип
    static QMap<int, tabCashOperation*> p_instance;
    SCashRegisterModel *m_cashRegisterModel = nullptr;
    SPaymentTypesModel *m_operationTypesModel;
    SSortFilterProxyModel *m_paymentAccountsProxyModel;
    int m_initialOrderId = 0;
    int m_orderId = 0;
    int m_orderType = 0;
    bool m_comboBoxTypeRO = 0;
    double m_amount = 0;
    QString m_linkId = 0;
    QString m_linkTitle;
    int m_linkType = LinkType::NoLink;
    bool m_clientWidgetVisible = true;
    int m_clientWidgetMode = ClientMode::Client;
    int m_clientWidgetButtons = SClientSelectForm::AccessMode::Denied;
    QString m_generatedReason;
    bool m_reasonRO = 1;
    bool m_amountRO = 0;
    double m_minAmount = 0;
    double m_maxAmount = 1.7E308;
    bool m_reasonManEdited = 0;
    bool m_showBalance = 0;
    bool m_skipAutoLogRecord = 0;
    bool m_backdate = 0;
    bool m_buttonRevertVisible = 0;
    bool m_paymentAccountRO = 0;
    bool m_checkBoxPrintVisible = 1;
    SRepairModel *m_repairModel = nullptr;
    SDocumentModel *m_documentModel = nullptr;
    SInvoiceModel *m_invoiceModel = nullptr;
    Ui::tabCashOperation *ui;
    SClientModel *m_clientModel = nullptr;
    QString m_companyLabel;
    QString m_widgetClientLabel;
    void initModels();
    void initWidgets();
    void load(const int);
    void updateWidgets();
    void initCashRegisterModel();
    void initPKO();
    void initRKO();
    void setOrderTypeModel(SPaymentTypesModel *model);
    void resetWidgetsSettings();
    void resetLinkVariables();
    void deleteLinkObject();
    void updateLinkWidgets();
    void updateClientWidget();
    void setDefaultStylesheets();
    bool checkInput();
    bool commit(bool repeatAfter = 0);
    bool commitLink();
    bool commitSimple();
    bool commitBalance(const double amount);
    bool commitDocument();
    bool commitInvoice();
    bool commitPrepayRepair();
    bool commitRevert();
    void print();
    void updateReasonWidget();
    void updateAmountWidget();
    double amountAbsToSign(const double amountAbs);
    int paymentSystemId();
    void setPaymentAccountIndex(const int id);
    void updateLineEditOrderId();
    void updateModelData();
#ifdef QT_DEBUG
    enum RandomFillerStep {OpType = 1, LinkedObj, RClient, Amount, PaymentSys, End};
    void randomFill() override;
    void dbgRandomRepair();
    void dbgRandomDocument(int type);
    void dbgRandomInvoice();
#endif
public slots:
    void fillDeviceCreds(int repairId);
private slots:
    void fillClientCreds(int);
    void clientModelUpdated();
    void initRepairVariables(int);
    void initDocumentVariables(int);
    void initInvoiceVariables(int);
    void initLinkVariables(int objId = 0);
    void operationTypeChanged(int);
    void autoReasonEnabled(int);
    void reasonEdited(QString);
    void paymentSystemChanged(int);
    void buttonOpenDocClicked();
    void buttonSaveClicked();
    void buttonSaveMoreClicked();
    void buttonPrintClicked();
    void buttonRevertClicked();
    void amountChanged(double);
    void applyPaymentSystem();
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABCASHOPERATION_H

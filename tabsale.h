#ifndef TABSALE_H
#define TABSALE_H

#include <tabCommon>

class SDocumentModel;
class SClientModel;
class QSqlQueryModel;
class StoreSaleModel;
class tabSaleSettingsMenu;
class SCashRegisterModel;
class SSortFilterProxyModel;

namespace Ui {
class tabSale;
}

class tabSale : public tabCommon
{
    Q_OBJECT

signals:
    void createTabSelectExistingClient(int, QWidget*);
    void createTabClient(int);
    void createTabSparePart(int);
    void generatePrintout(QMap<QString, QVariant> report_vars);
    void createTabSelectItem(int, QWidget*);

public:
    enum OpType {Sale = 0, Reserve = 1, SaleReserved = 2, Unsale = 3, CancelReserve = 4, SetTrack = 5};
    enum EndCommitOp {SwitchToViewMode, PrepareRepeat};
    explicit tabSale(int, MainWindow *parent = nullptr);
    static tabSale* getInstance(int, MainWindow *parent = nullptr);
    ~tabSale();
    QString tabTitle() override;
private:
    Ui::tabSale *ui;
    static QMap<int, tabSale*> p_instance;
    int doc_id = 0;
    SDocumentModel *docModel;
    SClientModel *clientModel;
    QSqlQueryModel* clientPhonesModel;
    StoreSaleModel *tableModel;
    tabSaleSettingsMenu *widgetAction;
    SCashRegisterModel *cashRegister;
    SSortFilterProxyModel *m_priceColProxyModel;
    int m_client = 0;
    int price_col = 2;
    int m_opType = Sale;
    int *params;
    int m_endCommitOp = SwitchToViewMode;
    void updateTabPtr(const int oldId, const int newId);
    void initPriceColModel();
    void eventResize(QResizeEvent *);
    void setDefaultStyleSheets();
    void setBalanceWidgetsVisible(bool);
    void updateWidgets();
    int checkInputNew();
    int checkInputUnsale();
    int checkInput() override;
    int commitStages() override;
    int targetPaymentAccount();
    void prepareCashRegisterModel(const double amount, const int account = 0);   // по умолчанию Наличные
    QString prepareUnsaleNote();
    void prepareDocModel();
    void beginCommit() override;
    bool skip(const int stage) override;
    void commit(const int stage) override;
    void commitClient();
    void commitNew();
    void commitUnsale();
    void commitCancelReserve();
    void commitTrack();
    void commitLogs();
    void throwHandler(int) override;
    void endCommit(const int stage) override;
    void endCommit() override;
    void prepareForRepeatedOp();
    void switchTabToViewMode();
    void clearAll();
    void print();
#ifdef QT_DEBUG
    void randomFill() override;
    void createTestPanel();
    QWidget *testPanel;
    QLineEdit *testLineEdit;
    QPushButton *testPushButton;
    QPushButton *testBtnAddRandomItem;
#endif

public slots:

private slots:
    void clearClientCreds(bool hideCoincidence = true);
    void fillClientCreds(int);
    void comboBoxIndexChanged(int);
    void updateTotalSumms(const double amountTotal, const double, const double);
    void phoneNumberEdited(QString);
    void buttonSelectExistingClientHandler();
    void buttonCreateTabClientHandler();
    void tableRowDoubleClick(QModelIndex);
    void hideGroupBoxClient(bool);
    void selectPriceCol(int);
    void addItemByUID();
    void saleButtonClicked();
    void saleMoreButtonClicked();
    void reserveButtonClicked();
    void addItemButtonClicked();
    void paramsButtonClicked();
    void updateChargeAmount(QString);
    void phoneTypeChanged(int);
    void setTrackNum();
    void setTrackNum(int);
    void reserveCancelButtonClicked();
    void printButtonClicked();
    void logButtonClicked();
    void unSaleButtonClicked();
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
    void test_updateWidgetsWithDocNum();
#endif
};

#endif // TABREPAIR_H

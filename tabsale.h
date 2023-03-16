#ifndef TABSALE_H
#define TABSALE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>
#include <QMessageBox>
#include "tabcommon.h"
#include "models/sdocumentmodel.h"
#include "models/sstandarditemmodel.h"
#include "models/ssqlquerymodel.h"
#include "models/ssaletablemodel.h"
#include "models/slogrecordmodel.h"
#include "models/scashregistermodel.h"
#include "models/sclientmodel.h"
#include "models/sphonemodel.h"
#include "widgets/saletableitemdelegates.h"
#include "widgets/tabsalesettingsmenu.h"
#include "widgets/shortlivednotification.h"
#include "widgets/stableviewbase.h"
#include "widgets/saletableitemdelegates.h"

namespace Ui {
class tabSale;
}

class sparePartsTable : public STableViewBase
{
    Q_OBJECT
signals:
    void createTabSparePart(int);
public:
    explicit sparePartsTable(QWidget *parent = nullptr);
    ~sparePartsTable();
    void setModel(QAbstractItemModel *model) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    SSaleTableModel *m_model = nullptr;
    SaleTableItemDelegates *m_itemDelagates = nullptr;
};

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
    enum OpType {Sale = 0, Reserve = 1, SaleReserved = 2};
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
    SSaleTableModel *tableModel;
    SaleTableItemDelegates *itemDelagates;
    tabSaleSettingsMenu *widgetAction;
    SCashRegisterModel *cashRegister;
    int client = 0;
    int price_col = 2;
    int m_opType = 0;
    int *params;
    int m_docState = 0;
    void eventResize(QResizeEvent *);
    void setDefaultStyleSheets();
    void setBalanceWidgetsVisible(bool);
    void updateWidgets();
    bool checkInput();
    bool createClient();
    bool createNewDoc();
    bool sale();
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
    bool unSale();
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
    void paymentSystemChanged(int);
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
    void test_updateWidgetsWithDocNum();
#endif
};

#endif // TABREPAIR_H

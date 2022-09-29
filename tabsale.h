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
#include "models/saletablemodel.h"
#include "models/slogrecordmodel.h"
#include "models/scashregistermodel.h"
#include "models/sclientmodel.h"
#include "models/sphonemodel.h"
#include "widgets/saletableitemdelegates.h"
#include "widgets/tabsalesettingsmenu.h"
#include "widgets/shortlivednotification.h"

namespace Ui {
class tabSale;
}

class sparePartsTable : public QTableView
{
    Q_OBJECT

public:
    explicit sparePartsTable(QWidget *parent = nullptr);
    ~sparePartsTable();
    void resizeEvent(QResizeEvent*);
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>());
#endif
private:

};

class tabSale : public tabCommon
{
    Q_OBJECT

signals:
    void createTabSelectExistingClient(int, QWidget*);
    void createTabClient(int);
    void createTabSparePart(int);
    void generatePrintout(QMap<QString, QVariant> report_vars);

public:
    enum DocState {Sale = 0, Reserve = 1, SaleReserved = 2};
    explicit tabSale(int, MainWindow *parent = nullptr);
    static tabSale* getInstance(int, MainWindow *parent = nullptr);
    ~tabSale();

private:
    Ui::tabSale *ui;
    void setDefaultStyleSheets();
    void setBalanceWidgetsVisible(bool);
    static QMap<int, tabSale*> p_instance;
    int doc_id = 0;
    SDocumentModel *docModel;
    SClientModel *clientModel;
    float total_sum, takein_sum, charge_sum;
    void eventResize(QResizeEvent *);
    QSqlQueryModel* clientPhonesModel;
    int client = 0;
    int price_col = 2;
    void updateWidgets();
    bool checkInput();
    SaleTableModel *tableModel;
    QSqlQueryModel *newItemModel;
    SaleTableItemDelegates *itemDelagates;
    int reserve = 0;
    bool createClient();
    bool createNewDoc();
    bool sale();
    void clearAll();
    int isItemAlreadyInList(int);
    void addItemByUID(int);
    tabSaleSettingsMenu *widgetAction;
    int *params;
    QMessageBox msgBox;
    SCashRegisterModel *cashRegister;
    int m_docState = 0;
    void print();
#ifdef QT_DEBUG
    void randomFill();
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
    void updateTotalSumms(float);
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
    void test_scheduler_handler();
    void test_scheduler2_handler();
    void test_updateWidgetsWithDocNum();
    void test_addRandomItem();
#endif
};

#endif // TABREPAIR_H

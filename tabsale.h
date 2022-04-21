#ifndef TABSALE_H
#define TABSALE_H

#include <QWidget>
#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QScrollBar>
#include <QTimeZone>
#include <QDateTime>
#include <QLocale>
#include <QRandomGenerator>
#include <QPushButton>
#include "tabcommon.h"
#include "sstandarditemmodel.h"
#include "ssqlquerymodel.h"
#include "models/saletable/saletablemodel.h"
#include "models/saletable/saletableitemdelegates.h"
#include "widgets/sgroupboxeventfilter.h"
#include "models/tabsalesettingsmenu.h"
#ifdef QT_DEBUG
#include <QTimer>
#include <QFile>
#include "querylog.h"
#endif

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
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
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
    explicit tabSale(int, MainWindow *parent = nullptr);
    static tabSale* getInstance(int, MainWindow *parent = nullptr);
    ~tabSale();

private:
    Ui::tabSale *ui;
    void setDefaultStyleSheets();
    void setBalanceWidgetsVisible(bool);
    static QMap<int, tabSale*> p_instance;
    int doc_id;
    QSqlQueryModel *docModel;
    SSqlQueryModel *clientModel;
    float total_sum, takein_sum, charge_sum;
    void eventResize(QResizeEvent *);
    QSqlQueryModel* clientsMatchTable;
    SGroupBoxEventFilter *groupBoxEventFilter;
    QSqlQueryModel* clientPhonesModel;
    int client = 0;
    int price_col = 2;
    void updateDocData();
    void updateWidgets();
    bool checkInput();
    SaleTableModel *tableModel;
    QSqlQueryModel *newItemModel;
    SaleTableItemDelegates *itemDelagates;
    int reserve = 0;
    QString genUserWebPass() const;
    bool sale();
    void clearAll();
    int isItemAlreadyInList(int);
    void addItemByUID(int);
    tabSaleSettingsMenu *widgetAction;
    int *params;

#ifdef QT_DEBUG
    void randomFill();
    QTimer *test_scheduler, *test_scheduler2, *main_window_test_scheduler, *main_window_test_scheduler2;
    uint test_scheduler_counter = 0;
    QWidget *testPanel;
    QLineEdit *testLineEdit;
    QPushButton *testPushButton;
#endif

public slots:

private slots:
    void clearClientCreds(bool);
    void fillClientCreds(int);
    void comboBoxIndexChanged(int);
    void updateTotalSumms(float);
    void findMatchingClient(QString);
    void buttonSelectExistingClientHandler();
    void buttonCreateTabClientHandler();
    void tableRowDoubleClick(QModelIndex);
    void hideGroupBoxClient(bool);
    void selectPriceCol(int);
    void clientMatchTableDoubleClicked(QModelIndex);
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
    void reserveCancelButtonClicked();
    void printButtonClicked();
    void logButtonClicked();
    void unSaleButtonClicked();
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
    void test_updateWidgetsWithDocNum();
#endif
};

#endif // TABREPAIR_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QTextCodec>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QWidgetAction>
#include <QStyle>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QToolButton>
#include <QComboBox>
#include <QMap>
#include <QTimer>
#include <QCompleter>
#include <QFontMetrics>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QRandomGenerator>
#include <QCloseEvent>
#include "windowsdispatcher.h"
#include "widgets/shortlivednotification.h"
#include <QtAutoUpdaterWidgets/UpdateController>
#include <QtAutoUpdaterWidgets/UpdateButton>

namespace Ui {
class MainWindow;
}

class tabBarEventFilter : public QObject
{
    Q_OBJECT
public:
    tabBarEventFilter(QObject*);
protected:
    bool eventFilter(QObject*, QEvent*) override;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

signals:
	void DBConnectErr(const QString &message);
	void DBConnectOK();
public:
    static MainWindow* getInstance(windowsDispatcher *parent = nullptr);
    ~MainWindow();
    void addTab(QWidget *widget);
    void insertTab(QWidget *tab, int rightOf = -1);
    bool event(QEvent *event) override;
#ifdef QT_DEBUG
    QTimer *test_scheduler, *test_scheduler2;
    uint test_scheduler_counter = 0;
#endif
private:
    explicit MainWindow(windowsDispatcher *parent = nullptr);
    Ui::MainWindow *ui;
    static MainWindow* p_instance;
    void closeEvent(QCloseEvent*) override;
    void createMenu();
    void readGoods(const QModelIndex &index, const QString &warehouse_code);
	void readConsignments(const QModelIndex &index, const QString &warehouse_code);
	void get_warehouses_list();
	QStandardItemModel *comboboxSourceModel;
	QStandardItemModel *comboboxDestModel;
	QStandardItemModel *tree_model;
	QSortFilterProxyModel *proxy_tableGoodsModel;	// Модель для преобразования исходной модели с целью специфических способов сортировки и фильтрования данных.
	QStandardItemModel *tableGoodsModel;	// Модель таблицы отображающей наименования имеющихся в наличии товаров
	QStringList tableGoodsHeaders;
	QStandardItemModel *tableConsignmentsModel; // Модель таблицы, отображающей партии товара
	QStringList tableConsignmentsHeaders;
    QtAutoUpdater::UpdateController *updateController = nullptr;
#ifdef QT_DEBUG
    void createTestTab();
#endif

public slots:
    void createTabRepairs(int type = 0, QWidget *caller = nullptr);    // Этот слот public, т. к. может создаваться по-умолчанию при запуске приложения.
    void createTabRepairNew();  // Этот слот public только для debug'а, в релизе нужно сделать его private
    void createTabRepair(int);
    void createTabClients(int type = 0, QWidget *caller = nullptr);
    QWidget *createTabClients(QWidget *callerTab);
    void createTabClient(int);
    void createTabRepairCartridges(QList<int> *list);
    void createTabSale(int doc_id = 0);
    void createTabSparePart(int);
    void createTabSparePartReserve(int);
    void switchToLastUsedTab();
    void createTabTechReports();
    void createTabPurchaseManager();
    QWidget *createTabPartRequest(const int id);
    void reactivateCallerTab(QWidget *);
private slots:
    void reactivateTabRepairNew(int);
    void createTabPrint(QMap<QString, QVariant>); // Создание вкладки предпросмотра/печати только через слот; прямой вызов функции с вкладки приёма в ремонт приводил к падению программы.
    void createTabCashOperations();
    void createTabCashOperation( int, QMap<int, QVariant> data = QMap<int, QVariant>() );
    void createTabNewPKO();
    void createTabNewRKO();
    void createTabCashMoveExch();
    void createTabDocuments(int type = 0, QWidget *caller = nullptr);
    void createTabInvoices(int type = 0, QWidget *caller = nullptr);
    void createTabWarehouseItems(int type = 0, QWidget *caller = nullptr);
    void createTabSettings();
    void createTabSalary();
    void createTabReceptCartridge();
    void createTabStoreItems();
    void createTabCashOrders();
    void createTabTasks();
    bool closeTab(int index);
    void updateTabTitle(QWidget*);
    void updateTabIcon(QWidget*);
    void createUpdaterWidget();
    void applyGuiSettings();
#ifdef QT_DEBUG
    void btnClick();
    void on_treeView_clicked(const QModelIndex &index);
    void on_treeView_activated(const QModelIndex &index);
    void on_comboBoxSourceWarehouse_currentIndexChanged(int index);
    void on_tableGoods_activated(const QModelIndex &index);
    void on_tableGoods_clicked(const QModelIndex &index);
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif

};

#endif // MAINWINDOW_H

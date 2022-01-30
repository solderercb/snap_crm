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
#include "windowsdispatcher.h"

namespace Ui {
class MainWindow;
}

class tabBarEventFilter : public QObject
{
    Q_OBJECT
signals:

public:
    tabBarEventFilter(QObject*);
private:
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
    static MainWindow* getInstance(QWidget *parent = nullptr);
    ~MainWindow();	// Деструктор
    QSqlQueryModel* permissionsModel;
    QSqlQueryModel* companiesModel;
    QSqlQueryModel* officesModel;
    QSqlQueryModel* warehousesModel;
    QSqlQueryModel* usersModel;
    QSqlQueryModel* managersModel;
    QSqlQueryModel* engineersModel;
    QSqlQueryModel* boxesModel;
//    QSqlQueryModel* Model;
//    QSqlQueryModel* Model;
//    QSqlQueryModel* Model;

private:
    explicit MainWindow(QWidget *parent = 0);
    Ui::MainWindow *ui;
    static MainWindow* p_instance;
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
    void initGlobalModels();    // общие модели данных: организации, офисы, склады, сотрудники (все, менеджеры, инженеры) и др.
#ifdef QT_DEBUG
    QTimer *test_scheduler, *test_scheduler2;
    uint test_scheduler_counter = 0;
#endif

public slots:
    void createTabRepairs(int type = 0);    // Этот слот public, т. к. может создаваться по-умолчанию при запуске приложения.
    void createTabRepairNew();  // Этот слот public только для debug'а, в релизе нужно сделать его private

private slots:
	void btnClick();
	void on_treeView_clicked(const QModelIndex &index);
	void on_treeView_activated(const QModelIndex &index);
	void on_comboBoxSourceWarehouse_currentIndexChanged(int index);
	void on_tableGoods_activated(const QModelIndex &index);
	void on_tableConsignments_clicked(const QModelIndex &index);
	void on_tableGoods_clicked(const QModelIndex &index);

    void createTabRepair(int);
    void reactivateTabRepairNew(int);
    void createTabClients(int type = 0);
    void createTabClient(int);
    void closeTab(int index);
    void applyStyleSheet();
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif

};

#endif // MAINWINDOW_H

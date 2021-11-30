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
#include "sqlcreds.h"
#include "ui_bottoolbarwidget.h"
#include "dbconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

signals:
	void DBConnectErr(const QString &message);
	void DBConnectOK();


public:
	explicit MainWindow(QWidget *parent = 0); // Конструктор
	~MainWindow();	// Деструктор
    DBConnection* dbConnection;

private:
	Ui::MainWindow *ui; // Указатель на класс MainWindow
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

public slots:
    void ConnectToDB(const QString &username, const QString &password, const QString &ipaddr, const uint &port, const QString &dbName, const QString &connName);

private slots:
	void btnClick();
	void on_treeView_clicked(const QModelIndex &index);
	void on_treeView_activated(const QModelIndex &index);
	void on_comboBoxSourceWarehouse_currentIndexChanged(int index);
	void on_tableGoods_activated(const QModelIndex &index);
	void on_tableConsignments_clicked(const QModelIndex &index);
	void on_tableGoods_clicked(const QModelIndex &index);
    void on_workshop_button_triggered();
    void createTabRepairNew();
    void closeTab(int index);
};

#endif // MAINWINDOW_H

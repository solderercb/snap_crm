#ifndef TABCLIENTS_H
#define TABCLIENTS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include "mainwindow.h"
#include "tabcommon.h"

namespace Ui {
class tabClients;
}

class tabClients : public tabCommon
{
    Q_OBJECT

signals:
    void doubleClicked(int);
    void activateCaller(QWidget *);

public:
    explicit tabClients(bool type = 0, MainWindow *parent = nullptr);
    static tabClients* getInstance(bool type, MainWindow *parent = nullptr);
    void lineEditSearchSetFocus();
    ~tabClients();
    void setCallerPtr(QWidget *ptr){callerPtr = ptr;};

private:
    Ui::tabClients *ui;
    static tabClients* p_instance[2];
    bool _type;
    void updateTableWidget();
    QTableView* tableView;
    QSqlQueryModel* clientsTable;
    QSortFilterProxyModel* proxyModel;
    QString query_static;
    QString query_where_static;
    QString query_group_static;
    QString query_order_static;
    QStringList query;
    QStringList query_where;
    QStringList query_group;
    QStringList query_order;
    QWidget *callerPtr = nullptr;

private slots:
    void clientTypeChanged(QModelIndex);
    void clientAdvertisingChanged(int);
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);
    void togglePropertiesPanel();

};

#endif // TABCLIENTS_H

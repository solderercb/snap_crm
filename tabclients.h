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
#include "repairstablemodel.h"
#include "dbconnection.h"

namespace Ui {
class tabClients;
}

class tabClients : public QWidget
{
    Q_OBJECT

signals:
    void doubleClicked(int);

public:
    explicit tabClients(bool type = 0, QWidget *parent = nullptr);
    static tabClients* getInstance(bool type, QWidget *parent = nullptr);
    void lineEditSearchSetFocus();
    ~tabClients();

private:
    Ui::tabClients *ui;
    static tabClients* p_instance[2];
    bool _type;
    DBConnection* dbConnection;
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
    QStandardItemModel* clientsTypesList;
    QSqlQueryModel* clientsAdTypesList;
    QList<QStandardItem*> clientTypeSelector[8];

private slots:
    void clientTypeChanged(QModelIndex);
    void clientAdvertisingChanged(int);
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);

};

#endif // TABCLIENTS_H

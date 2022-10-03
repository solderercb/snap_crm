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
#include <QTimer>
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

public:
    explicit tabClients(bool type = 0, MainWindow *parent = nullptr);
    static tabClients* getInstance(bool type, MainWindow *parent = nullptr);
    void lineEditSearchSetFocus();
    ~tabClients();
    QString tabTitle() override;
private:
    Ui::tabClients *ui;
    static tabClients* p_instance[2];
    bool _type;
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
    QTimer *tableUpdateDelay;
#ifdef QT_DEBUG
    void randomFill(){};
#endif

private slots:
    void updateTableWidget();
    void clientTypeChanged(QModelIndex);
    void clientAdvertisingChanged(int);
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);
    void togglePropertiesPanel();
#ifdef QT_DEBUG
    void test_scheduler_handler(){};
    void test_scheduler2_handler(){};
#endif
};

#endif // TABCLIENTS_H

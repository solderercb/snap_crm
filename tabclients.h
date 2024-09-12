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
#include "models/stableclientsmodel.h"
#include "widgets/stableviewbase.h"

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
    ~tabClients();
    bool event(QEvent *event) override;
    void setCategory(int category);
    void setFocusSearchField();
private:
    Ui::tabClients *ui;
    static tabClients* p_instance[2];
    bool m_type;
    STableClientsModel* clientsTable;
    QStringList query_group;
    void constructQueryClause();
#ifdef QT_DEBUG
    void randomFill() override{};
#endif

private slots:
    void refreshTable(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
    void clientTypeChanged(QModelIndex);
    void clientAdvertisingChanged(int);
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void togglePropertiesPanel();
    void buttonRefreshClicked();
    void createNewClient();
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABCLIENTS_H

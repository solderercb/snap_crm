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
    bool m_type;
    QSqlQueryModel* clientsTable;
    QStringList query_group;
    QTimer *m_tableUpdateDelay;
#ifdef QT_DEBUG
    void randomFill() override{};
#endif

private slots:
    void refreshTable();
    void clientTypeChanged(QModelIndex);
    void clientAdvertisingChanged(int);
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void togglePropertiesPanel();
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABCLIENTS_H

#ifndef TABREPAIRS_H
#define TABREPAIRS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QRegularExpressionValidator>
#include <QFrame>
#include <QWidgetAction>
#include <QTimer>
#include "tabcommon.h"
#include "models/repairstablemodel.h"
#include "models/repairtablefiltermenu.h"

namespace Ui {
class tabRepairs;
}

class tabRepairs : public tabCommon
{
    Q_OBJECT

signals:
    void doubleClicked(int);
    void buttonRepairNewClicked();

public:
    explicit tabRepairs(bool type = 0, MainWindow *parent = nullptr);
    static tabRepairs* getInstance(bool type, MainWindow *parent = nullptr);
    ~tabRepairs();
    QString tabTitle() override;
    void updateWidgets();
private:
    enum TableMode{NoCartridges = 0, Cartridges = 1};
    Ui::tabRepairs *ui;
    QMap<QString, int> *filterSettings;
    static tabRepairs* p_instance[2];
    bool _type;
    QTableView* tableView;
    QSqlQueryModel* repairs_table;
    QSqlQueryModel* cartridges_table = nullptr;
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

public slots:
    void refreshTable();
private slots:
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);
    void tableModeChanged(bool mode);
#ifdef QT_DEBUG
    void test_scheduler_handler(){};
    void test_scheduler2_handler(){};
#endif
};

#endif // TABREPAIRS_H

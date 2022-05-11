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
#include "repairstablemodel.h"
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

private:
    Ui::tabRepairs *ui;
    QMap<QString, int> *filterSettings;
    static tabRepairs* p_instance[2];
    bool _type;
    QTableView* tableView;
    QSqlQueryModel* repairs_table;
    QString query_static;
    QString query_where_static;
    QString query_group_static;
    QString query_order_static;
    QStringList query;
    QStringList query_where;
    QStringList query_group;
    QStringList query_order;
    QTimer *tableUpdateDelay;

private slots:
    void updateTableWidget();
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);
};

#endif // TABREPAIRS_H

#ifndef TABREPAIR_H
#define TABREPAIR_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QScrollBar>
#include <QTimeZone>
#include <QDateTime>
#include "tabcommon.h"

namespace Ui {
class tabRepair;
}

class commentsTable : public QTableView
{
    Q_OBJECT

public:
    explicit commentsTable(QWidget *parent = nullptr);
    ~commentsTable();
    void resizeEvent(QResizeEvent*);
private:
};

class commentsDataModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit commentsDataModel(QWidget *parent = nullptr);
    ~commentsDataModel();
private:
    QVariant data(const QModelIndex &index, int role) const;
};

class tabRepair : public tabCommon
{
    Q_OBJECT

signals:
    void worksTreeDoubleClicked(int);

public:
    explicit tabRepair(int rep_id, MainWindow *parent = nullptr);
    static tabRepair* getInstance(int rep_id, MainWindow *parent = nullptr);
    ~tabRepair();

private:
    Ui::tabRepair *ui;
    static QMap<int, tabRepair*> p_instance;
    int repair_id;
    QSqlQueryModel *repairModel;
    QSqlQueryModel *clientModel;
    QSqlQueryModel *fieldsModel;
    QSortFilterProxyModel *statusesProxyModel;
    commentsDataModel *commentsModel;
    bool statusUpdateFlag;
    void getRepairData();
    int getFieldIdByName(const QString &, QSqlQueryModel *);
    QString getDisplayRoleById(int, QAbstractItemModel*, int column = 0);
    void eventResize(QResizeEvent *);

    void updateTableWidget();
//    QTableView* tableView;
    QSqlQueryModel* works_table;

private slots:
    void worksTreeDoubleClicked(QModelIndex);
    void comboBoxIndexChanged(int);
//    void lineEditSearchTextChanged(QString);
//    void lineEditSearchReturnPressed();

};

#endif // TABREPAIR_H

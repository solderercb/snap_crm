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
#include "repairstablemodel.h"
#include "dbconnection.h"

namespace Ui {
class tabRepairs;
}

class tabRepairs : public QWidget
{
    Q_OBJECT

signals:
    void doubleClicked(int);

public:
    explicit tabRepairs(bool type = 0, QWidget *parent = nullptr);
    static tabRepairs* getInstance(QWidget *parent = nullptr);
    ~tabRepairs();

private:
    Ui::tabRepairs *ui;
    static tabRepairs* p_instance;
    DBConnection* dbConnection;
    void updateTableWidget();
    QTableView* tableView;
    QSqlQueryModel* repairs_table;

private slots:
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();

};

#endif // TABREPAIRS_H

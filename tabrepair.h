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
#include "tabcommon.h"

namespace Ui {
class tabRepair;
}

class tabRepair : public tabCommon
{
    Q_OBJECT

signals:
    void worksTreeDoubleClicked(int);

public:
    explicit tabRepair(int repair_id = 0, MainWindow *parent = nullptr);
    static tabRepair* getInstance(MainWindow *parent = nullptr);
    ~tabRepair();

private:
    Ui::tabRepair *ui;
    static tabRepair* p_instance;
    void updateTableWidget();
//    QTableView* tableView;
    QSqlQueryModel* works_table;

private slots:
    void worksTreeDoubleClicked(QModelIndex);
//    void lineEditSearchTextChanged(QString);
//    void lineEditSearchReturnPressed();

};

#endif // TABREPAIR_H

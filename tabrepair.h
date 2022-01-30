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

namespace Ui {
class tabRepair;
}

class tabRepair : public QWidget
{
    Q_OBJECT

signals:
    void worksTreeDoubleClicked(int);

public:
    explicit tabRepair(int repair_id = 0, QWidget *parent = nullptr);
    static tabRepair* getInstance(QWidget *parent = nullptr);
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

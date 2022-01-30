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
#include "repairstablemodel.h"

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
    static tabRepairs* getInstance(bool type, QWidget *parent = nullptr);
    ~tabRepairs();

private:
    Ui::tabRepairs *ui;
    static tabRepairs* p_instance[2];
    bool _type;
    void updateTableWidget();
    QTableView* tableView;
    QSqlQueryModel* repairs_table;

private slots:
    void tableItemDoubleClick(QModelIndex);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableSectionMoved(int, int, int);
    void tableSectionResized(int, int, int);
    void tableSortingChanged(int, Qt::SortOrder);

};

#endif // TABREPAIRS_H

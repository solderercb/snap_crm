#ifndef SPAGESALARYITEMS_H
#define SPAGESALARYITEMS_H

#include "spagesalarybase.h"
#include <QWidget>
#include "../stableviewbase.h"

namespace Ui {
class SPageSalaryItems;
}

// Таблица с суммами по столбцам
class STableViewSalaryItemsSummary : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryItemsSummary(QWidget *parent = nullptr);
    ~STableViewSalaryItemsSummary();
    void setGridLayout(XtraSerializer* layout);
    void setTotal(int column, double value);
private:
    QStandardItemModel *dataModel;
public slots:
    void applyGridlayout();
protected slots:
    void columnResized(int, int, int) override {};
};

class SPageSalaryItems : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryItems(QWidget *parent = nullptr);
    ~SPageSalaryItems();

private:
    Ui::SPageSalaryItems *ui;
    void enableVScrollbarPadding(const bool state = 1);
public slots:
    void updateModels();
    void updateWidgets();
    void tableItemsRowDoubleClicked(const QModelIndex &index);
private slots:
    void repairsTableColumnResized(int column, int newWidth);
};

#endif // SPAGESALARYITEMS_H

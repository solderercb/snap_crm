#ifndef SPAGESALARYREPAIRS_H
#define SPAGESALARYREPAIRS_H

#include <QWidget>
#include "spagesalarybase.h"
#include "../stableviewbase.h"
#include "widgets/stableviewbaseitemdelegates.h"

namespace Ui {
class SPageSalaryRepairs;
}

// Таблица с суммами по столбцам
class STableViewSalaryRepairsSummary : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryRepairsSummary(QWidget *parent = nullptr);
    ~STableViewSalaryRepairsSummary();
    void setGridLayout(XtraSerializer* layout);
    void setTotal(int column, double value);
private:
    QStandardItemModel *dataModel;
public slots:
    void applyGridlayout();
protected slots:
    void columnResized(int, int, int) override {};
};

class SPageSalaryRepairs : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryRepairs(QWidget *parent = nullptr);
    ~SPageSalaryRepairs();
private:
    Ui::SPageSalaryRepairs *ui;
    void loadRepairData(const int id);
    void enableVScrollbarPadding(const bool state = 1);
public slots:
    void updateModels();
    void updateWidgets();
    void repairsModelReset();
    void tableRepairsRowActivated(const QModelIndex &index);
    void tableRepairsRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableRepairsRowDoubleClicked(const QModelIndex &index);
    void tableRepairPartsRowDoubleClicked(const QModelIndex &index);
private slots:
    void repairsTableColumnResized(int column, int newWidth);
};

#endif // SPAGESALARYREPAIRS_H

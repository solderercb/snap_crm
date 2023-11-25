#ifndef SPAGESALARYREPAIRS_H
#define SPAGESALARYREPAIRS_H

#include <QWidget>
#include "spagesalarybase.h"
#include "../stableviewbase.h"
#include "../stableviewsummarybase.h"
#include "widgets/stableviewbaseitemdelegates.h"

namespace Ui {
class SPageSalaryRepairs;
}

// Таблица с суммами по столбцам
class STableViewSalaryRepairsSummary : public STableViewSummaryBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryRepairsSummary(QWidget *parent = nullptr);
    void setGridLayout(XtraSerializer* layout) override;
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
    void showRowsPayed(bool state = false);
    void updateSummary();
public slots:
    void updateModels() override;
    void updateWidgets() override;
    void repairsModelReset();
    void tableRepairsRowActivated(const QModelIndex &index);
    void tableRepairsRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableRepairsRowDoubleClicked(const QModelIndex &index);
    void tableRepairPartsRowDoubleClicked(const QModelIndex &index);
private slots:
    void tableColumnResized(int column, int newWidth);
    void guiFontChanged() override;
};

#endif // SPAGESALARYREPAIRS_H

#ifndef SPAGESALARYITEMS_H
#define SPAGESALARYITEMS_H

#include <QObject>
#include <STableViewSummaryBase>
#include <SSalaryBasePage>

class QWidget;

namespace Ui {
class SPageSalaryItems;
}

// Таблица с суммами по столбцам
class STableViewSalaryItemsSummary : public STableViewSummaryBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryItemsSummary(QWidget *parent = nullptr);
    void setGridLayout(XtraSerializer* layout);
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
    void updateModels() override;
    void updateWidgets() override;
    void tableItemsRowDoubleClicked(const QModelIndex &index);
private slots:
    void tableColumnResized(int column, int newWidth);
    void guiFontChanged() override;
};

#endif // SPAGESALARYITEMS_H

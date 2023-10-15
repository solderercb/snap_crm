#ifndef SPAGESALARYEXTRA_H
#define SPAGESALARYEXTRA_H

#include <QWidget>
#include "spagesalarybase.h"
#include "../stableviewbase.h"

namespace Ui {
class SPageSalaryExtra;
}

// Таблица с суммами по столбцам
class STableViewSalaryExtraSummary : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryExtraSummary(QWidget *parent = nullptr);
    ~STableViewSalaryExtraSummary();
    void setGridLayout(XtraSerializer* layout);
    void setTotal(int column, double value);
private:
    QStandardItemModel *dataModel;
public slots:
    void applyGridlayout();
protected slots:
    void columnResized(int, int, int) override {};
};

class SPageSalaryExtra : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryExtra(QWidget *parent = nullptr);
    ~SPageSalaryExtra();

private:
    Ui::SPageSalaryExtra *ui;
    void enableVScrollbarPadding(const bool state = 1);
public slots:
    void updateModels();
    void updateWidgets();
private slots:
    void tableColumnResized(int column, int newWidth);
};

#endif // SPAGESALARYEXTRA_H

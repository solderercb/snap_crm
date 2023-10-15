#ifndef SPAGESALARYPAYMENTS_H
#define SPAGESALARYPAYMENTS_H

#include <QWidget>
#include "spagesalarybase.h"
#include "../stableviewbase.h"

namespace Ui {
class SPageSalaryPayments;
}

// Таблица с суммами по столбцам
class STableViewSalaryPaymentsSummary : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryPaymentsSummary(QWidget *parent = nullptr);
    ~STableViewSalaryPaymentsSummary();
    void setGridLayout(XtraSerializer* layout);
    void setTotal(int column, double value);
private:
    QStandardItemModel *dataModel;
public slots:
    void applyGridlayout();
protected slots:
    void columnResized(int, int, int) override {};
};

class SPageSalaryPayments : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryPayments(QWidget *parent = nullptr);
    ~SPageSalaryPayments();
private:
    Ui::SPageSalaryPayments *ui;
public slots:
    void updateModels();
    void updateWidgets();
private slots:
    void tableColumnResized(int column, int newWidth);
};

#endif // SPAGESALARYPAYMENTS_H

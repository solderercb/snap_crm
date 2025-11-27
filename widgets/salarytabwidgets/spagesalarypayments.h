#ifndef SPAGESALARYPAYMENTS_H
#define SPAGESALARYPAYMENTS_H

#include <QObject>
#include <SSalaryBasePage>
#include <STableViewSummaryBase>

class QWidget;

namespace Ui {
class SPageSalaryPayments;
}

// Таблица с суммами по столбцам
class STableViewSalaryPaymentsSummary : public STableViewSummaryBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryPaymentsSummary(QWidget *parent = nullptr);
    void setGridLayout(XtraSerializer* layout);
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

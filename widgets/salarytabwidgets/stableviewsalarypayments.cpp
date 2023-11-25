#include "stableviewsalarypayments.h"

STableViewSalaryPayments::STableViewSalaryPayments(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryPaymentsGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Notes].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryPayments::~STableViewSalaryPayments()
{
}

XtraSerializer *STableViewSalaryPayments::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryPayments::translateNames()
{
    tr("Doc");
    tr("Date");
    tr("PeriodFrom");
    tr("PeriodTo");
    tr("CashAmount");
    tr("BalanceAmount");
    tr("Employee");
    tr("User");
    tr("Notes");
}

void STableViewSalaryPayments::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

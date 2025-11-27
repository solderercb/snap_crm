#include "stableviewsalaryrepairworks.h"
#include <STableViewGridLayout>

STableViewSalaryRepairWorks::STableViewSalaryRepairWorks(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairPartsGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryRepairWorks::~STableViewSalaryRepairWorks()
{
}

void STableViewSalaryRepairWorks::translateNames()
{
    tr("Name");
    tr("Qty");
    tr("Price");
    tr("Amount");
    tr("Warranty");
    tr("Percent");
    tr("Salary");
}

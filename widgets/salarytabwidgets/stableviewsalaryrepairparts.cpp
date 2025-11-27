#include "stableviewsalaryrepairparts.h"
#include <STableViewGridLayout>

STableViewSalaryRepairParts::STableViewSalaryRepairParts(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairWorksGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryRepairParts::~STableViewSalaryRepairParts()
{
}

void STableViewSalaryRepairParts::translateNames()
{
    tr("Name");
    tr("Qty");
    tr("Price");
    tr("Amount");
    tr("SN");
    tr("Warranty");
    tr("Profit");
    tr("Salary");
}

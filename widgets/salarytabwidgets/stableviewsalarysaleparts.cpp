#include "stableviewsalarysaleparts.h"
#include <STableViewGridLayout>

STableViewSalarySaleParts::STableViewSalarySaleParts(QWidget *parent) :
    STableViewBase(SLocalSettings::SalarySalePartsGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalarySaleParts::~STableViewSalarySaleParts()
{
}

void STableViewSalarySaleParts::translateNames()
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

#include "stableviewsalarysales.h"
#include <STableViewGridLayout>

STableViewSalarySales::STableViewSalarySales(QWidget *parent) :
    STableViewBase(SLocalSettings::SalarySalesGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Client].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalarySales::~STableViewSalarySales()
{
}

void STableViewSalarySales::translateNames()
{
    tr("Doc");
    tr("Date");
    tr("Client");
    tr("Amount");
    tr("Profit");
    tr("Salary");
}

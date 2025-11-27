#include "stableviewsalaryitems.h"
#include <STableViewGridLayout>

STableViewSalaryItems::STableViewSalaryItems(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryItemsGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryItems::~STableViewSalaryItems()
{
}

XtraSerializer *STableViewSalaryItems::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryItems::translateNames()
{
    tr("Name");
    tr("Date");
    tr("Qty");
    tr("Price");
    tr("Amount");
    tr("State");
}

void STableViewSalaryItems::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

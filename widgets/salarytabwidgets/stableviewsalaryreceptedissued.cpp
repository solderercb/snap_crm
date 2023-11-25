#include "stableviewsalaryreceptedissued.h"

STableViewSalaryRecepted::STableViewSalaryRecepted(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryReceptedGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Title].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryRecepted::~STableViewSalaryRecepted()
{
}

void STableViewSalaryRecepted::translateNames()
{
    tr("Id");
    tr("Title");
    tr("RepairCost");
    tr("ReceptDate");
}

STableViewSalaryIssued::STableViewSalaryIssued(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryIssuedGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Title].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryIssued::~STableViewSalaryIssued()
{
}

void STableViewSalaryIssued::translateNames()
{
    tr("Id");
    tr("Title");
    tr("RepairCost");
    tr("IssueDate");
}

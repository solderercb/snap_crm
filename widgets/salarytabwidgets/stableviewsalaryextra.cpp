#include "stableviewsalaryextra.h"
#include <STableViewGridLayout>
#include <SSalaryExtraModel>

STableViewSalaryExtra::STableViewSalaryExtra(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryExtraChargesGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Reason].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryExtra::~STableViewSalaryExtra()
{
}

XtraSerializer *STableViewSalaryExtra::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryExtra::setModel(QAbstractItemModel *model)
{
    STableViewBase::setModel(model);
}

void STableViewSalaryExtra::translateNames()
{
    tr("ID");
    tr("Reason");
    tr("Amount");
    tr("Date");
    tr("User");
}

void STableViewSalaryExtra::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

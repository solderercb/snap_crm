#include "stableviewsalaryrepairs.h"
#include <STableViewGridLayout>
#include <SSalaryRepairsModel>

STableViewSalaryRepairs::STableViewSalaryRepairs(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairsGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Title].Width_marked = true;  // автоширина по умолчанию
}

STableViewSalaryRepairs::~STableViewSalaryRepairs()
{
}

XtraSerializer* STableViewSalaryRepairs::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryRepairs::showRowsPayed(bool state)
{
    m_showPayedRepairs = state;
    for(int i = 0; i < m_model->rowCount(); i++)
    {
        if(state)
            showRow(i);
        else
        {
            if(static_cast<STableSalaryRepairsModel*>(m_model)->paidAmount(i))
                hideRow(i);
        }
    }
}

void STableViewSalaryRepairs::translateNames()
{
    tr("Id");
    tr("Title");
    tr("RealRepCost");
    tr("EmployeeWorks");
    tr("AllParts");
    tr("EmployeeParts");
    tr("EmployeeSalaryWorks");
    tr("EmployeeSalaryParts");
    tr("IssueDate");
}

void STableViewSalaryRepairs::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

void STableViewSalaryRepairs::refresh(bool, bool)
{
    STableViewBase::refresh();
    showRowsPayed(m_showPayedRepairs);
}

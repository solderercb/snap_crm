#include "stableviewsalaryrepairs.h"
#include "models/salarytabmodels/stablesalaryrepairsmodel.h"

STableViewSalaryRepairs::STableViewSalaryRepairs(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairsGrid, parent)
{
    i_defaultColumnsWidths = {{0, 100},{1, 690},{2, 115},{3, 115},{4, 115},{5, 115},{6, 115},{7, 115},{8, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Работа") << tr("Все детали") << tr("Детали") << tr("Заработок (раб.)") << tr("Заработок (ЗИП)") << tr("Дата выдачи");
    readLayout();
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
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
            if(static_cast<STableSalaryRepairsModel*>(m_model)->payedSumm(i))
                hideRow(i);
        }
    }
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

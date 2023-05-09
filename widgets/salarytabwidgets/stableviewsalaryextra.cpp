#include "stableviewsalaryextra.h"
#include "models/salarytabmodels/stablesalaryextramodel.h"

STableViewSalaryExtra::STableViewSalaryExtra(QWidget *parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 400},{2, 60},{3, 115},{4, 150}};
    i_defaultHeaderLabels << "ID" << tr("Основание") << tr("Сумма") << tr("Дата") << tr("Пользователь");
    readLayout(SLocalSettings::SalaryExtraChargesGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с основанием
}

STableViewSalaryExtra::~STableViewSalaryExtra()
{
    saveLayout(SLocalSettings::SalaryExtraChargesGrid);
}

XtraSerializer *STableViewSalaryExtra::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryExtra::setModel(QAbstractItemModel *model)
{
    STableViewBase::setModel(model);
}

void STableViewSalaryExtra::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

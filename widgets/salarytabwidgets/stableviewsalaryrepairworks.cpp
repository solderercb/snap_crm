#include "stableviewsalaryrepairworks.h"

STableViewSalaryRepairWorks::STableViewSalaryRepairWorks(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairPartsGrid, parent)
{
    i_defaultColumnsWidths = {{0, 50},{1, 250},{2, 60},{3, 60},{4, 60},{5, 60},{6, 60},{7, 60}};
    i_defaultHeaderLabels << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Цена") << tr("Сумма") << tr("Гарантия") << tr("%") << tr("Заработок");
    readLayout();
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryRepairWorks::~STableViewSalaryRepairWorks()
{
}

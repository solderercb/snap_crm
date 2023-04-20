#include "stableviewsalaryrepairparts.h"

STableViewSalaryRepairParts::STableViewSalaryRepairParts(QWidget *parent)
{
    i_defaultColumnsWidths = {{0, 100},{1, 250},{2, 60},{3, 60},{4, 60},{5, 60},{6, 60}};
    i_defaultHeaderLabels << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Цена") << tr("Сумма") << tr("SN") << tr("Гарантия");
    readLayout(SLocalSettings::SalaryRepairWorksGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryRepairParts::~STableViewSalaryRepairParts()
{
    saveLayout(SLocalSettings::SalaryRepairWorksGrid);
}

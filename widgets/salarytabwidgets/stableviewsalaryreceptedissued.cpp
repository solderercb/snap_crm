#include "stableviewsalaryreceptedissued.h"

STableViewSalaryRecepted::STableViewSalaryRecepted(QWidget *parent) :
    STableViewBase(parent)
{
    i_defaultColumnsWidths = {{0, 50},{1, 250},{2, 115},{3, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Дата приёма");
    readLayout(SLocalSettings::SalaryReceptedGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryRecepted::~STableViewSalaryRecepted()
{
    saveLayout(SLocalSettings::SalaryReceptedGrid);
}

STableViewSalaryIssued::STableViewSalaryIssued(QWidget *parent) :
    STableViewBase(parent)
{
    i_defaultColumnsWidths = {{0, 50},{1, 250},{2, 115},{3, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Дата выдачи");
    readLayout(SLocalSettings::SalaryIssuedGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryIssued::~STableViewSalaryIssued()
{
    saveLayout(SLocalSettings::SalaryIssuedGrid);
}

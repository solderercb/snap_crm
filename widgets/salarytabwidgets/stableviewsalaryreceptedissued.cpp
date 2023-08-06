#include "stableviewsalaryreceptedissued.h"

STableViewSalaryRecepted::STableViewSalaryRecepted(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryReceptedGrid, parent)
{
    i_defaultColumnsWidths = {{0, 50},{1, 250},{2, 115},{3, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Дата приёма");
    readLayout();
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryRecepted::~STableViewSalaryRecepted()
{
}

STableViewSalaryIssued::STableViewSalaryIssued(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryIssuedGrid, parent)
{
    i_defaultColumnsWidths = {{0, 50},{1, 250},{2, 115},{3, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Дата выдачи");
    readLayout();
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryIssued::~STableViewSalaryIssued()
{
}

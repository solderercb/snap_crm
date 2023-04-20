#include "stableviewsalaryrepairs.h"

STableViewSalaryRepairs::STableViewSalaryRepairs(QWidget *parent) : STableViewBase(parent)
{
    i_defaultColumnsWidths = {{0, 100},{1, 690},{2, 115},{3, 115},{4, 115},{5, 115},{6, 115},{7, 115},{8, 115}};
    i_defaultHeaderLabels << tr("Заказ") << tr("Оборудование") << tr("Стоимость ремонта") << tr("Работа") << tr("Все детали") << tr("Детали") << tr("Заработок (раб.)") << tr("Заработок (ЗИП)") << tr("Дата выдачи");
    readLayout(SLocalSettings::SalaryRepairsGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryRepairs::~STableViewSalaryRepairs()
{
    saveLayout(SLocalSettings::SalaryRepairsGrid);
}

XtraSerializer* STableViewSalaryRepairs::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryRepairs::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

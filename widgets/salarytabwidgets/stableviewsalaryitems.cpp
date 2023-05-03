#include "stableviewsalaryitems.h"

STableViewSalaryItems::STableViewSalaryItems(QWidget *parent)
{
    i_defaultColumnsWidths = {{0, 100},{1, 280},{2, 115},{3, 60},{4, 115},{5, 115},{6, 115}};
    i_defaultHeaderLabels << tr("UID") << tr("Наименование") << tr("Дата выдачи") << tr("Кол-во") << tr("Цена") << tr("Сумма") << tr("Статус");
    readLayout(SLocalSettings::SalaryItemsGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalaryItems::~STableViewSalaryItems()
{
    saveLayout(SLocalSettings::SalaryItemsGrid);
}

XtraSerializer *STableViewSalaryItems::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryItems::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

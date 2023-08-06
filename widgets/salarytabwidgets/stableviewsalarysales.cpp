#include "stableviewsalarysales.h"

STableViewSalarySales::STableViewSalarySales(QWidget *parent) :
    STableViewBase(SLocalSettings::SalarySalesGrid, parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 120},{2, 250},{3, 160},{4, 160},{5, 160}};
    i_defaultHeaderLabels << tr("№ РН") << tr("Дата") << tr("Клиент") << tr("Сумма") << tr("Прибыль") << tr("Заработок");
    readLayout();
    i_gridLayout->$GridControl.Columns[2].Width_marked = true;  // по умолчанию автоширина столбца с ФИО клиента
}

STableViewSalarySales::~STableViewSalarySales()
{
}

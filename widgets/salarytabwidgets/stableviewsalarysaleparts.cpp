#include "stableviewsalarysaleparts.h"

STableViewSalarySaleParts::STableViewSalarySaleParts(QWidget *parent)
{
    i_defaultColumnsWidths = {{0, 100},{1, 250},{2, 120},{3, 120},{4, 120},{5, 120},{6, 120},{7, 120},{8, 120}};
    i_defaultHeaderLabels << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Цена") << tr("Сумма") << tr("SN") << tr("Гарантия") << tr("Прибыль") << tr("Заработок");
    readLayout(SLocalSettings::SalarySalePartsGrid);
    i_gridLayout->$GridControl.Columns[1].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewSalarySaleParts::~STableViewSalarySaleParts()
{
    saveLayout(SLocalSettings::SalarySalePartsGrid);
}

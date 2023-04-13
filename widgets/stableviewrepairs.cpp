#include "stableviewrepairs.h"

STableViewRepairs::STableViewRepairs(QWidget *parent) :
    STableViewBase(parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 90},{2, 270},{3, 130},{4, 220},{5, 120},{6, 120},{7, 90},{8, 120},{9, 120},{10, 230}, {11, 140}, {12, 120}, {13, 110}};
    i_defaultHeaderLabels << tr("i") << tr("Заказ") << tr("Оборудование") << tr("ФИО клиента") << tr("Оборудование") << tr("Серийный номер") << tr("Офис") << tr("Менеджер") << tr("Инженер") << tr("Дата поступления") << tr("Дата выдачи") << tr("Статус") << tr("Неисправность") << tr("Сумма") << tr("Контактный телефон") << tr("Ячейка") << tr("Срок");
    readLayout(SLocalSettings::RepairsGrid);

    setItemDelegate(new STableViewRepairsItemDelegates());
}

STableViewRepairs::~STableViewRepairs()
{
    saveLayout(SLocalSettings::RepairsGrid);
}



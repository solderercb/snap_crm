#include "stableviewclients.h"

STableViewClients::STableViewClients(QWidget *parent) :
    STableViewBase(SLocalSettings::CustomersGrid, parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 300},{2, 90},{3, 90},{4, 90},{5, 60},{6, 140},{7, 140}};
    i_defaultHeaderLabels << tr("ID") << tr("ФИО") << tr("Баланс") << tr("Ремонтов") << tr("Покупок") << tr("Тип") << tr("Телефон") << tr("email");

    readLayout();
    // в файле АСЦ заголовки столбцов на английском, после импорта восстанавливаем значения по умолчанию
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        i_gridLayout->$GridControl.Columns[i].FieldName = i_defaultHeaderLabels.value(i, i_gridLayout->$GridControl.Columns[i].FieldName);
    }
//    setItemDelegate(new STableViewClientsItemDelegates());
}

STableViewClients::~STableViewClients()
{
}

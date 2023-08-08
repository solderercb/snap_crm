#include "stableviewrepairs.h"

STableViewRepairs::STableViewRepairs(QWidget *parent) :
    STableViewBase(SLocalSettings::RepairsGrid, parent)
{
    readLayout();

    // в АСЦ в таблице ремонтов по умолчанию отображаются значения доп полей;
    // при импорте файла настроек в этих столбцах будут отображаться служебные данные; возможно позже я тоже сделаю отображение данных доп. полей, а пока скрываю их.
    for(int i = metaObject()->enumerator(metaObject()->indexOfEnumerator("Column")).keyCount(); i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        i_gridLayout->$GridControl.Columns[i].Visible = false;
    }

    setItemDelegate(new STableViewRepairsItemDelegates());
}

STableViewRepairs::~STableViewRepairs()
{
}

void STableViewRepairs::translateNames()
{
    tr("Title");
    tr("Serial");
    tr("Fault");
    tr("Engineer");
    tr("State");
    tr("RepairAmount");
    tr("InDate");
    tr("OutDate");
    tr("ClientFullName");
    tr("ClientPrimaryPhone");
    tr("Place");
    tr("Manager");
    tr("Progress");
}


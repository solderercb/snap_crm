#include "stableviewrepairs.h"
#include "models/stablerepairsmodel.h"

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

QList<int> *STableViewRepairs::selectedRepairsList()
{
    QList<int> *list = new QList<int>();
    STableRepairsModel *model = static_cast<STableRepairsModel*>(this->model());
    foreach(QModelIndex index, selectionList())
    {
        list->append(model->unformattedData(index).toInt());
    }
    return list;
}

bool STableViewRepairs::selectedCanBeIssued()
{
    STableRepairsModel *model = static_cast<STableRepairsModel*>(this->model());
    bool en = !selectionList().isEmpty();
    int client = 0;
    foreach(QModelIndex index, selectionList())
    {
        int c = model->unformattedData(index.siblingAtColumn(ServiceColumn::ClientId)).toInt();
        int s = model->unformattedData(index.siblingAtColumn(ServiceColumn::StateId)).toInt();
        if(!client)
            client = c;
        else if(client != c)
        {
            en &= 0;
            break;
        }

        if(s != Global::RepStateIds::Ready && s != Global::RepStateIds::ReadyNoRepair)
        {
            en &= 0;
            break;
        }
    }
    return en;
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


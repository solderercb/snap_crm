#include "stableviewclients.h"

STableViewClients::STableViewClients(QWidget *parent) :
    STableViewBase(SLocalSettings::CustomersGrid, parent)
{
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

void STableViewClients::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<STableBaseModel*>(model);
    STableViewBase::setModel(model);
}

void STableViewClients::setColumnWidth(int column, int width)
{
    if(m_model)
        m_model->setColumnWidth(column, (int)(width/m_fontMetrics->averageCharWidth()));

    STableViewBase::setColumnWidth(column, width);
}

void STableViewClients::translateNames()
{
    tr("ID");
    tr("FullName");
    tr("Balance");
    tr("Repairs");
    tr("Purchases");
    tr("Type");
    tr("PrimaryPhone");
    tr("EMail");
}


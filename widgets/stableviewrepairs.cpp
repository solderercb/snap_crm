#include "stableviewrepairs.h"
#include "models/stablerepairsmodel.h"

STableViewRepairs::STableViewRepairs(QWidget *parent) :
    STableViewBase(SLocalSettings::RepairsGrid, parent)
{
    readLayout();

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
        int a = model->unformattedData(index.siblingAtColumn(ServiceColumn::RealRepairCost)).toInt();

        if(!client)
            client = c;
        else if(client != c)
        {
            en &= 0;
            break;
        }

        if(m_mode == ModeCartridges && comSettings->useSimplifiedCartridgeRepair)
        {
            if(!(s == Global::RepStateIds::Ready || s == Global::RepStateIds::ReadyNoRepair || (a > 0 && s == Global::RepStateIds::InWork)))
            {
                en &= 0;
                break;
            }
        }
        else
        {
            if(s != Global::RepStateIds::Ready && s != Global::RepStateIds::ReadyNoRepair)
            {
                en &= 0;
                break;
            }

        }
    }

    return en;
}

void STableViewRepairs::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<STableRepairsModel*>(model);
    STableViewBase::setModel(model);
}

bool STableViewRepairs::mode()
{
    return m_mode;
}

void STableViewRepairs::setMode(const int mode)
{
    m_mode = mode;
    STableViewBase::setLayoutVariant( (mode == ModeCartridges)?(SLocalSettings::SettingsVariant::CartridgesGrid):(SLocalSettings::SettingsVariant::RepairsGrid) );
    readLayout();
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
    tr("ExtNotes");
    tr("DevClass");
}

void STableViewRepairs::setColumnWidth(int column, int width)
{
    if(m_model)
        m_model->setColumnWidth(column, (int)(width/m_fontMetrics->averageCharWidth()));

    STableViewBase::setColumnWidth(column, width);
}

void STableViewRepairs::readLayout()
{
    STableViewBase::readLayout();

    // в АСЦ в таблице ремонтов по умолчанию отображаются значения доп полей;
    // при импорте файла настроек в этих столбцах будут отображаться служебные данные; возможно позже я тоже сделаю отображение данных доп. полей, а пока скрываю их.
    for(int i = metaObject()->enumerator(metaObject()->indexOfEnumerator("Column")).keyCount(); i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        i_gridLayout->$GridControl.Columns[i].Visible = false;
    }
}

void STableViewRepairs::clearModel()
{
    if(!m_model)
        return;

    m_model->clear();
}

void STableViewRepairs::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    if(!m_model)
        return;

    m_model->setQuery(query, database);
}

/*  Переопределённый метод
 *  Загружает недостающие данные за один запрос
*/
void STableViewRepairs::fetchMore(const QModelIndex &parent)
{
    m_model->fetchMore(i_vspOldRowCount - model()->rowCount(), parent);
}

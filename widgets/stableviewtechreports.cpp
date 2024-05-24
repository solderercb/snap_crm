#include "stableviewtechreports.h"
#include "models/stabletechreportsmodel.h"

STableViewTechReports::STableViewTechReports(QWidget *parent) :
    STableViewBase(SLocalSettings::TechReportsGrid, parent)
{
    readLayout();
}

void STableViewTechReports::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<STableTechReportsModel*>(model);
    STableViewBase::setModel(model);
}

QList<int> *STableViewTechReports::selectedReportsList()
{
    QList<int> *list = new QList<int>();
    STableTechReportsModel *model = static_cast<STableTechReportsModel*>(this->model());
    foreach(QModelIndex index, selectionList())
    {
        list->append(model->unformattedData(index).toInt());
    }
    return list;
}

void STableViewTechReports::setColumnWidth(int column, int width)
{
    if(m_model)
        m_model->setColumnWidth(column, (int)(width/m_fontMetrics->averageCharWidth()));

    STableViewBase::setColumnWidth(column, width);
}

void STableViewTechReports::clearModel()
{
    if(!m_model)
        return;

    m_model->clear();
}

void STableViewTechReports::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    if(!m_model)
        return;

    m_model->setQuery(query, database);
}

void STableViewTechReports::translateNames()
{
    tr("Num");
    tr("Created");
    tr("User");
    tr("Company");
    tr("Client");
    tr("Device");
    tr("InvNumber");
    tr("SerialNumber");
    tr("ProductionDate");
    tr("PurchaseDate");
    tr("InitCost");
    tr("ResidualCost");
    tr("Fault");
    tr("DiagResult");
    tr("Conclusion");
    tr("Notes");
}

#include "stableviewtechreports.h"
#include "models/stabletechreportsmodel.h"

STableViewTechReports::STableViewTechReports(QWidget *parent) :
    STableViewBase(SLocalSettings::TechReportsGrid, parent)
{
    readLayout();
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
    tr("ProductionData");
    tr("PurchaseDate");
    tr("InitCost");
    tr("ResidualCost");
    tr("Fault");
    tr("DiagResult");
    tr("Conclusion");
    tr("Notes");
}

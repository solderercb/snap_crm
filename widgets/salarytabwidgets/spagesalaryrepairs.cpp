#include "spagesalaryrepairs.h"
#include "ui_spagesalaryrepairs.h"
#include "widgets/salarytabwidgets/stableviewsalaryrepairs.h"

SPageSalaryRepairs::SPageSalaryRepairs(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryRepairs)
{
    ui->setupUi(this);
    SPageSalaryRepairs::guiFontChanged();
    ui->checkBoxIncludeNotIssued->setChecked(comSettings->salaryIncludeNotIssuedByDefault);

    ui->tableViewRepairsSummary->setGridLayout(ui->tableViewRepairs->gridLayout());
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::signalColumnResized, this, &SPageSalaryRepairs::tableColumnResized);

    ui->tableViewRepairs->setModel(parentTab->m_repairs);
    ui->tableViewRepairWorks->setModel(parentTab->m_repairWorks);
    ui->tableViewRepairParts->setModel(parentTab->m_repairParts);

    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::pressed, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::clicked, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::activated, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::doubleClicked, this, &SPageSalaryRepairs::tableRepairsRowDoubleClicked);
    connect(ui->tableViewRepairParts, &STableViewSalaryRepairs::doubleClicked, this, &SPageSalaryRepairs::tableRepairPartsRowDoubleClicked);
    connect(ui->tableViewRepairs->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPageSalaryRepairs::tableRepairsRowSelected);
    connect(ui->checkBoxShowPayed, &QCheckBox::toggled, this, &SPageSalaryRepairs::showRowsPayed);
}

SPageSalaryRepairs::~SPageSalaryRepairs()
{
    delete ui;
}

void SPageSalaryRepairs::loadRepairData(const int id)
{
    ui->tableViewRepairWorks->setQuery(QUERY_SEL_SALARY_REPAIR_WORKS(
                                           id,
                                           m_userModel->id()));
    ui->tableViewRepairWorks->refresh();

    ui->tableViewRepairParts->setQuery(QUERY_SEL_SALARY_REPAIR_PARTS(
                                           id,
                                           m_userModel->id()));
    ui->tableViewRepairParts->refresh();

}

void SPageSalaryRepairs::enableVScrollbarPadding(const bool state)
{
    ui->gridLayoutRepairs->setColumnMinimumWidth(1, state*ui->tableViewRepairs->verticalScrollBar()->width());
}

void SPageSalaryRepairs::showRowsPayed(bool state)
{
    updateSummary();
    ui->tableViewRepairs->showRowsPayed(state);
}

void SPageSalaryRepairs::updateSummary()
{
    bool excludePayed = !ui->checkBoxShowPayed->isChecked();
    ui->tableViewRepairsSummary->setTotal(STableViewSalaryRepairs::Column::Id, parentTab->m_repairs->visibleRowCount(excludePayed));
    for(int i = STableViewSalaryRepairs::Column::RealRepCost; i < STableViewSalaryRepairs::Column::IssueDate; i++)
    {
        ui->tableViewRepairsSummary->setTotal(i, parentTab->m_repairs->total(i, excludePayed));
    }
}

void SPageSalaryRepairs::updateModels()
{
    QStringList query_group;
    QStringList statuses;
    query_group << "`id`";    // default GROUP part of query

    statuses << QString::number(Global::RepStateIds::Returned);
    statuses << QString::number(Global::RepStateIds::ReturnedNoRepair);
    statuses << QString::number(Global::RepStateIds::ReturnedInCredit);
    if(ui->checkBoxIncludeNotIssued->isChecked())
    {
        statuses << QString::number(Global::RepStateIds::Ready);
        statuses << QString::number(Global::RepStateIds::ReadyNoRepair);
    }

    ui->tableViewRepairs->setQuery(QUERY_SEL_SALARY_REPAIRS(
                                       parentTab->periodBegin(),
                                       parentTab->periodEnd(),
                                       statuses.join(','),
                                       parentTab->employeeId()));

    ui->tableViewRepairs->setGrouping(query_group);
    ui->tableViewRepairs->refresh();

    parentTab->setModelUpdatedFlag(RepairsModel);
}

void SPageSalaryRepairs::updateWidgets()
{
    double notIssuedValue = 0;
    m_userModel = parentTab->m_userModel;
    ui->labelPercentWorksValue->setText(QString::number(m_userModel->payRepair()));
    ui->labelPercentWorksQuickValue->setText(QString::number(m_userModel->payRepairQuick()));
    ui->labelPercentPartsValue->setText(QString::number(m_userModel->pay4SaleInRepair()?m_userModel->paySale():0));
    ui->labelPercentRefillValue->setText(QString::number(m_userModel->payCartridgeRefill()));

    updateSummary();
    if(ui->checkBoxIncludeNotIssued->isChecked())
        notIssuedValue = parentTab->m_repairs->notIssuedTotal(STableSalaryRepairsModel::EmployeeSalaryWorks) + parentTab->m_repairs->notIssuedTotal(STableSalaryRepairsModel::EmployeeSalaryParts);

    ui->labelSalaryValue->setText(sysLocale.toString(parentTab->m_repairs->total(STableSalaryRepairsModel::EmployeeSalaryWorks, STableSalaryReceptedIssued::IncludePayed) + parentTab->m_repairs->total(STableSalaryRepairsModel::EmployeeSalaryParts, STableSalaryReceptedIssued::IncludePayed), 'f', comSettings->classicKassa?2:0));
    ui->labelSalaryCartridgesValue->setText(sysLocale.toString(parentTab->m_repairs->totalForCartridges(), 'f', comSettings->classicKassa?2:0));
    ui->labelSalaryToPayValue->setText(sysLocale.toString(parentTab->m_repairs->total(STableSalaryRepairsModel::EmployeeSalaryWorks) + parentTab->m_repairs->total(STableSalaryRepairsModel::EmployeeSalaryParts), 'f', comSettings->classicKassa?2:0));
    ui->labelNotIssuedValue->setText(sysLocale.toString(notIssuedValue, 'f', comSettings->classicKassa?2:0));
}

void SPageSalaryRepairs::repairsModelReset()
{
}

void SPageSalaryRepairs::tableRepairsRowActivated(const QModelIndex &index)
{
    loadRepairData(parentTab->m_repairs->index(index.row(), STableSalaryRepairsModel::Columns::Id).data().toInt());
}

void SPageSalaryRepairs::tableRepairsRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    Q_UNUSED(prev);
    tableRepairsRowActivated(current);
}

void SPageSalaryRepairs::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(parentTab->m_repairs->index(index.row(), STableSalaryRepairsModel::Columns::Id).data().toInt());
}

void SPageSalaryRepairs::tableRepairPartsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_repairParts->index(index.row(), STableSalaryRepairPartsModel::Columns::ItemId).data().toInt());
}

void SPageSalaryRepairs::tableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableViewRepairs->verticalScrollBar()->isVisible());
    ui->tableViewRepairsSummary->applyGridlayout();
}

void SPageSalaryRepairs::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);
    font.setBold(true);

    ui->labelNotIssuedValue->setFont(font);
    ui->labelPercentPartsValue->setFont(font);
    ui->labelPercentRefillValue->setFont(font);
    ui->labelPercentWorksQuickValue->setFont(font);
    ui->labelPercentWorksValue->setFont(font);
    ui->labelSalaryToPayValue->setFont(font);
    ui->labelSalaryValue->setFont(font);
    ui->labelSalaryCartridgesValue->setFont(font);
}


STableViewSalaryRepairsSummary::STableViewSalaryRepairsSummary(QWidget *parent) :
    STableViewSummaryBase(SLocalSettings::SalaryRepairsGrid, parent)
{
    setBackgroundRole(QPalette::Window);
//    setStyleSheet("background-color: rgb(240, 240, 240);");
}

void STableViewSalaryRepairsSummary::setGridLayout(XtraSerializer *layout)
{
    STableViewSummaryBase::setGridLayout(layout);
    for(int i = STableViewSalaryRepairs::Column::RealRepCost; i < STableViewSalaryRepairs::Column::IssueDate; i++)
    {
        setData(0, i, "<value>");
    }
}

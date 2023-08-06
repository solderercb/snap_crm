#include "spagesalaryrepairs.h"
#include "ui_spagesalaryrepairs.h"
#include "widgets/salarytabwidgets/stableviewsalaryrepairs.h"

SPageSalaryRepairs::SPageSalaryRepairs(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryRepairs)
{
    ui->setupUi(this);
    ui->checkBoxIncludeNotIssued->setChecked(comSettings->salaryIncludeNotIssuedByDefault);

    ui->tableViewRepairsSummary->setGridLayout(ui->tableViewRepairs->gridLayout());
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::signalColumnResized, this, &SPageSalaryRepairs::repairsTableColumnResized);

    ui->tableViewRepairs->setModel(parentTab->m_repairs);
    ui->tableViewRepairWorks->setModel(parentTab->m_repairWorks);
    ui->tableViewRepairParts->setModel(parentTab->m_repairParts);

    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::pressed, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::clicked, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::activated, this, &SPageSalaryRepairs::tableRepairsRowActivated);
    connect(ui->tableViewRepairs, &STableViewSalaryRepairs::doubleClicked, this, &SPageSalaryRepairs::tableRepairsRowDoubleClicked);
    connect(ui->tableViewRepairParts, &STableViewSalaryRepairs::doubleClicked, this, &SPageSalaryRepairs::tableRepairPartsRowDoubleClicked);
    connect(ui->tableViewRepairs->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPageSalaryRepairs::tableRepairsRowSelected);
    connect(ui->checkBoxShowPayed, &QCheckBox::toggled, ui->tableViewRepairs, &STableViewSalaryRepairs::showRowsEarlyPayed);
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

void SPageSalaryRepairs::updateModels()
{
    QStringList query_group;
    query_group << "`id`";    // default GROUP part of query

    ui->tableViewRepairs->setQuery(QUERY_SEL_SALARY_REPAIRS(
                                       parentTab->periodBegin(),
                                       parentTab->periodEnd(),
                                       (ui->checkBoxIncludeNotIssued->isChecked())?"8,12,16,6,7":"8,12,16",
                                       parentTab->employeeId()));

    ui->tableViewRepairs->setGrouping(query_group);
    ui->tableViewRepairs->refresh(ui->checkBoxShowPayed->isChecked());

    parentTab->setModelUpdatedFlag(RepairsModel);
}

void SPageSalaryRepairs::updateWidgets()
{
    double notUssuedValue = 0;
    m_userModel = parentTab->m_userModel;
    ui->labelPercentWorksValue->setText(QString::number(m_userModel->payRepair()));
    ui->labelPercentWorksQuickValue->setText(QString::number(m_userModel->payRepairQuick()));
    ui->labelPercentPartsValue->setText(QString::number(m_userModel->pay4SaleInRepair()?m_userModel->paySale():0));
    ui->labelPercentRefillValue->setText(QString::number(m_userModel->payCartridgeRefill()));
//    ui->labelSalaryValue->setText(QString::number(m_userModel->pay...()));
//    ui->labelNotUssuedValue->setText(QString::number(m_userModel->pay...()));

    ui->tableViewRepairsSummary->setTotal(0, parentTab->m_repairs->rowCount());
    for(int i = 2; i < 8; i++)
    {
        ui->tableViewRepairsSummary->setTotal(i, parentTab->m_repairs->total(i));
    }
    if(ui->checkBoxIncludeNotIssued->isChecked())
        notUssuedValue = parentTab->m_repairs->notIssuedTotal(6) + parentTab->m_repairs->notIssuedTotal(7);

    ui->labelSalaryValue->setText(sysLocale.toString(parentTab->m_repairs->total(6, STableSalaryReceptedIssued::IncludePayed) + parentTab->m_repairs->total(7, STableSalaryReceptedIssued::IncludePayed), 'f', comSettings->classicKassa?2:0));
    ui->labelSalaryToPayValue->setText(sysLocale.toString(parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7), 'f', comSettings->classicKassa?2:0));
    ui->labelNotUssuedValue->setText(sysLocale.toString(notUssuedValue, 'f', comSettings->classicKassa?2:0));
}

void SPageSalaryRepairs::repairsModelReset()
{
}

void SPageSalaryRepairs::tableRepairsRowActivated(const QModelIndex &index)
{
    loadRepairData(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalaryRepairs::tableRepairsRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    tableRepairsRowActivated(current);
}

void SPageSalaryRepairs::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalaryRepairs::tableRepairPartsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalaryRepairs::repairsTableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableViewRepairs->verticalScrollBar()->isVisible());
    ui->tableViewRepairsSummary->applyGridlayout();
}


STableViewSalaryRepairsSummary::STableViewSalaryRepairsSummary(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryRepairsGrid, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    dataModel = new QStandardItemModel();
    setModel(dataModel);
    setBackgroundRole(QPalette::Window);
}

STableViewSalaryRepairsSummary::~STableViewSalaryRepairsSummary()
{
    i_gridLayout = nullptr;
}

void STableViewSalaryRepairsSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    for(int i = 2; i < 8; i++)
    {
        if(i == 6) continue;
        emptyModelData->at(i)->setText("<value>");
    }
    dataModel->appendRow(*emptyModelData);
    setRowHeight(0, this->height());
}

void STableViewSalaryRepairsSummary::setTotal(int column, double value)
{
    dataModel->setData(dataModel->index(0, column), value);
}

void STableViewSalaryRepairsSummary::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}


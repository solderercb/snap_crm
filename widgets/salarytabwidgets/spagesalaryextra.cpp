#include "spagesalaryextra.h"
#include "ui_spagesalaryextra.h"

SPageSalaryExtra::SPageSalaryExtra(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryExtra)
{
    ui->setupUi(this);

    ui->tableViewExtraChargesSummary->setGridLayout(ui->tableViewExtraCharges->gridLayout());
    connect(ui->tableViewExtraCharges, &STableViewSalaryExtra::signalColumnResized, this, &SPageSalaryExtra::repairsTableColumnResized);
    connect(parentTab->m_extraCharges, &STableSalaryExtraModel::repopulate, this, &SPageSalaryExtra::updateModels);

    ui->tableViewExtraCharges->setModel(parentTab->m_extraCharges);
}

SPageSalaryExtra::~SPageSalaryExtra()
{
    delete ui;
}

void SPageSalaryExtra::enableVScrollbarPadding(const bool state)
{
    ui->gridLayoutExtraCharges->setColumnMinimumWidth(1, state*ui->tableViewExtraCharges->verticalScrollBar()->width());
}

void SPageSalaryExtra::updateModels()
{
    ui->tableViewExtraCharges->setQuery(QUERY_SEL_SALARY_EXTRA_CHARGES(\
                                            parentTab->periodBegin(),\
                                            parentTab->periodEnd(),\
                                            parentTab->employeeId()));

    ui->tableViewExtraCharges->refresh();
    parentTab->setModelUpdatedFlag(ExtraChargesModel);
}

void SPageSalaryExtra::updateWidgets()
{
    ui->tableViewExtraChargesSummary->setTotal(0, parentTab->m_extraCharges->rowCount());
    ui->tableViewExtraChargesSummary->setTotal(2, parentTab->m_extraCharges->total(2));
}

void SPageSalaryExtra::repairsTableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableViewExtraCharges->verticalScrollBar()->isVisible());
    ui->tableViewExtraChargesSummary->applyGridlayout();
}

STableViewSalaryExtraSummary::STableViewSalaryExtraSummary(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryExtraChargesGrid, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    dataModel = new QStandardItemModel();
    setModel(dataModel);
    setBackgroundRole(QPalette::Window);
}

STableViewSalaryExtraSummary::~STableViewSalaryExtraSummary()
{
    i_gridLayout = nullptr;
}

void STableViewSalaryExtraSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    emptyModelData->at(0)->setText("<value>");
    emptyModelData->at(2)->setText("<value>");
    dataModel->appendRow(*emptyModelData);
    setRowHeight(0, this->height());
}

void STableViewSalaryExtraSummary::setTotal(int column, double value)
{
    dataModel->setData(dataModel->index(0, column), value);
}

void STableViewSalaryExtraSummary::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}

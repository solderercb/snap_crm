#include "spagesalaryextra.h"
#include "ui_spagesalaryextra.h"

SPageSalaryExtra::SPageSalaryExtra(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryExtra)
{
    ui->setupUi(this);

    ui->tableViewExtraChargesSummary->setGridLayout(ui->tableViewExtraCharges->gridLayout());
    connect(ui->tableViewExtraCharges, &STableViewSalaryExtra::signalColumnResized, this, &SPageSalaryExtra::tableColumnResized);
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
    ui->tableViewExtraChargesSummary->setTotal(STableViewSalaryExtra::Column::ID, parentTab->m_extraCharges->rowCount());
    ui->tableViewExtraChargesSummary->setTotal(STableViewSalaryExtra::Column::Amount, parentTab->m_extraCharges->total(2));
}

void SPageSalaryExtra::tableColumnResized(int, int)
{
    enableVScrollbarPadding(ui->tableViewExtraCharges->verticalScrollBar()->isVisible());
    ui->tableViewExtraChargesSummary->applyGridlayout();
}

STableViewSalaryExtraSummary::STableViewSalaryExtraSummary(QWidget *parent) :
    STableViewSummaryBase(SLocalSettings::SalaryExtraChargesGrid, parent)
{
    setBackgroundRole(QPalette::Window);
//    setStyleSheet("background-color: rgb(240, 240, 240);");
}

void STableViewSalaryExtraSummary::setGridLayout(XtraSerializer *layout)
{
    STableViewSummaryBase::setGridLayout(layout);
    setData(0, STableViewSalaryExtra::Column::ID, "<value>");
    setData(0, STableViewSalaryExtra::Column::Amount, "<value>");
}

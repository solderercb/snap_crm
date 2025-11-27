#include "spagesalaryextra.h"
#include "ui_spagesalaryextra.h"
#include <QScrollBar>
#include <ProjectQueries>
#include <tabSalary>
#include <STableViewBase>
#include <FlashPopup>
#include <SSalaryExtraModel>
#include <SSalaryExtraView>

SPageSalaryExtra::SPageSalaryExtra(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryExtra)
{
    ui->setupUi(this);

    ui->tableViewExtraChargesSummary->setGridLayout(ui->tableViewExtraCharges->gridLayout());
    connect(ui->tableViewExtraCharges, &STableViewSalaryExtra::signalColumnResized, this, &SPageSalaryExtra::tableColumnResized);

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


int SPageSalaryExtra::checkInput()
{
    return (parentTab->m_extraCharges->rowCount() == 0);
}

QString SPageSalaryExtra::queryLogFile()
{
    return parentTab->metaObject()->className();
}

void SPageSalaryExtra::commit(const int)
{
    parentTab->m_extraCharges->saveTable();
}

void SPageSalaryExtra::endCommit()
{
    auto *p = new shortlivedNotification(this, tr("Начисления и списания"), tr("Список успешно сохранён"), QColor(214,239,220), QColor(229,245,234));
    Q_UNUSED(p);
    updateModels();
}


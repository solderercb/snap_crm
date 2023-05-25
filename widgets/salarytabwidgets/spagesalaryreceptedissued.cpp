#include "spagesalaryreceptedissued.h"
#include "ui_spagesalaryreceptedissued.h"

SPageSalaryReceptedIssued::SPageSalaryReceptedIssued(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryReceptedIssued)
{
    ui->setupUi(this);

    ui->tableViewRecepted->setModel(parentTab->m_recepted);
    ui->tableViewIssued->setModel(parentTab->m_issued);
}

SPageSalaryReceptedIssued::~SPageSalaryReceptedIssued()
{
    delete ui;
}

void SPageSalaryReceptedIssued::updateModels()
{
    ui->tableViewRecepted->setQuery(QUERY_SEL_SALARY_RECEPTED_DEVICES(
                                        parentTab->periodBegin(),
                                        parentTab->periodEnd(),
                                        parentTab->employeeId()));
    ui->tableViewRecepted->refresh();
    parentTab->setModelUpdatedFlag(ReceptedModel);

    ui->tableViewIssued->setQuery(QUERY_SEL_SALARY_ISSUED_DEVICES(
                                      parentTab->periodBegin(),
                                      parentTab->periodEnd(),
                                      parentTab->employeeId()));
    ui->tableViewIssued->refresh();
    parentTab->setModelUpdatedFlag(IssuedModel);

    updateWidgets();
}

void SPageSalaryReceptedIssued::updateWidgets()
{
    int recepted = parentTab->m_recepted->rowCount();
    int issued = parentTab->m_issued->rowCount();
    double summReceptedIssued = parentTab->m_recepted->total(4) + parentTab->m_issued->total(4);
    ui->labelPayForReceptValue->setText(QString::number(parentTab->m_userModel->payDeviceIn()));
    ui->labelPayForIssueValue->setText(QString::number(parentTab->m_userModel->payDeviceOut()));
    ui->labelReceptedSummaryValue->setText(QString::number(recepted));
    ui->labelIssuedSummaryValue->setText(QString::number(issued));
    ui->labelRepairsSummaryValue->setText(QString::number(recepted + issued));
    ui->labelSummValue->setText(sysLocale.toString(summReceptedIssued, 'f', comSettings->value("classic_kassa").toBool()?2:0));
}

void SPageSalaryReceptedIssued::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(index.model()->index(index.row(), 0).data().toInt());
}

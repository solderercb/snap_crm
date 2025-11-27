#include "spagesalaryreceptedissued.h"
#include "ui_spagesalaryreceptedissued.h"
#include <QWidget>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SComSettings>
#include <Mainwindow>
#include <tabSalary>
#include <SUserSettings>
#include <SUserModel>
#include <SSalaryReceptedIssuedModel>

SPageSalaryReceptedIssued::SPageSalaryReceptedIssued(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalaryReceptedIssued)
{
    ui->setupUi(this);
    SPageSalaryReceptedIssued::guiFontChanged();

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
    double summReceptedIssued = parentTab->m_recepted->total(STableSalaryReceptedIssued::Columns::PayDeviceIn) + parentTab->m_issued->total(STableSalaryReceptedIssued::Columns::PayDeviceOut);
    ui->labelPayForReceptValue->setText(QString::number(parentTab->m_userModel->payDeviceIn()));
    ui->labelPayForIssueValue->setText(QString::number(parentTab->m_userModel->payDeviceOut()));
    ui->labelReceptedSummaryValue->setText(QString::number(recepted));
    ui->labelIssuedSummaryValue->setText(QString::number(issued));
    ui->labelRepairsSummaryValue->setText(QString::number(recepted + issued));
    ui->labelSummValue->setText(sysLocale.toString(summReceptedIssued, 'f', comSettings->classicKassa()?2:0));
}

void SPageSalaryReceptedIssued::tableRepairsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabRepair(index.model()->index(index.row(), STableSalaryReceptedIssued::Columns::Id).data().toInt());
}

void SPageSalaryReceptedIssued::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());
    font.setBold(true);

    ui->labelIssuedSummaryValue->setFont(font);
    ui->labelPayForIssueValue->setFont(font);
    ui->labelPayForReceptValue->setFont(font);
    ui->labelReceptedSummaryValue->setFont(font);
    ui->labelRepairsSummaryValue->setFont(font);
    ui->labelSummValue->setFont(font);
}

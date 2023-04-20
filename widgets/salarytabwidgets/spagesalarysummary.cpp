#include "spagesalarysummary.h"
#include "ui_spagesalarysummary.h"

SPageSalarySummary::SPageSalarySummary(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySummary)
{
    ui->setupUi(this);
    connect(parentTab->m_userModel, &SUserModel::sigModelReset, this, &SPageSalarySummary::userModelReset);
    ui->toolButtonApplySummaryMonthCharge->resize(ui->doubleSpinBoxSummaryMonthCharge->height(), ui->doubleSpinBoxSummaryMonthCharge->height());
    ui->dateEditSubsistenceDate->setDate(QDate::currentDate());
    ui->dateEditSalaryDate->setDate(QDate::currentDate());

    paymentSystemsProxyModel = new SSortFilterProxyModel();
    paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(-2)|(-1)).*$"));
    paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxSalaryPaymentSystem->setModel(paymentSystemsProxyModel);
    ui->comboBoxSalaryPaymentSystem->setCurrentIndex(-1);
    ui->comboBoxSubsistencePaymentSystem->setModel(paymentSystemsProxyModel);
    ui->comboBoxSubsistencePaymentSystem->setCurrentIndex(-1);
}

SPageSalarySummary::~SPageSalarySummary()
{
    delete ui;
}

void SPageSalarySummary::updateModels()
{
    updateWidgets();
}

void SPageSalarySummary::userModelReset()
{
    parentTab->setModelUpdatedFlag(0);
}

void SPageSalarySummary::updateWidgets()
{
    double earning = 0;
    ui->groupBoxSubsistence->setVisible(parentTab->m_showSubsistanceGroup);
    ui->doubleSpinBoxSummaryEarningsRepairs->setValue(parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7));
    earning += parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7);
    ui->doubleSpinBoxSummaryMonthRate->setValue(parentTab->m_userModel->salaryRate(parentTab->m_periodBegin.date()));
    ui->doubleSpinBoxSummaryMonthCharge->setValue(parentTab->m_userModel->salaryRate(parentTab->m_periodBegin.date()));
    ui->doubleSpinBoxSummaryEarningsReceiptIssue->setValue(parentTab->summRecepedIssued());
    earning += parentTab->summRecepedIssued();
    ui->doubleSpinBoxEarning->setValue(earning);
}

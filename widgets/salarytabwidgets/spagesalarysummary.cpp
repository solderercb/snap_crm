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

    connect(ui->toolButtonApplySummaryMonthCharge, &QToolButton::clicked, this, &SPageSalarySummary::setMonthCharge);
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
    m_monthCharge = parentTab->m_userModel->salaryRate(parentTab->m_periodBegin.date());
    parentTab->setModelUpdatedFlag(UserModel);
}

void SPageSalarySummary::updateWidgets()
{
    double earningWorksAndParts = parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7);   // в АСЦ в поле "Заработок с ремонтов" только сумма за работы; сумма за детали в ремонтах прибавляется к сумме за проданные и отображается в поле "Заработок с продаж"
    double earningReceptedIssued = parentTab->m_recepted->total() + parentTab->m_issued->total();
    double earningSales = parentTab->m_sales->total(5);
    double earning = 0;
    ui->groupBoxSubsistence->setVisible(parentTab->m_showSubsistanceGroup);
    ui->doubleSpinBoxSummaryEarningsRepairs->setValue(earningWorksAndParts);
    earning += earningWorksAndParts;
    ui->doubleSpinBoxSummaryMonthRate->setValue(parentTab->m_userModel->salaryRate(parentTab->m_periodBegin.date()));
    ui->doubleSpinBoxSummaryMonthCharge->setValue(m_monthCharge);
    ui->doubleSpinBoxSummaryEarningsSales->setValue(earningSales);
    ui->doubleSpinBoxSummaryEarningsReceiptIssue->setValue(earningReceptedIssued);
    earning += earningSales;
    earning += earningReceptedIssued;
    earning += ui->doubleSpinBoxSummaryMonthCharge->value();
    ui->doubleSpinBoxEarning->setValue(earning);
    ui->doubleSpinBoxPayed->setValue(parentTab->m_payments->total(2));
    ui->doubleSpinBoxBalance->setValue(parentTab->m_userModel->balance(parentTab->m_periodBegin));
}

void SPageSalarySummary::setMonthCharge()
{
    m_monthCharge = ui->doubleSpinBoxSummaryMonthCharge->value();
    updateWidgets();
}

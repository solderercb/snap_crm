#include "spagesalarysummary.h"
#include "ui_spagesalarysummary.h"

SPageSalarySummary::SPageSalarySummary(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySummary)
{
    ui->setupUi(this);
    ui->textEditDisclaimer->setVisible(ui->toolButtonDisclaimer->isChecked());
    connect(parentTab, &tabSalary::showSubsistanceGroup, this, &SPageSalarySummary::setGroupBoxSubsistanceVisible);
    connect(parentTab, &tabSalary::setFillMonthChargeOnUpdate, this, &SPageSalarySummary::setFillMonthChargeOnUpdate);
    ui->toolButtonApplySummaryMonthCharge->resize(ui->doubleSpinBoxSummaryMonthCharge->height(), ui->doubleSpinBoxSummaryMonthCharge->height());
    ui->dateEditSubsistenceDate->setDate(QDate::currentDate());
    ui->dateEditSalaryDate->setDate(QDate::currentDate());
    ui->doubleSpinBoxBalance->setMinimum(-ui->doubleSpinBoxBalance->maximum());
    ui->doubleSpinBoxSummaryAmountToPay->setMinimum(-ui->doubleSpinBoxSummaryAmountToPay->maximum());

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

void SPageSalarySummary::setFillMonthChargeOnUpdate(const bool state)
{
    m_fillMonthChargeOnUpdate = state;
}

void SPageSalarySummary::setGroupBoxSubsistanceVisible(bool visible)
{
    ui->groupBoxSubsistence->setVisible(visible);
}

void SPageSalarySummary::updateModels()
{
}

void SPageSalarySummary::updateWidgets()
{
    double monthRate = parentTab->m_userModel->salaryRate();
    double monthCharge = 0;
    double earningWorksAndParts = parentTab->m_repairs->total(6) + parentTab->m_repairs->total(7);   // в АСЦ в поле "Заработок с ремонтов" только сумма за работы; сумма за детали в ремонтах прибавляется к сумме за проданные и отображается в поле "Заработок с продаж"
    double earningReceptedIssued = parentTab->m_recepted->total() + parentTab->m_issued->total();
    double earningSales = parentTab->m_sales->total(5);
    double extraCharges = parentTab->m_extraCharges->total(2, 1);
    double extraChargesOff = parentTab->m_extraCharges->total(2, -1);
    double earning = 0;
    double paymentsSubsistanceSumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Subsistance);
    double paymentsSalarySumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Salary);
    double employeeBalance = parentTab->m_userModel->balance(parentTab->m_periodBegin);
    if(m_fillMonthChargeOnUpdate)
    {
        monthCharge = monthRate;
        m_fillMonthChargeOnUpdate = 0;
    }

    ui->doubleSpinBoxSummaryEarningsRepairs->setValue(earningWorksAndParts);
    earning += earningWorksAndParts;
    ui->doubleSpinBoxSummaryMonthRate->setValue(monthRate);
    ui->doubleSpinBoxSummaryMonthCharge->setValue(monthCharge);
    ui->doubleSpinBoxSummaryEarningsSales->setValue(earningSales);
    ui->doubleSpinBoxSummaryEarningsReceiptIssue->setValue(earningReceptedIssued);
    ui->doubleSpinBoxSummaryItemsCost->setValue(parentTab->m_items->totalUnusedItemsCost());
    ui->doubleSpinBoxExtraCharges->setValue(extraCharges);
    if(extraCharges)
        ui->doubleSpinBoxExtraCharges->setStyleSheet("background-color: rgb(144,238,144);");
    else
        ui->doubleSpinBoxExtraCharges->setStyleSheet("");
    ui->doubleSpinBoxExtraChargesOff->setValue(-extraChargesOff);
    if(extraChargesOff)
        ui->doubleSpinBoxExtraChargesOff->setStyleSheet("background-color: rgb(234,73,73);");
    else
        ui->doubleSpinBoxExtraChargesOff->setStyleSheet("");
    earning += earningSales;
    earning += earningReceptedIssued;
    earning += monthCharge;
    earning += extraCharges + extraChargesOff;
    ui->doubleSpinBoxEarning->setValue(earning);
    ui->doubleSpinBoxSubsistence->setValue(paymentsSubsistanceSumm);
    ui->doubleSpinBoxPayed->setValue(paymentsSalarySumm);
    ui->doubleSpinBoxBalance->setValue(employeeBalance);
    ui->doubleSpinBoxSummaryAmountToPay->setValue(employeeBalance + earning - paymentsSubsistanceSumm - paymentsSalarySumm);
    ui->doubleSpinBoxSalarySumm->setValue(employeeBalance + earning - paymentsSubsistanceSumm - paymentsSalarySumm);
}

/*  Кнопка справа от поля "По ставке начислить" должна выполнять те же действия, что и кнопка "Загрузить" на нижней панели,
 *  кроме автозаполнения поля (поведение аналогично АСЦ).
*/
void SPageSalarySummary::setMonthCharge()
{
    if(parentTab->checkInput())
        return;
    parentTab->updateModels();
    updateWidgets();
}
